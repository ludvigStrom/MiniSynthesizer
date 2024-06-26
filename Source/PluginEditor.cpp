#include "PluginProcessor.h"
#include "PluginEditor.h"

MiniSynthesizerAudioProcessorEditor::MiniSynthesizerAudioProcessorEditor (MiniSynthesizerAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState (vts)
{
    // Oscillator 1 Range ComboBox
    osc1RangeComboBox.addItemList({"32'", "16'", "8'", "4'", "2'"}, 1);
    addAndMakeVisible(&osc1RangeComboBox);
    osc1RangeAttachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(valueTreeState, "osc1range", osc1RangeComboBox));

    // Oscillator 1 Waveform ComboBox
    osc1WaveformComboBox.addItemList({"Sine", "Triangle", "Pulse"}, 1);
    addAndMakeVisible(&osc1WaveformComboBox);
    osc1WaveformAttachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(valueTreeState, "osc1waveform", osc1WaveformComboBox));

    // Oscillator 1 Tuning Slider
    osc1TuningSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    osc1TuningSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    osc1TuningSlider.setPopupDisplayEnabled(true, false, this);
    osc1TuningSlider.setTextValueSuffix(" semitones");
    addAndMakeVisible(&osc1TuningSlider);
    osc1TuningAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "osc1tuning", osc1TuningSlider));

    // Oscillator 1 PWM Slider
    osc1PWMSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    osc1PWMSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    osc1PWMSlider.setPopupDisplayEnabled(true, false, this);
    osc1PWMSlider.setTextValueSuffix(" PWM");
    addAndMakeVisible(&osc1PWMSlider);
    osc1PWMAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "osc1pwm", osc1PWMSlider));

    osc1Label.setText("Oscillator 1", juce::dontSendNotification);
    addAndMakeVisible(&osc1Label);

    // Oscillator 2 Range ComboBox
    osc2RangeComboBox.addItemList({"32'", "16'", "8'", "4'", "2'"}, 1);
    addAndMakeVisible(&osc2RangeComboBox);
    osc2RangeAttachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(valueTreeState, "osc2range", osc2RangeComboBox));

    // Oscillator 2 Waveform ComboBox
    osc2WaveformComboBox.addItemList({"Sine", "Triangle", "Pulse"}, 1);
    addAndMakeVisible(&osc2WaveformComboBox);
    osc2WaveformAttachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(valueTreeState, "osc2waveform", osc2WaveformComboBox));

    // Oscillator 2 Tuning Slider
    osc2TuningSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    osc2TuningSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    osc2TuningSlider.setPopupDisplayEnabled(true, false, this);
    osc2TuningSlider.setTextValueSuffix(" semitones");
    addAndMakeVisible(&osc2TuningSlider);
    osc2TuningAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "osc2tuning", osc2TuningSlider));

    // Oscillator 2 PWM Slider
    osc2PWMSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    osc2PWMSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    osc2PWMSlider.setPopupDisplayEnabled(true, false, this);
    osc2PWMSlider.setTextValueSuffix(" PWM");
    addAndMakeVisible(&osc2PWMSlider);
    osc2PWMAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "osc2pwm", osc2PWMSlider));

    osc2Label.setText("Oscillator 2", juce::dontSendNotification);
    addAndMakeVisible(&osc2Label);

    setSize(400, 600);
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

    // Layout Oscillator 1 Controls
    osc1Label.setBounds(area.removeFromTop(20));
    osc1RangeComboBox.setBounds(area.removeFromTop(30));
    osc1WaveformComboBox.setBounds(area.removeFromTop(30));
    osc1TuningSlider.setBounds(area.removeFromTop(60));
    osc1PWMSlider.setBounds(area.removeFromTop(60));

    // Layout Oscillator 2 Controls
    osc2Label.setBounds(area.removeFromTop(20));
    osc2RangeComboBox.setBounds(area.removeFromTop(30));
    osc2WaveformComboBox.setBounds(area.removeFromTop(30));
    osc2TuningSlider.setBounds(area.removeFromTop(60));
    osc2PWMSlider.setBounds(area.removeFromTop(60));
}
