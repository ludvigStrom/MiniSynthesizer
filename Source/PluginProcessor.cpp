#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <juce_dsp/juce_dsp.h>

MiniSynthesizerAudioProcessor::MiniSynthesizerAudioProcessor()
    : AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, juce::Identifier("MiniSynthesizer"),
                 {
                     std::make_unique<juce::AudioParameterFloat>("osc1tuning", "Oscillator 1 Tuning", juce::NormalisableRange<float>(-12.0f, 12.0f), 0.0f),
                     std::make_unique<juce::AudioParameterFloat>("osc2tuning", "Oscillator 2 Tuning", juce::NormalisableRange<float>(-12.0f, 12.0f), 0.0f),
                     std::make_unique<juce::AudioParameterChoice>("osc1range", "Oscillator 1 Range", juce::StringArray{"32'", "16'", "8'", "4'", "2'"}, 2),
                     std::make_unique<juce::AudioParameterChoice>("osc2range", "Oscillator 2 Range", juce::StringArray{"32'", "16'", "8'", "4'", "2'"}, 2),
                     std::make_unique<juce::AudioParameterChoice>("osc1waveform", "Oscillator 1 Waveform", juce::StringArray{"Sine", "Triangle", "Pulse"}, 0),
                     std::make_unique<juce::AudioParameterChoice>("osc2waveform", "Oscillator 2 Waveform", juce::StringArray{"Sine", "Triangle", "Pulse"}, 0),
                     std::make_unique<juce::AudioParameterFloat>("osc1pwm", "Oscillator 1 PWM", 0.0f, 1.0f, 0.5f),
                     std::make_unique<juce::AudioParameterFloat>("osc2pwm", "Oscillator 2 PWM", 0.0f, 1.0f, 0.5f)
                 })
{
    osc1TuningParam = parameters.getRawParameterValue("osc1tuning");
    osc2TuningParam = parameters.getRawParameterValue("osc2tuning");
    osc1RangeParam = parameters.getRawParameterValue("osc1range");
    osc2RangeParam = parameters.getRawParameterValue("osc2range");
    osc1WaveformParam = parameters.getRawParameterValue("osc1waveform");
    osc2WaveformParam = parameters.getRawParameterValue("osc2waveform");
    osc1PWMParam = parameters.getRawParameterValue("osc1pwm");
    osc2PWMParam = parameters.getRawParameterValue("osc2pwm");

    if (osc1TuningParam == nullptr || osc2TuningParam == nullptr ||
        osc1RangeParam == nullptr || osc2RangeParam == nullptr ||
        osc1WaveformParam == nullptr || osc2WaveformParam == nullptr ||
        osc1PWMParam == nullptr || osc2PWMParam == nullptr)
    {
        jassertfalse; // This will trigger a breakpoint in debug mode
        DBG("Failed to get raw parameter values");
    }

    synth.clearVoices();
    synth.clearSounds();

    synth.addSound(new SineWaveSound());
    synth.addVoice(new OscillatorVoice(osc1TuningParam, osc2TuningParam, osc1RangeParam, osc2RangeParam, osc1WaveformParam, osc2WaveformParam, osc1PWMParam, osc2PWMParam));

    DBG("MiniSynthesizerAudioProcessor constructor completed");
}

MiniSynthesizerAudioProcessor::~MiniSynthesizerAudioProcessor()
{
    DBG("MiniSynthesizerAudioProcessor destructor called");
}

const juce::String MiniSynthesizerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MiniSynthesizerAudioProcessor::acceptsMidi() const
{
    return true;
}

bool MiniSynthesizerAudioProcessor::producesMidi() const
{
    return false;
}

bool MiniSynthesizerAudioProcessor::isMidiEffect() const
{
    return false;
}

double MiniSynthesizerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MiniSynthesizerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MiniSynthesizerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MiniSynthesizerAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String MiniSynthesizerAudioProcessor::getProgramName(int index)
{
    return {};
}

void MiniSynthesizerAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

void MiniSynthesizerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    DBG("Preparing to play with sample rate: " << sampleRate << " and block size: " << samplesPerBlock);
    synth.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<OscillatorVoice*>(synth.getVoice(i)))
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
    }
}

void MiniSynthesizerAudioProcessor::releaseResources()
{
    DBG("Releasing resources");
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MiniSynthesizerAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MiniSynthesizerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    DBG("Processing block: " << buffer.getNumSamples() << " samples");
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    DBG("Block processed");
}

bool MiniSynthesizerAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* MiniSynthesizerAudioProcessor::createEditor()
{
    return new MiniSynthesizerAudioProcessorEditor(*this, parameters);
}

void MiniSynthesizerAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    DBG("Saving state information");
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void MiniSynthesizerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    DBG("Loading state information");
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

MiniSynthesizerAudioProcessor::OscillatorVoice::OscillatorVoice(std::atomic<float>* osc1TuningParam, std::atomic<float>* osc2TuningParam,
                                                                std::atomic<float>* osc1RangeParam, std::atomic<float>* osc2RangeParam,
                                                                std::atomic<float>* osc1WaveformParam, std::atomic<float>* osc2WaveformParam,
                                                                std::atomic<float>* osc1PWMParam, std::atomic<float>* osc2PWMParam)
    : osc1TuningParameter(osc1TuningParam), osc2TuningParameter(osc2TuningParam),
      osc1RangeParameter(osc1RangeParam), osc2RangeParameter(osc2RangeParam),
      osc1WaveformParameter(osc1WaveformParam), osc2WaveformParameter(osc2WaveformParam),
      osc1PWMParameter(osc1PWMParam), osc2PWMParameter(osc2PWMParam),
      osc1([](float x) { return std::sin(x); }), osc2([](float x) { return std::sin(x); })
{
    if (osc1TuningParameter == nullptr || osc2TuningParameter == nullptr ||
        osc1RangeParameter == nullptr || osc2RangeParameter == nullptr ||
        osc1WaveformParameter == nullptr || osc2WaveformParameter == nullptr ||
        osc1PWMParameter == nullptr || osc2PWMParameter == nullptr)
    {
        jassertfalse; // This will trigger a breakpoint in debug mode
        DBG("OscillatorVoice constructor received null parameter");
    }
    DBG("OscillatorVoice constructor completed");
}

