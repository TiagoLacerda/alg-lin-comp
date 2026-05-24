#include "STFTEngine.hpp"

#include <algorithm>

#include "FourierTransform.hpp"
#include "Util.hpp"

STFTEngine::STFTEngine(const int frameSamples)
{
	this->frameSamples = frameSamples;
	this->analysisHopSamples = frameSamples / 4;
	this->synthesisHopSamples = frameSamples / 4;
	this->analysisWritePointer = 0;
	this->synthesisReadPointer = frameSamples;
	this->synthesisWritePointer = 0;
	this->samplesSinceLastFrame = 0;
	this->semitones = 1.0f;
	this->buffer.resize(frameSamples, std::complex<float>(0.0f, 0.0f));
	this->twiddles = FFT::computeTwiddles(frameSamples);
	this->window = computeSqrtHanning(frameSamples);
	this->normalization.resize(frameSamples, 0.0f);
	this->analysisBuffer.resize(frameSamples * 4, 0.0f);
	this->synthesisBuffer.resize(frameSamples * 4, 0.0f);
	this->previousAnalysisPhases.resize(frameSamples / 2 + 1, 0.0f);
	this->synthesisPhases.resize(frameSamples / 2 + 1, 0.0f);
	this->shiftedBins.resize(frameSamples / 2 + 1, 0.0f);
	this->interpolationAbscissa.resize(frameSamples / 2 + 1, 0.0f);
	this->shiftedMagnitude.resize(frameSamples / 2 + 1, 0.0f);
	this->shiftedFrequency.resize(frameSamples / 2 + 1, 0.0f);

	for (int k = 0; k <= frameSamples / 2; ++k)
	{
		interpolationAbscissa[k] = static_cast<float>(k);
	}

	for (int frame = 0; frame < frameSamples / synthesisHopSamples; ++frame)
	{
		int offset = frame * synthesisHopSamples;

		for (int i = 0; i < frameSamples; ++i)
		{
			int index = (i + offset) % frameSamples;

			normalization[index] += window[i] * window[i];
		}
	}
}

void STFTEngine::processBlock(float* buffer, const int numSamples, const float semitones)
{
	if (semitones != this->semitones) {

		this->semitones = semitones;
		this->synthesisPhases = this->previousAnalysisPhases;
	}

	push(buffer, numSamples);
	pop(buffer, numSamples);
}

void STFTEngine::push(const float* data, const int length)
{
	for (int i = 0; i < length; ++i)
	{
		analysisBuffer[analysisWritePointer] = data[i];

		analysisWritePointer = (analysisWritePointer + 1) % analysisBuffer.size();

		samplesSinceLastFrame++;

		while (samplesSinceLastFrame >= analysisHopSamples)
		{
			processFrame();

			samplesSinceLastFrame -= analysisHopSamples;
		}
	}
}

void STFTEngine::pop(float* data, const int length)
{
	for (int i = 0; i < length; ++i)
	{
		data[i] = synthesisBuffer[synthesisReadPointer];

		synthesisBuffer[synthesisReadPointer] = 0.0f;

		synthesisReadPointer = (synthesisReadPointer + 1) % synthesisBuffer.size();
	}
}

void STFTEngine::processFrame()
{
	std::fill(shiftedMagnitude.begin(), shiftedMagnitude.end(), 0.0f);
	std::fill(shiftedFrequency.begin(), shiftedFrequency.end(), 0.0f);

	constexpr float pi = 3.14159265358979323846f;
	constexpr float tau = 2.0f * pi;

	float pitchRatio = std::pow(2.0f, semitones / 12.0f);

	// 

	int start = analysisWritePointer - frameSamples;

	if (start < 0)
	{
		start += analysisBuffer.size();
	}

	for (int i = 0; i < frameSamples; ++i)
	{
		int index = (start + i) % analysisBuffer.size();

		buffer[i] = std::complex<float>(analysisBuffer[index] * window[i], 0.0f);
	}

	FFT::fastFourierTransform(buffer, twiddles);

	// Analysis

	for (int k = 0; k <= frameSamples / 2; ++k)
	{
		float real = buffer[k].real();
		float imag = buffer[k].imag();
		float magnitude = std::hypot(real, imag);
		float phase = std::atan2(imag, real);
		float expectedPhaseAdvance = tau * static_cast<float>(k) * static_cast<float>(analysisHopSamples) / static_cast<float>(frameSamples);
		float deltaPhase = phase - previousAnalysisPhases[k];
		float residualPhase = wrapAngle(deltaPhase - expectedPhaseAdvance);
		float trueFrequency = tau * static_cast<float>(k) / static_cast<float>(frameSamples);

		trueFrequency += residualPhase / static_cast<float>(analysisHopSamples);

		previousAnalysisPhases[k] = phase;

		shiftedBins[k] = static_cast<float>(k) * pitchRatio;
		shiftedMagnitude[k] = magnitude;
		shiftedFrequency[k] = trueFrequency * pitchRatio;
	}

	// Synthesis

	interpolate(shiftedBins, shiftedMagnitude, interpolationAbscissa, interpolatedMagnitude);
	interpolate(shiftedBins, shiftedFrequency, interpolationAbscissa, interpolatedFrequency);

	for (int k = 0; k <= frameSamples / 2; ++k)
	{
		synthesisPhases[k] += interpolatedFrequency[k] * static_cast<float>(synthesisHopSamples);
		synthesisPhases[k] = wrapAngle(synthesisPhases[k]);

		buffer[k] = std::polar(interpolatedMagnitude[k], synthesisPhases[k]);
	}

	// Synthesis - DC, Nyquist and hermitian symmetry

	buffer[0] = std::complex<float>(buffer[0].real(), 0.0f);
	buffer[frameSamples / 2] = std::complex<float>(buffer[frameSamples / 2].real(), 0.0f);

	for (int k = 1; k < frameSamples / 2; ++k)
	{
		buffer[frameSamples - k] = std::conj(buffer[k]);
	}

	//

	FFT::inverseFastFourierTransform(buffer, twiddles);

	for (int i = 0; i < frameSamples; ++i)
	{
		int index = (synthesisWritePointer + i) % synthesisBuffer.size();

		synthesisBuffer[index] += buffer[i].real() * window[i] / normalization[i];
	}

	synthesisWritePointer = (synthesisWritePointer + synthesisHopSamples) % synthesisBuffer.size();
}