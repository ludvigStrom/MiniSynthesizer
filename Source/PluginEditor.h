#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class MiniSynthesizerAudioProcessorEditor : public juce::AudioProcessorEditor
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

    juce::Slider osc1AttackSlider;
    juce::Slider osc1DecaySlider;
    juce::Slider osc1SustainSlider;
    juce::Slider osc1ReleaseSlider;
    juce::Slider osc2AttackSlider;
    juce::Slider osc2DecaySlider;
    juce::Slider osc2SustainSlider;
    juce::Slider osc2ReleaseSlider;

    juce::Label osc1AttackLabel;
    juce::Label osc1DecayLabel;
    juce::Label osc1SustainLabel;
    juce::Label osc1ReleaseLabel;
    juce::Label osc2AttackLabel;
    juce::Label osc2DecayLabel;
    juce::Label osc2SustainLabel;
    juce::Label osc2ReleaseLabel;
    
    juce::Label osc1PWMLabel;
    juce::Label osc1RangeLabel;
    juce::Label osc1WaveformLabel;

    juce::Label osc2PWMLabel;
    juce::Label osc2RangeLabel;
    juce::Label osc2WaveformLabel;
    
    juce::Slider osc1VolumeSlider;
    juce::Slider osc2VolumeSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc1VolumeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc2VolumeAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc1TuningAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc2TuningAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc1AttackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc1DecayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc1SustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc1ReleaseAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc2AttackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc2DecayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc2SustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc2ReleaseAttachment;

    juce::Slider osc1PWMSlider;
    juce::Slider osc1RangeSlider;
    juce::Slider osc1WaveformSlider;
    juce::Slider osc2PWMSlider;
    juce::Slider osc2RangeSlider;
    juce::Slider osc2WaveformSlider;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc1PWMAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc1RangeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc1WaveformAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc2PWMAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc2RangeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc2WaveformAttachment;
    
    juce::ToggleButton bitcrusherToggle;
    juce::Slider bitDepthSlider;
    juce::Slider sampleRateReductionSlider;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> bitcrusherToggleAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bitDepthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sampleRateReductionAttachment;
    
    juce::Slider formantFrequency1Slider;
    juce::Slider formantFrequency2Slider;
    juce::Slider formantFrequency3Slider;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> formantFrequency1Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> formantFrequency2Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> formantFrequency3Attachment;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MiniSynthesizerAudioProcessorEditor)
};
