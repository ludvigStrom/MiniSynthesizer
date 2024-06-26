#include "PluginProcessor.h"
#include "PluginEditor.h"

MiniSynthesizerAudioProcessorEditor::MiniSynthesizerAudioProcessorEditor (MiniSynthesizerAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState (vts)
{
    // Oscillator 1 Tuning Slider
    osc1TuningSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    osc1TuningSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    osc1TuningSlider.setPopupDisplayEnabled (true, false, this);
    osc1TuningSlider.setTextValueSuffix (" semitones");
    addAndMakeVisible (&osc1TuningSlider);

    osc1TuningAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (valueTreeState, "osc1tuning", osc1TuningSlider));

    osc1Label.setText ("Oscillator 1 Tuning", juce::dontSendNotification);
    osc1Label.attachToComponent (&osc1TuningSlider, false);
    addAndMakeVisible (&osc1Label);

    // Oscillator 2 Tuning Slider
    osc2TuningSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    osc2TuningSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    osc2TuningSlider.setPopupDisplayEnabled (true, false, this);
    osc2TuningSlider.setTextValueSuffix (" semitones");
    addAndMakeVisible (&osc2TuningSlider);

    osc2TuningAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (valueTreeState, "osc2tuning", osc2TuningSlider));

    osc2Label.setText ("Oscillator 2 Tuning", juce::dontSendNotification);
    osc2Label.attachToComponent (&osc2TuningSlider, false);
    addAndMakeVisible (&osc2Label);

    setSize (400, 300);
}

MiniSynthesizerAudioProcessorEditor::~MiniSynthesizerAudioProcessorEditor()
{
}

void MiniSynthesizerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("MiniSynthesizer", getLocalBounds(), juce::Justification::centredTop, 1);
}

void MiniSynthesizerAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    auto topPadding = 40;
    area.removeFromTop(topPadding);

    auto sliderWidth = area.getWidth() / 2;

    osc1TuningSlider.setBounds (area.removeFromLeft (sliderWidth).reduced (10));
    osc2TuningSlider.setBounds (area.reduced (10));
}
