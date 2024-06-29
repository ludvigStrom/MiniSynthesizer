#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <juce_dsp/juce_dsp.h>
#include "FormantFilter.h" // Add this line

MiniSynthesizerAudioProcessor::MiniSynthesizerAudioProcessor()
    : AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, juce::Identifier("MiniSynthesizer"),
                 {
                     std::make_unique<juce::AudioParameterFloat>("osc1tuning", "Oscillator 1 Tuning", juce::NormalisableRange<float>(-12.0f, 12.0f), 0.0f),
                     std::make_unique<juce::AudioParameterFloat>("osc2tuning", "Oscillator 2 Tuning", juce::NormalisableRange<float>(-12.0f, 12.0f), 0.0f),
                     std::make_unique<juce::AudioParameterFloat> ("osc1volume", "Oscillator 1 Volume", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f, 0.5f), 0.5f),
                     std::make_unique<juce::AudioParameterFloat> ("osc2volume", "Oscillator 2 Volume", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f, 0.5f), 0.5f),
                     std::make_unique<juce::AudioParameterChoice>("osc1range", "Oscillator 1 Range", juce::StringArray{"32'", "16'", "8'", "4'", "2'"}, 2),
                     std::make_unique<juce::AudioParameterChoice>("osc2range", "Oscillator 2 Range", juce::StringArray{"32'", "16'", "8'", "4'", "2'"}, 2),
                     std::make_unique<juce::AudioParameterChoice>("osc1waveform", "Oscillator 1 Waveform", juce::StringArray{"Sine", "Saw", "Pulse"}, 0),
                     std::make_unique<juce::AudioParameterChoice>("osc2waveform", "Oscillator 2 Waveform", juce::StringArray{"Sine", "Saw", "Pulse"}, 0),
                     std::make_unique<juce::AudioParameterFloat>("osc1pwm", "Oscillator 1 PWM", 0.0f, 1.0f, 0.5f),
                     std::make_unique<juce::AudioParameterFloat>("osc2pwm", "Oscillator 2 PWM", 0.0f, 1.0f, 0.5f),
                     std::make_unique<juce::AudioParameterFloat>("osc1attack", "Oscillator 1 Attack", 0.0f, 5.0f, 0.1f),
                     std::make_unique<juce::AudioParameterFloat>("osc1decay", "Oscillator 1 Decay", 0.0f, 5.0f, 0.1f),
                     std::make_unique<juce::AudioParameterFloat>("osc1sustain", "Oscillator 1 Sustain", 0.0f, 1.0f, 0.8f),
                     std::make_unique<juce::AudioParameterFloat>("osc1release", "Oscillator 1 Release", 0.0f, 5.0f, 0.5f),
                     std::make_unique<juce::AudioParameterFloat>("osc2attack", "Oscillator 2 Attack", 0.0f, 5.0f, 0.1f),
                     std::make_unique<juce::AudioParameterFloat>("osc2decay", "Oscillator 2 Decay", 0.0f, 5.0f, 0.1f),
                     std::make_unique<juce::AudioParameterFloat>("osc2sustain", "Oscillator 2 Sustain", 0.0f, 1.0f, 0.8f),
                     std::make_unique<juce::AudioParameterFloat>("osc2release", "Oscillator 2 Release", 0.0f, 5.0f, 0.5f),
                     std::make_unique<juce::AudioParameterBool>("bitcrusherEnabled", "Enable Bitcrusher", false),
                     std::make_unique<juce::AudioParameterInt>("bitDepth", "Bit Depth", 2, 16, 16),
                     std::make_unique<juce::AudioParameterFloat>("sampleRateReduction", "Sample Rate Reduction", 200.0f, 20000.0f, 20000.0f),
                     std::make_unique<juce::AudioParameterFloat>("formantFrequency1", "Formant Frequency 1", juce::NormalisableRange<float>(200.0f, 2000.0f), 700.0f),
                     std::make_unique<juce::AudioParameterFloat>("formantFrequency2", "Formant Frequency 2", juce::NormalisableRange<float>(200.0f, 2000.0f), 1200.0f),
                     std::make_unique<juce::AudioParameterFloat>("formantFrequency3", "Formant Frequency 3", juce::NormalisableRange<float>(200.0f, 2000.0f), 2500.0f)
                 })
{
    osc1TuningParam = parameters.getRawParameterValue("osc1tuning");
    osc2TuningParam = parameters.getRawParameterValue("osc2tuning");
    osc1VolumeParameter = parameters.getRawParameterValue("osc1volume");
    osc2VolumeParameter = parameters.getRawParameterValue("osc2volume");
    osc1RangeParam = parameters.getRawParameterValue("osc1range");
    osc2RangeParam = parameters.getRawParameterValue("osc2range");
    osc1WaveformParam = parameters.getRawParameterValue("osc1waveform");
    osc2WaveformParam = parameters.getRawParameterValue("osc2waveform");
    osc1PWMParam = parameters.getRawParameterValue("osc1pwm");
    osc2PWMParam = parameters.getRawParameterValue("osc2pwm");
    osc1AttackParam = parameters.getRawParameterValue("osc1attack");
    osc1DecayParam = parameters.getRawParameterValue("osc1decay");
    osc1SustainParam = parameters.getRawParameterValue("osc1sustain");
    osc1ReleaseParam = parameters.getRawParameterValue("osc1release");
    osc2AttackParam = parameters.getRawParameterValue("osc2attack");
    osc2DecayParam = parameters.getRawParameterValue("osc2decay");
    osc2SustainParam = parameters.getRawParameterValue("osc2sustain");
    osc2ReleaseParam = parameters.getRawParameterValue("osc2release");
    bitcrusherEnabledParam = parameters.getRawParameterValue("bitcrusherEnabled");
    bitDepthParam = parameters.getRawParameterValue("bitDepth");
    sampleRateReductionParam = parameters.getRawParameterValue("sampleRateReduction");
    formantFrequency1Param = parameters.getRawParameterValue("formantFrequency1");
    formantFrequency2Param = parameters.getRawParameterValue("formantFrequency2");
    formantFrequency3Param = parameters.getRawParameterValue("formantFrequency3");

    if (osc1TuningParam == nullptr || osc2TuningParam == nullptr ||
        osc1RangeParam == nullptr || osc2RangeParam == nullptr ||
        osc1WaveformParam == nullptr || osc2WaveformParam == nullptr ||
        osc1PWMParam == nullptr || osc2PWMParam == nullptr ||
        osc1AttackParam == nullptr || osc1DecayParam == nullptr ||
        osc1SustainParam == nullptr || osc1ReleaseParam == nullptr ||
        osc2AttackParam == nullptr || osc2DecayParam == nullptr ||
        osc2SustainParam == nullptr || osc2ReleaseParam == nullptr)
    {
        jassertfalse; // This will trigger a breakpoint in debug mode
        // DBG("Failed to get raw parameter values");
    }

    synth.clearVoices();
    synth.clearSounds();

    synth.addSound(new SineWaveSound());
    
    for (int i = 0; i < 8; ++i)
        {
            synth.addVoice(new OscillatorVoice(osc1TuningParam, osc2TuningParam, osc1RangeParam, osc2RangeParam, osc1WaveformParam, osc2WaveformParam, osc1PWMParam, osc2PWMParam, osc1AttackParam, osc1DecayParam, osc1SustainParam, osc1ReleaseParam, osc2AttackParam, osc2DecayParam, osc2SustainParam, osc2ReleaseParam, osc1VolumeParameter, osc2VolumeParameter, formantFrequency1Param, formantFrequency2Param, formantFrequency3Param));
        }
    
    // synth.addVoice(new OscillatorVoice(osc1TuningParam, osc2TuningParam, osc1RangeParam, osc2RangeParam, osc1WaveformParam, osc2WaveformParam, osc1PWMParam, osc2PWMParam, osc1AttackParam, osc1DecayParam, osc1SustainParam, osc1ReleaseParam, osc2AttackParam, osc2DecayParam, osc2SustainParam, osc2ReleaseParam, osc1VolumeParameter, osc2VolumeParameter, formantFrequency1Param, formantFrequency2Param, formantFrequency3Param));


    // DBG("MiniSynthesizerAudioProcessor constructor completed");
}

