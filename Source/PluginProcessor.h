#pragma once

#include <JuceHeader.h>
#include "STFTEngine.hpp"

class PitchShiftPhaseVocoderAudioProcessor : public juce::AudioProcessor
{
public:
	PitchShiftPhaseVocoderAudioProcessor();

	void processBlock(
		juce::AudioBuffer<float>&,
		juce::MidiBuffer&) override;

	const juce::String getName() const override { return "PitchShiftPhaseVocoder"; }

	bool acceptsMidi() const override { return false; }
	bool producesMidi() const override { return false; }
	bool isMidiEffect() const override { return false; }

	double getTailLengthSeconds() const override { return 0.0; }

	int getNumPrograms() override { return 1; }
	int getCurrentProgram() override { return 0; }
	void setCurrentProgram(int) override {}
	const juce::String getProgramName(int) override { return {}; }
	void changeProgramName(int, const juce::String&) override {}

	void prepareToPlay(double, int) override;
	void releaseResources() override {}

	bool hasEditor() const override { return false; }
	juce::AudioProcessorEditor* createEditor() override { return nullptr; }

	void getStateInformation(juce::MemoryBlock&) override {}
	void setStateInformation(const void*, int) override {}
	
	juce::AudioProcessorValueTreeState apvts;
private:
	std::vector<STFTEngine> engines;
	static juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
};