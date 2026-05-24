#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class PitchShiftPhaseVocoderAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
	PitchShiftPhaseVocoderAudioProcessorEditor(PitchShiftPhaseVocoderAudioProcessor&);
	~PitchShiftPhaseVocoderAudioProcessorEditor() override = default;

	void paint(juce::Graphics&) override;
	void resized() override;

private:
	PitchShiftPhaseVocoderAudioProcessor& audioProcessor;

	juce::Slider slider;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachment;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PitchShiftPhaseVocoderAudioProcessorEditor)
};
