#include "PluginProcessor.h"
#include "PluginEditor.h"

MiniSynthesizerAudioProcessorEditor::MiniSynthesizerAudioProcessorEditor (MiniSynthesizerAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState (vts)
{
    // Oscillator 1 Tuning Slider
    osc1TuningSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    osc1TuningSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 50, 20);
    osc1TuningSlider.setPopupDisplayEnabled (true, false, this);
    osc1TuningSlider.setTextValueSuffix (" semitones");
    addAndMakeVisible (&osc1TuningSlider);

    osc1TuningAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (valueTreeState, "osc1tuning", osc1TuningSlider));

    osc1Label.setText ("Oscillator 1 Tuning", juce::dontSendNotification);
    osc1Label.attachToComponent (&osc1TuningSlider, false);
    addAndMakeVisible (&osc1Label);

    // Oscillator 2 Tuning Slider
    osc2TuningSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    osc2TuningSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 50, 20);
    osc2TuningSlider.setPopupDisplayEnabled (true, false, this);
    osc2TuningSlider.setTextValueSuffix (" semitones");
    addAndMakeVisible (&osc2TuningSlider);

    osc2TuningAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (valueTreeState, "osc2tuning", osc2TuningSlider));

    osc2Label.setText ("Oscillator 2 Tuning", juce::dontSendNotification);
    osc2Label.attachToComponent (&osc2TuningSlider, false);
    addAndMakeVisible (&osc2Label);
    
    // Oscillator 1 Volume Slider
    osc1VolumeSlider.setSliderStyle(juce::Slider::LinearVertical);
    osc1VolumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    osc1VolumeSlider.setSkewFactorFromMidPoint(0.5f); // Exponential response
    addAndMakeVisible(&osc1VolumeSlider);

    osc1VolumeAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "osc1volume", osc1VolumeSlider));

    // Oscillator 2 Volume Slider
    osc2VolumeSlider.setSliderStyle(juce::Slider::LinearVertical);
    osc2VolumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    osc2VolumeSlider.setSkewFactorFromMidPoint(0.5f); // Exponential response
    addAndMakeVisible(&osc2VolumeSlider);

    osc2VolumeAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "osc2volume", osc2VolumeSlider));

    // Oscillator 1 ADSR Sliders
    osc1AttackSlider.setSliderStyle(juce::Slider::LinearVertical);
    osc1AttackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(&osc1AttackSlider);
    osc1AttackAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "osc1attack", osc1AttackSlider));
    osc1AttackLabel.setText("Attack", juce::dontSendNotification);
    osc1AttackLabel.attachToComponent(&osc1AttackSlider, false);
    addAndMakeVisible(&osc1AttackLabel);

    osc1DecaySlider.setSliderStyle(juce::Slider::LinearVertical);
    osc1DecaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(&osc1DecaySlider);
    osc1DecayAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "osc1decay", osc1DecaySlider));
    osc1DecayLabel.setText("Decay", juce::dontSendNotification);
    osc1DecayLabel.attachToComponent(&osc1DecaySlider, false);
    addAndMakeVisible(&osc1DecayLabel);

    osc1SustainSlider.setSliderStyle(juce::Slider::LinearVertical);
    osc1SustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(&osc1SustainSlider);
    osc1SustainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "osc1sustain", osc1SustainSlider));
    osc1SustainLabel.setText("Sustain", juce::dontSendNotification);
    osc1SustainLabel.attachToComponent(&osc1SustainSlider, false);
    addAndMakeVisible(&osc1SustainLabel);

    osc1ReleaseSlider.setSliderStyle(juce::Slider::LinearVertical);
    osc1ReleaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(&osc1ReleaseSlider);
    osc1ReleaseAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "osc1release", osc1ReleaseSlider));
    osc1ReleaseLabel.setText("Release", juce::dontSendNotification);
    osc1ReleaseLabel.attachToComponent(&osc1ReleaseSlider, false);
    addAndMakeVisible(&osc1ReleaseLabel);

    // Oscillator 2 ADSR Sliders
    osc2AttackSlider.setSliderStyle(juce::Slider::LinearVertical);
    osc2AttackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(&osc2AttackSlider);
    osc2AttackAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "osc2attack", osc2AttackSlider));
    osc2AttackLabel.setText("Attack", juce::dontSendNotification);
    osc2AttackLabel.attachToComponent(&osc2AttackSlider, false);
    addAndMakeVisible(&osc2AttackLabel);

    osc2DecaySlider.setSliderStyle(juce::Slider::LinearVertical);
    osc2DecaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(&osc2DecaySlider);
    osc2DecayAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "osc2decay", osc2DecaySlider));
    osc2DecayLabel.setText("Decay", juce::dontSendNotification);
    osc2DecayLabel.attachToComponent(&osc2DecaySlider, false);
    addAndMakeVisible(&osc2DecayLabel);

    osc2SustainSlider.setSliderStyle(juce::Slider::LinearVertical);
    osc2SustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(&osc2SustainSlider);
    osc2SustainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "osc2sustain", osc2SustainSlider));
    osc2SustainLabel.setText("Sustain", juce::dontSendNotification);
    osc2SustainLabel.attachToComponent(&osc2SustainSlider, false);
    addAndMakeVisible(&osc2SustainLabel);

    osc2ReleaseSlider.setSliderStyle(juce::Slider::LinearVertical);
    osc2ReleaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(&osc2ReleaseSlider);
    osc2ReleaseAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "osc2release", osc2ReleaseSlider));
    osc2ReleaseLabel.setText("Release", juce::dontSendNotification);
    osc2ReleaseLabel.attachToComponent(&osc2ReleaseSlider, false);
    addAndMakeVisible(&osc2ReleaseLabel);

    // Oscillator 1 PWM, Range, and Waveform Sliders
    osc1PWMSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    osc1PWMSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 50, 20);
    osc1PWMSlider.setPopupDisplayEnabled (true, false, this);
    osc1PWMLabel.setText ("PWM", juce::dontSendNotification);
    osc1PWMLabel.attachToComponent(&osc1PWMSlider, false);
    addAndMakeVisible (&osc1PWMSlider);

    osc1PWMAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (valueTreeState, "osc1pwm", osc1PWMSlider));

    osc1RangeSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    osc1RangeSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 50, 20);
    osc1RangeSlider.setPopupDisplayEnabled (true, false, this);
    
    osc1RangeLabel.setText ("Range", juce::dontSendNotification);
    osc1RangeLabel.attachToComponent(&osc1RangeSlider, false);
    
    addAndMakeVisible (&osc1RangeSlider);

    osc1RangeAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (valueTreeState, "osc1range", osc1RangeSlider));

    osc1WaveformSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    osc1WaveformSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 50, 20);
    osc1WaveformSlider.setPopupDisplayEnabled (true, false, this);
    
    osc1WaveformLabel.setText ("Waveform", juce::dontSendNotification);
    osc1WaveformLabel.attachToComponent(&osc1WaveformSlider, false);
    
    addAndMakeVisible (&osc1WaveformSlider);

    osc1WaveformAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (valueTreeState, "osc1waveform", osc1WaveformSlider));

    // Oscillator 2 PWM, Range, and Waveform Sliders
    osc2PWMSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    osc2PWMSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 50, 20);
    osc2PWMSlider.setPopupDisplayEnabled (true, false, this);
    addAndMakeVisible (&osc2PWMSlider);

    osc2PWMAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (valueTreeState, "osc2pwm", osc2PWMSlider));

    osc2RangeSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    osc2RangeSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 50, 20);
    osc2RangeSlider.setPopupDisplayEnabled (true, false, this);
    addAndMakeVisible (&osc2RangeSlider);

    osc2RangeAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (valueTreeState, "osc2range", osc2RangeSlider));

    osc2WaveformSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    osc2WaveformSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 50, 20);
    osc2WaveformSlider.setPopupDisplayEnabled (true, false, this);
    addAndMakeVisible (&osc2WaveformSlider);

    osc2WaveformAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (valueTreeState, "osc2waveform", osc2WaveformSlider));
    
    // Bitcrusher Toggle Button
    bitcrusherToggle.setButtonText("Enable Bitcrusher");
    addAndMakeVisible(&bitcrusherToggle);
    bitcrusherToggleAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "bitcrusherEnabled", bitcrusherToggle));
    
    // Bit Depth Slider
    bitDepthSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    bitDepthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    bitDepthSlider.setRange(2, 16, 1);
    addAndMakeVisible(&bitDepthSlider);
    bitDepthAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "bitDepth", bitDepthSlider));

    // Sample Rate Reduction Slider
    sampleRateReductionSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    sampleRateReductionSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    sampleRateReductionSlider.setRange(200.0f, 20000.0f, 1.0f);
    sampleRateReductionSlider.setTextValueSuffix(" Hz");
    addAndMakeVisible(&sampleRateReductionSlider);
    sampleRateReductionAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "sampleRateReduction", sampleRateReductionSlider));

    setSize (1000, 400); // Adjust size as needed to fit all controls
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
    auto area = getLocalBounds().reduced(10);
    auto sliderWidth = area.getWidth() / 12; // Adjust as needed
    
    auto title = area.removeFromTop(area.getHeight() * 0.05);

    // Bitcrusher controlls
    auto bitcrusherArea = area.removeFromRight(250);
    auto bitcrusherSliderWidth = bitcrusherArea.getWidth() / 3; // Adjust as needed
    bitcrusherToggle.setBounds(bitcrusherArea.removeFromLeft(bitcrusherSliderWidth).reduced(10));
    bitDepthSlider.setBounds(bitcrusherArea.removeFromLeft(bitcrusherSliderWidth).reduced(10));
    sampleRateReductionSlider.setBounds(bitcrusherArea.removeFromLeft(bitcrusherSliderWidth).reduced(10));
    
    // Oscillator 1 Controls
    auto osc1Area = area.removeFromTop(area.getHeight() / 2);
    osc1TuningSlider.setBounds(osc1Area.removeFromLeft(sliderWidth).reduced(10));
    osc1AttackSlider.setBounds(osc1Area.removeFromLeft(sliderWidth).reduced(10));
    osc1DecaySlider.setBounds(osc1Area.removeFromLeft(sliderWidth).reduced(10));
    osc1SustainSlider.setBounds(osc1Area.removeFromLeft(sliderWidth).reduced(10));
    osc1ReleaseSlider.setBounds(osc1Area.removeFromLeft(sliderWidth).reduced(10));
    osc1PWMSlider.setBounds(osc1Area.removeFromLeft(sliderWidth).reduced(10));
    osc1RangeSlider.setBounds(osc1Area.removeFromLeft(sliderWidth).reduced(10));
    osc1WaveformSlider.setBounds(osc1Area.removeFromLeft(sliderWidth).reduced(10));
    osc1VolumeSlider.setBounds(osc1Area.removeFromLeft(sliderWidth).reduced(10));

    // Oscillator 2 Controls
    auto osc2Area = area;
    osc2TuningSlider.setBounds(osc2Area.removeFromLeft(sliderWidth).reduced(10));
    osc2AttackSlider.setBounds(osc2Area.removeFromLeft(sliderWidth).reduced(10));
    osc2DecaySlider.setBounds(osc2Area.removeFromLeft(sliderWidth).reduced(10));
    osc2SustainSlider.setBounds(osc2Area.removeFromLeft(sliderWidth).reduced(10));
    osc2ReleaseSlider.setBounds(osc2Area.removeFromLeft(sliderWidth).reduced(10));
    osc2PWMSlider.setBounds(osc2Area.removeFromLeft(sliderWidth).reduced(10));
    osc2RangeSlider.setBounds(osc2Area.removeFromLeft(sliderWidth).reduced(10));
    osc2WaveformSlider.setBounds(osc2Area.removeFromLeft(sliderWidth).reduced(10));
    osc2VolumeSlider.setBounds(osc2Area.removeFromLeft(sliderWidth).reduced(10));
}