MiniSynthesizerAudioProcessor::~MiniSynthesizerAudioProcessor()
{
    // DBG("MiniSynthesizerAudioProcessor destructor called");
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
    // DBG("Preparing to play with sample rate: " << sampleRate << " and block size: " << samplesPerBlock);
    synth.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<OscillatorVoice*>(synth.getVoice(i)))
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
    }
}

void MiniSynthesizerAudioProcessor::applyBitcrusher(juce::AudioBuffer<float>& buffer, int numSamples)
{
    if (*bitcrusherEnabledParam)
    {
        auto bitDepth = static_cast<int>(bitDepthParam->load());
        auto sampleRateReduction = static_cast<int>(sampleRateReductionParam->load());

        float bitScale = static_cast<float>(1 << (bitDepth - 1));
        int sampleRateStep = static_cast<int>(getSampleRate() / sampleRateReduction);

        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            float* channelData = buffer.getWritePointer(channel);
            float lastSample = 0.0f;

            for (int sample = 0; sample < numSamples; ++sample)
            {
                if (sampleRateStep > 0 && sample % sampleRateStep == 0)
                {
                    lastSample = std::round(channelData[sample] * bitScale) / bitScale;
                }
                channelData[sample] = lastSample;
            }
        }
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

    // Process the synth voices
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    // Apply bitcrusher
    applyBitcrusher(buffer, buffer.getNumSamples());

    // Apply formant filter
    if (buffer.getNumChannels() > 0 && buffer.getNumSamples() > 0)
    {
        // Helper function to clamp values
        auto clamp = [](float value, float min, float max) {
            return std::min(std::max(value, min), max);
        };

        float f1 = clamp(formantFrequency1Param->load(), 50.0f, 5000.0f);
        float f2 = clamp(formantFrequency2Param->load(), 50.0f, 5000.0f);
        float f3 = clamp(formantFrequency3Param->load(), 50.0f, 5000.0f);
        
        for (int i = 0; i < synth.getNumVoices(); ++i)
        {
            if (auto* voice = dynamic_cast<OscillatorVoice*>(synth.getVoice(i)))
            {
                voice->formantFilter.setFormantFrequencies(f1, f2, f3);
                voice->formantFilter.processBlock(buffer);
            }
        }
    }
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
    // DBG("Saving state information");
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void MiniSynthesizerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // DBG("Loading state information");
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

MiniSynthesizerAudioProcessor::OscillatorVoice::OscillatorVoice(
    std::atomic<float>* osc1TuningParam, std::atomic<float>* osc2TuningParam,
    std::atomic<float>* osc1RangeParam, std::atomic<float>* osc2RangeParam,
    std::atomic<float>* osc1WaveformParam, std::atomic<float>* osc2WaveformParam,
    std::atomic<float>* osc1PWMParam, std::atomic<float>* osc2PWMParam,
    std::atomic<float>* osc1AttackParam, std::atomic<float>* osc1DecayParam,
    std::atomic<float>* osc1SustainParam, std::atomic<float>* osc1ReleaseParam,
    std::atomic<float>* osc2AttackParam, std::atomic<float>* osc2DecayParam,
    std::atomic<float>* osc2SustainParam, std::atomic<float>* osc2ReleaseParam,
    std::atomic<float>* osc1VolumeParam, std::atomic<float>* osc2VolumeParam,
    std::atomic<float>* formantFrequency1Param, std::atomic<float>* formantFrequency2Param, std::atomic<float>* formantFrequency3Param)
    : osc1TuningParameter(osc1TuningParam), osc2TuningParameter(osc2TuningParam),
      osc1RangeParameter(osc1RangeParam), osc2RangeParameter(osc2RangeParam),
      osc1WaveformParameter(osc1WaveformParam), osc2WaveformParameter(osc2WaveformParam),
      osc1PWMParameter(osc1PWMParam), osc2PWMParameter(osc2PWMParam),
      osc1AttackParameter(osc1AttackParam), osc1DecayParameter(osc1DecayParam),
      osc1SustainParameter(osc1SustainParam), osc1ReleaseParameter(osc1ReleaseParam),
      osc2AttackParameter(osc2AttackParam), osc2DecayParameter(osc2DecayParam),
      osc2SustainParameter(osc2SustainParam), osc2ReleaseParameter(osc2ReleaseParam),
      osc1VolumeParameter(osc1VolumeParam), osc2VolumeParameter(osc2VolumeParam),
      formantFrequency1Parameter(formantFrequency1Param), formantFrequency2Parameter(formantFrequency2Param), formantFrequency3Parameter(formantFrequency3Param),
      osc1([](float x) { return std::sin(x); }), osc2([](float x) { return std::sin(x); })
{
    if (osc1TuningParameter == nullptr || osc2TuningParameter == nullptr ||
        osc1RangeParameter == nullptr || osc2RangeParameter == nullptr ||
        osc1WaveformParameter == nullptr || osc2WaveformParameter == nullptr ||
        osc1PWMParameter == nullptr || osc2PWMParameter == nullptr ||
        osc1AttackParameter == nullptr || osc1DecayParameter == nullptr ||
        osc1SustainParameter == nullptr || osc1ReleaseParameter == nullptr ||
        osc2AttackParameter == nullptr || osc2DecayParameter == nullptr ||
        osc2SustainParameter == nullptr || osc2ReleaseParameter == nullptr ||
        osc1VolumeParameter == nullptr || osc2VolumeParameter == nullptr ||
        formantFrequency1Parameter == nullptr || formantFrequency2Parameter == nullptr || formantFrequency3Parameter == nullptr)

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
    noteNumber = midiNoteNumber;
    isNoteOn = true;
    
    // Set ADSR parameters
    osc1ADSR.setParameters(osc1AttackParameter->load(), osc1DecayParameter->load(), osc1SustainParameter->load(), osc1ReleaseParameter->load());
    osc2ADSR.setParameters(osc2AttackParameter->load(), osc2DecayParameter->load(), osc2SustainParameter->load(), osc2ReleaseParameter->load());
    
    osc1ADSR.noteOn();
    osc2ADSR.noteOn();

    // Update the frequencies based on the new MIDI note number
    updateFrequencies();

    setOscillatorWaveform(osc1, static_cast<int>(osc1WaveformParameter->load()), osc1PWMParameter);
    setOscillatorWaveform(osc2, static_cast<int>(osc2WaveformParameter->load()), osc2PWMParameter);

    DBG("Note started: " << midiNoteNumber << " with velocity " << velocity);
    DBG("Oscillator 1 Frequency: " << osc1.getFrequency());
    DBG("Oscillator 2 Frequency: " << osc2.getFrequency());
}

void MiniSynthesizerAudioProcessor::OscillatorVoice::updateFrequencies()
{
    double osc1Frequency = calculateFrequency(osc1TuningParameter, osc1RangeParameter);
    double osc2Frequency = calculateFrequency(osc2TuningParameter, osc2RangeParameter);

    osc1.setFrequency(osc1Frequency);
    osc2.setFrequency(osc2Frequency);
}

void MiniSynthesizerAudioProcessor::OscillatorVoice::stopNote(float velocity, bool allowTailOff)
{
    isNoteOn = false;
    if (!allowTailOff)
    {
        clearCurrentNote();
    }
    else
    {
        osc1ADSR.noteOff();
        osc2ADSR.noteOff();
    }
}

void MiniSynthesizerAudioProcessor::OscillatorVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if (isNoteOn || osc1ADSR.isActive() || osc2ADSR.isActive())
    {
        float osc1Volume = juce::jlimit(0.0f, 1.0f, osc1VolumeParameter ? osc1VolumeParameter->load() : 0.5f);
        float osc2Volume = juce::jlimit(0.0f, 1.0f, osc2VolumeParameter ? osc2VolumeParameter->load() : 0.5f);

        // Update frequencies if needed (e.g., for real-time modulation)
        updateFrequencies();

        setOscillatorWaveform(osc1, static_cast<int>(osc1WaveformParameter->load()), osc1PWMParameter);
        setOscillatorWaveform(osc2, static_cast<int>(osc2WaveformParameter->load()), osc2PWMParameter);

        juce::AudioBuffer<float> tempBuffer(outputBuffer.getNumChannels(), numSamples);
        tempBuffer.clear();

        for (int sample = 0; sample < numSamples; ++sample)
        {
            float currentSample1 = osc1.processSample(0.0f) * osc1Volume * osc1ADSR.getNextSample();
            float currentSample2 = osc2.processSample(0.0f) * osc2Volume * osc2ADSR.getNextSample();

            float currentSample = (currentSample1 + currentSample2) * 0.5f;
            currentSample = juce::jlimit(-1.0f, 1.0f, currentSample);

            for (int channel = 0; channel < tempBuffer.getNumChannels(); ++channel)
            {
                tempBuffer.addSample(channel, sample, currentSample);
            }

            if (!isNoteOn && !osc1ADSR.isActive() && !osc2ADSR.isActive())
            {
                break;
            }
        }

        formantFilter.setFormantFrequencies(formantFrequency1Parameter->load(),
                                            formantFrequency2Parameter->load(),
                                            formantFrequency3Parameter->load());
        formantFilter.processBlock(tempBuffer);

        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            outputBuffer.addFrom(channel, startSample, tempBuffer, channel, 0, numSamples);
        }
    }
}

