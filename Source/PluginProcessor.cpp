#include "PluginProcessor.h"
#include "STFTEngine.hpp"

#include <complex>
#include <cmath>

PitchShiftPhaseVocoderAudioProcessor::PitchShiftPhaseVocoderAudioProcessor() : AudioProcessor(BusesProperties()
	.withInput("Input", juce::AudioChannelSet::stereo(), true)
	.withOutput("Output", juce::AudioChannelSet::stereo(), true)),
	apvts(*this, nullptr, "Parameters", createParameters())

{
}

void PitchShiftPhaseVocoderAudioProcessor::prepareToPlay(
	double sampleRate,
	int maximumExpectedSamplesPerBlock)
{
	engines.clear();

	auto size = 1024 * 4;

	for (int i = 0; i < getTotalNumInputChannels(); ++i)
	{
		engines.emplace_back(size);
	}

	setLatencySamples(size);
}

void PitchShiftPhaseVocoderAudioProcessor::processBlock(
	juce::AudioBuffer<float>& buffer,
	juce::MidiBuffer&)
{
	juce::ScopedNoDenormals noDenormals;

	const int numChannels = buffer.getNumChannels();
	const int numSamples = buffer.getNumSamples();
	float semitones = apvts.getRawParameterValue("semitones")->load();

	for (int channel = 0; channel < numChannels; ++channel)
	{
		auto* pointer = buffer.getWritePointer(channel);

		auto& engine = engines[channel];

		engine.processBlock(pointer, numSamples, semitones);
	}
}

juce::AudioProcessorValueTreeState::ParameterLayout PitchShiftPhaseVocoderAudioProcessor::createParameters()
{
	std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

	params.push_back(
		std::make_unique<juce::AudioParameterFloat>(
			"semitones",
			"Semitones",
			juce::NormalisableRange<float>(
				-12.0f,
				12.0f,
				0.001f
			),
			0.00f
		)
	);

	return { params.begin(), params.end() };
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new PitchShiftPhaseVocoderAudioProcessor();
}