bool MiniSynthesizerAudioProcessor::OscillatorVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<SineWaveSound*>(sound) != nullptr;
}

void MiniSynthesizerAudioProcessor::OscillatorVoice::startNote(int midiNoteNumber, float velocity,
                                                               juce::SynthesiserSound*, int /*currentPitchWheelPosition*/)
{
    level = 0.0f;
    tailOff = 0.0f;
    isNoteOn = true;

    double baseFrequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);

    // Calculate the frequency with the range parameter
    auto calculateFrequency = [baseFrequency](std::atomic<float>* tuningParam, std::atomic<float>* rangeParam) {
        int range = static_cast<int>(rangeParam->load());
        double rangeMultiplier = std::pow(2.0, range - 2); // Adjust for ranges: 32, 16, 8, 4, 2
        return baseFrequency * rangeMultiplier * std::pow(2.0, tuningParam->load() / 12.0);
    };

    double osc1Frequency = calculateFrequency(osc1TuningParameter, osc1RangeParameter);
    double osc2Frequency = calculateFrequency(osc2TuningParameter, osc2RangeParameter);

    osc1.setFrequency(osc1Frequency);
    osc2.setFrequency(osc2Frequency);

    setOscillatorWaveform(osc1, static_cast<int>(osc1WaveformParameter->load()));
    setOscillatorWaveform(osc2, static_cast<int>(osc2WaveformParameter->load()));

    DBG("Note started: " << midiNoteNumber << " with velocity " << velocity);
    DBG("Oscillator 1 Frequency: " << osc1Frequency);
    DBG("Oscillator 2 Frequency: " << osc2Frequency);
}

void MiniSynthesizerAudioProcessor::OscillatorVoice::stopNote(float velocity, bool allowTailOff)
{
    if (allowTailOff)
    {
        if (tailOff == 0.0f)
            tailOff = 1.0f;
    }
    else
    {
        clearCurrentNote();
        osc1.reset();
        osc2.reset();
        level = 0.0f;
        tailOff = 0.0f;
    }
    isNoteOn = false;
    DBG("Note stopped");
}

void MiniSynthesizerAudioProcessor::OscillatorVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
                                                                     int startSample, int numSamples)
{
    if (isNoteOn || tailOff > 0.0f)
    {
        for (int sample = 0; sample < numSamples; ++sample)
        {
            float currentSample1 = osc1.processSample(0.0f);
            float currentSample2 = osc2.processSample(0.0f);

            // Mix the outputs of both oscillators
            float currentSample = (currentSample1 + currentSample2) * 0.5f;

            for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
            {
                outputBuffer.addSample(channel, startSample, currentSample * level);
            }

            ++startSample;

            if (isNoteOn)
            {
                // Attack phase
                if (level < 0.15f && tailOff == 0.0f)
                {
                    level += (0.15f / (attackTime * currentSampleRate));
                    if (level > 0.15f)
                        level = 0.15f;
                }
                // Sustain phase
                else if (level > 0.15f)
                {
                    level = 0.15f * sustainLevel;
                }
            }
            else if (tailOff > 0.0f)
            {
                // Release phase
                level -= (0.15f * sustainLevel / (releaseTime * currentSampleRate));
                if (level <= 0.0f)
                {
                    clearCurrentNote();
                    osc1.reset();
                    osc2.reset();
                    level = 0.0f;
                    tailOff = 0.0f;
                    break;
                }
            }
        }
    }
}

void MiniSynthesizerAudioProcessor::OscillatorVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    currentSampleRate = sampleRate;
    osc1.prepare({ sampleRate, static_cast<juce::uint32>(samplesPerBlock), static_cast<juce::uint32>(outputChannels) });
    osc2.prepare({ sampleRate, static_cast<juce::uint32>(samplesPerBlock), static_cast<juce::uint32>(outputChannels) });
    DBG("OscillatorVoice prepared with sample rate: " << sampleRate);
}

void MiniSynthesizerAudioProcessor::OscillatorVoice::setOscillatorWaveform(juce::dsp::Oscillator<float>& osc, int waveformType)
{
    switch (waveformType)
    {
        case Waveform::Sine:
            osc.initialise([](float x) { return std::sin(x); });
            break;
        case Waveform::Triangle:
            osc.initialise([](float x) { return x < juce::MathConstants<float>::pi ? -1.0f + (2.0f * x / juce::MathConstants<float>::pi) : 3.0f - (2.0f * x / juce::MathConstants<float>::pi); });
            break;
        case Waveform::Pulse:
            osc.initialise([this](float x) { return x < juce::MathConstants<float>::pi * osc1PWMParameter->load() ? 1.0f : -1.0f; });
            break;
        default:
            osc.initialise([](float x) { return std::sin(x); });
            break;
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    DBG("Creating MiniSynthesizerAudioProcessor");
    return new MiniSynthesizerAudioProcessor();
}
