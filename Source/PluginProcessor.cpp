#include "PluginProcessor.h"
#include "PluginEditor.h"

MiniSynthesizerAudioProcessor::MiniSynthesizerAudioProcessor()
    : AudioProcessor (BusesProperties().withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      parameters (*this, nullptr, juce::Identifier ("MiniSynthesizer"),
                  {
                      std::make_unique<juce::AudioParameterFloat> ("osc1tuning", "Oscillator 1 Tuning", juce::NormalisableRange<float>(-12.0f, 12.0f), 0.0f),
                      std::make_unique<juce::AudioParameterFloat> ("osc2tuning", "Oscillator 2 Tuning", juce::NormalisableRange<float>(-12.0f, 12.0f), 0.0f),
                      std::make_unique<juce::AudioParameterChoice>("osc1range", "Oscillator 1 Range", juce::StringArray{"32'", "16'", "8'", "4'", "2'"}, 2),
                      std::make_unique<juce::AudioParameterChoice>("osc2range", "Oscillator 2 Range", juce::StringArray{"32'", "16'", "8'", "4'", "2'"}, 2),
                      std::make_unique<juce::AudioParameterChoice>("osc1waveform", "Oscillator 1 Waveform", juce::StringArray{"Sine", "Triangle", "Pulse"}, 0),
                      std::make_unique<juce::AudioParameterChoice>("osc2waveform", "Oscillator 2 Waveform", juce::StringArray{"Sine", "Triangle", "Pulse"}, 0),
                      std::make_unique<juce::AudioParameterFloat> ("osc1pwm", "Oscillator 1 PWM", 0.0f, 1.0f, 0.5f),
                      std::make_unique<juce::AudioParameterFloat> ("osc2pwm", "Oscillator 2 PWM", 0.0f, 1.0f, 0.5f)
                  })
{
    osc1TuningParam = parameters.getRawParameterValue ("osc1tuning");
    osc2TuningParam = parameters.getRawParameterValue ("osc2tuning");
    osc1RangeParam = parameters.getRawParameterValue ("osc1range");
    osc2RangeParam = parameters.getRawParameterValue ("osc2range");
    osc1WaveformParam = parameters.getRawParameterValue ("osc1waveform");
    osc2WaveformParam = parameters.getRawParameterValue ("osc2waveform");
    osc1PWMParam = parameters.getRawParameterValue ("osc1pwm");
    osc2PWMParam = parameters.getRawParameterValue ("osc2pwm");

    if (osc1TuningParam == nullptr || osc2TuningParam == nullptr || osc1RangeParam == nullptr || osc2RangeParam == nullptr || osc1WaveformParam == nullptr || osc2WaveformParam == nullptr || osc1PWMParam == nullptr || osc2PWMParam == nullptr)
    {
        jassertfalse; // This will trigger a breakpoint in debug mode
        DBG("Failed to get raw parameter values");
    }

    synth.clearVoices();
    synth.clearSounds();

    synth.addSound (new SineWaveSound());
    synth.addVoice (new SineWaveVoice (osc1TuningParam, osc2TuningParam, osc1RangeParam, osc2RangeParam, osc1WaveformParam, osc2WaveformParam, osc1PWMParam, osc2PWMParam));

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
        if (auto voice = dynamic_cast<SineWaveVoice*>(synth.getVoice(i)))
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

MiniSynthesizerAudioProcessor::SineWaveVoice::SineWaveVoice(std::atomic<float>* osc1TuningParam, std::atomic<float>* osc2TuningParam,
                                                            std::atomic<float>* osc1RangeParam, std::atomic<float>* osc2RangeParam,
                                                            std::atomic<float>* osc1WaveformParam, std::atomic<float>* osc2WaveformParam,
                                                            std::atomic<float>* osc1PWMParam, std::atomic<float>* osc2PWMParam)
    : osc1TuningParameter(osc1TuningParam), osc2TuningParameter(osc2TuningParam),
      osc1RangeParameter(osc1RangeParam), osc2RangeParameter(osc2RangeParam),
      osc1WaveformParameter(osc1WaveformParam), osc2WaveformParameter(osc2WaveformParam),
      osc1PWMParameter(osc1PWMParam), osc2PWMParameter(osc2PWMParam)
{
    if (osc1TuningParameter == nullptr || osc2TuningParameter == nullptr ||
        osc1RangeParameter == nullptr || osc2RangeParameter == nullptr ||
        osc1WaveformParameter == nullptr || osc2WaveformParameter == nullptr ||
        osc1PWMParameter == nullptr || osc2PWMParameter == nullptr)
    {
        jassertfalse; // This will trigger a breakpoint in debug mode
        DBG("SineWaveVoice constructor received null parameter");
    }
    DBG("SineWaveVoice constructor completed");
}


bool MiniSynthesizerAudioProcessor::SineWaveVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<SineWaveSound*>(sound) != nullptr;
}

void MiniSynthesizerAudioProcessor::SineWaveVoice::startNote(int midiNoteNumber, float velocity,
                                                             juce::SynthesiserSound*, int /*currentPitchWheelPosition*/)
{
    currentAngle1 = 0.0;
    currentAngle2 = 0.0;
    level = 0.0f;
    tailOff = 0.0f;
    isNoteOn = true;

    if (osc1TuningParameter == nullptr || osc2TuningParameter == nullptr)
    {
        jassertfalse; // This will trigger a breakpoint in debug mode
        DBG("tuningParameter is null in startNote");
        return;
    }

    double baseFrequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    double osc1Frequency = baseFrequency * std::pow(2.0, osc1TuningParameter->load() / 12.0);
    double osc2Frequency = baseFrequency * std::pow(2.0, osc2TuningParameter->load() / 12.0);

    double targetPitch = std::min(osc1Frequency, osc2Frequency);
    double carrierPitch = calculateCarrierPitch(targetPitch, osc1Frequency);

    osc1Frequency = carrierPitch; // Set the carrier frequency for oscillator 1

    double cyclesPerSample1 = osc1Frequency / currentSampleRate;
    double cyclesPerSample2 = osc2Frequency / currentSampleRate;
    angleDelta1 = cyclesPerSample1 * 2.0 * juce::MathConstants<double>::pi;
    angleDelta2 = cyclesPerSample2 * 2.0 * juce::MathConstants<double>::pi;

    DBG("Note started: " << midiNoteNumber << " with velocity " << velocity);
    DBG("Oscillator 1 Frequency: " << osc1Frequency);
    DBG("Oscillator 2 Frequency: " << osc2Frequency);
}

void MiniSynthesizerAudioProcessor::SineWaveVoice::stopNote(float velocity, bool allowTailOff)
{
    isNoteOn = false;

    if (allowTailOff)
    {
        tailOff = 1.0f; // Begin tail-off
    }
    else
    {
        clearCurrentNote();
        angleDelta1 = 0.0;
        angleDelta2 = 0.0;
    }
    DBG("Note stopped");
}

void MiniSynthesizerAudioProcessor::SineWaveVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
                                                                   int startSample, int numSamples)
{
    if (angleDelta1 != 0.0 || angleDelta2 != 0.0)
    {
        for (int sample = 0; sample < numSamples; ++sample)
        {
            float currentSample1 = (float)(std::sin(currentAngle1) * level);
            float currentSample2 = (float)(std::sin(currentAngle2) * level);

            // Apply ring modulation
            float currentSample = currentSample1 * currentSample2;

            for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
            {
                outputBuffer.addSample(channel, startSample, currentSample);
            }

            currentAngle1 += angleDelta1;
            currentAngle2 += angleDelta2;
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
                    angleDelta1 = 0.0;
                    angleDelta2 = 0.0;
                    break;
                }
            }
        }
    }
}

void MiniSynthesizerAudioProcessor::SineWaveVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    currentSampleRate = sampleRate;
    DBG("SineWaveVoice prepared with sample rate: " << sampleRate);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    DBG("Creating MiniSynthesizerAudioProcessor");
    return new MiniSynthesizerAudioProcessor();
}
