#pragma once

#include <vector>
#include <complex>

class STFTEngine
{
public:
	STFTEngine(const int frameSamples);

	void processBlock(float* buffer, const int numSamples, const float semitones);
private:
	int frameSamples;
	int analysisHopSamples;
	int synthesisHopSamples;
	int analysisWritePointer;
	int synthesisReadPointer;
	int synthesisWritePointer;
	int samplesSinceLastFrame;

	float semitones;

	std::vector<std::complex<float>> buffer;
	std::vector<std::complex<float>> twiddles;
	std::vector<float> window;
	std::vector<float> normalization;
	std::vector<float> analysisBuffer;
	std::vector<float> synthesisBuffer;
	std::vector<float> previousAnalysisPhases;
	std::vector<float> synthesisPhases;
	std::vector<float> shiftedBins;
	std::vector<float> shiftedMagnitude;
	std::vector<float> shiftedFrequency;
	std::vector<float> interpolationAbscissa;
	std::vector<float> interpolatedMagnitude;
	std::vector<float> interpolatedFrequency;

	void push(const float* data, const int length);
	void pop(float* data, const int length);
	void processFrame();
};