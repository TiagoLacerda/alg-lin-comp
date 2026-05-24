#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <memory>

PitchShiftPhaseVocoderAudioProcessorEditor::PitchShiftPhaseVocoderAudioProcessorEditor(PitchShiftPhaseVocoderAudioProcessor& p)
	: AudioProcessorEditor(&p), audioProcessor(p)
{
	auto* parameter = audioProcessor.apvts.getParameter("semitones");
	auto* parameterFloat = dynamic_cast<juce::AudioParameterFloat*>(parameter);

	if (parameterFloat != nullptr)
	{
		auto range = parameterFloat->range;

		slider.setSliderStyle(juce::Slider::LinearHorizontal);
		slider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);

		/*slider.setRange(range.start, range.end, range.interval);
		slider.setValue(parameterFloat->get(), juce::dontSendNotification);*/

		addAndMakeVisible(slider);

		sliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
			audioProcessor.apvts,
			"semitones",
			slider
		));

	}

	setSize(400, 120);
}

void PitchShiftPhaseVocoderAudioProcessorEditor::paint(juce::Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void PitchShiftPhaseVocoderAudioProcessorEditor::resized()
{
	slider.setBounds(20, 40, 300, 20);
}