double MiniSynthesizerAudioProcessor::OscillatorVoice::calculateFrequency(std::atomic<float>* tuningParam, std::atomic<float>* rangeParam)
{
    double baseFrequency = juce::MidiMessage::getMidiNoteInHertz(getCurrentlyPlayingNote());
    int range = static_cast<int>(rangeParam->load());
    double rangeMultiplier = std::pow(2.0, range - 2); // Adjust for ranges: 32, 16, 8, 4, 2
    return baseFrequency * rangeMultiplier * std::pow(2.0, tuningParam->load() / 12.0);
}

void MiniSynthesizerAudioProcessor::OscillatorVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    currentSampleRate = sampleRate;
    osc1.prepare({ sampleRate, static_cast<juce::uint32>(samplesPerBlock), static_cast<juce::uint32>(outputChannels) });
    osc2.prepare({ sampleRate, static_cast<juce::uint32>(samplesPerBlock), static_cast<juce::uint32>(outputChannels) });
    formantFilter.prepareToPlay(sampleRate, samplesPerBlock);
}

void MiniSynthesizerAudioProcessor::OscillatorVoice::setOscillatorWaveform(juce::dsp::Oscillator<float>& osc, int waveformType, std::atomic<float>* pwmParam = nullptr)
{
    switch (waveformType)
    {
        case Waveform::Sine:
            osc.initialise([](float x) { return std::sin(x); });
            break;
        case Waveform::Saw:
            osc.initialise([](float x) { return x < juce::MathConstants<float>::pi ? -1.0f + (2.0f * x / juce::MathConstants<float>::pi) : 3.0f - (2.0f * x / juce::MathConstants<float>::pi); });
            break;
        case Waveform::Pulse:
            osc.initialise([pwmParam](float x) {
                return pwmParam ? (x < juce::MathConstants<float>::pi * pwmParam->load() ? 1.0f : -1.0f) : (x < juce::MathConstants<float>::pi ? 1.0f : -1.0f);
            });
            break;
        default:
            osc.initialise([](float x) { return std::sin(x); });
            break;
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    // DBG("Creating MiniSynthesizerAudioProcessor");
    return new MiniSynthesizerAudioProcessor();
}
