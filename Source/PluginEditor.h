#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class MiniSynthesizerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MiniSynthesizerAudioProcessorEditor (MiniSynthesizerAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~MiniSynthesizerAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    MiniSynthesizerAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& valueTreeState;

    juce::Slider osc1TuningSlider;
    juce::Slider osc2TuningSlider;
    juce::Label osc1Label;
    juce::Label osc2Label;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc1TuningAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc2TuningAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MiniSynthesizerAudioProcessorEditor)
};
