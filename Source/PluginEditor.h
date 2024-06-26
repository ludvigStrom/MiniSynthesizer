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

    juce::ComboBox osc1RangeComboBox;
    juce::ComboBox osc2RangeComboBox;
    juce::ComboBox osc1WaveformComboBox;
    juce::ComboBox osc2WaveformComboBox;
    juce::Slider osc1TuningSlider;
    juce::Slider osc2TuningSlider;
    juce::Slider osc1PWMSlider;
    juce::Slider osc2PWMSlider;
    juce::Label osc1Label;
    juce::Label osc2Label;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> osc1RangeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> osc2RangeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> osc1WaveformAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> osc2WaveformAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc1TuningAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc2TuningAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc1PWMAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc2PWMAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MiniSynthesizerAudioProcessorEditor)
};
