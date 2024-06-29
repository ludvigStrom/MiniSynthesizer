#include "OscillatorVoice.h"
#include "SynthesizerEnums.h"
#include "SineWaveSound.h"

OscillatorVoice::OscillatorVoice(
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
    osc1.setFrequency(440.0);  // Initialize to A4
    osc2.setFrequency(440.0);  // Initialize to A4

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

bool OscillatorVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<SineWaveSound*>(sound) != nullptr;
}

void OscillatorVoice::pitchWheelMoved(int newPitchWheelValue)
{
    // Implement pitch wheel behavior if needed
}

void OscillatorVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
    // Implement controller behavior if needed
}

void OscillatorVoice::startNote(int midiNoteNumber, float velocity,
                                juce::SynthesiserSound*, int /*currentPitchWheelPosition*/)
{
    noteNumber = midiNoteNumber;
    isNoteOn = true;

    // Update the frequencies based on the new MIDI note number
    updateFrequencies();

    setOscillatorWaveform(osc1, static_cast<int>(osc1WaveformParameter->load()), osc1PWMParameter);
    setOscillatorWaveform(osc2, static_cast<int>(osc2WaveformParameter->load()), osc2PWMParameter);

    adsr1.setParameters(osc1AttackParameter->load(), osc1DecayParameter->load(), osc1SustainParameter->load(), osc1ReleaseParameter->load());
    adsr2.setParameters(osc2AttackParameter->load(), osc2DecayParameter->load(), osc2SustainParameter->load(), osc2ReleaseParameter->load());

    adsr1.noteOn();
    adsr2.noteOn();

    DBG("Note started: " << midiNoteNumber << " with velocity " << velocity);
    DBG("Oscillator 1 Frequency: " << osc1.getFrequency());
    DBG("Oscillator 2 Frequency: " << osc2.getFrequency());
}

void OscillatorVoice::updateFrequencies()
{
    double osc1Frequency = calculateFrequency(osc1TuningParameter, osc1RangeParameter);
    double osc2Frequency = calculateFrequency(osc2TuningParameter, osc2RangeParameter);

    osc1.setFrequency(osc1Frequency);
    osc2.setFrequency(osc2Frequency);
}

void OscillatorVoice::stopNote(float velocity, bool allowTailOff)
{
    isNoteOn = false;
    if (!allowTailOff)
    {
        clearCurrentNote();
        adsr1.noteOff();
        adsr2.noteOff();
    }
}

void OscillatorVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if (isNoteOn || (!isNoteOn && adsr1.getNextSample() > 0.0f))
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
            float currentSample1 = juce::jlimit(-1.0f, 1.0f, osc1.processSample(0.0f)) * osc1Volume;
            float currentSample2 = juce::jlimit(-1.0f, 1.0f, osc2.processSample(0.0f)) * osc2Volume;

            float adsr1Value = adsr1.getNextSample();
            float adsr2Value = adsr2.getNextSample();

            currentSample1 *= adsr1Value;
            currentSample2 *= adsr2Value;

            float currentSample = (currentSample1 + currentSample2) * 0.5f;
            currentSample = juce::jlimit(-1.0f, 1.0f, currentSample);

            for (int channel = 0; channel < tempBuffer.getNumChannels(); ++channel)
            {
                tempBuffer.addSample(channel, sample, currentSample);
            }

            if (!isNoteOn && adsr1Value == 0.0f)
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

double OscillatorVoice::calculateFrequency(std::atomic<float>* tuningParam, std::atomic<float>* rangeParam)
{
    double baseFrequency = juce::MidiMessage::getMidiNoteInHertz(noteNumber);
    int range = static_cast<int>(rangeParam->load());
    double rangeMultiplier = std::pow(2.0, range - 2);
    double tuning = tuningParam->load();

    DBG("Base Frequency: " << baseFrequency << " Hz");
    DBG("Range: " << range << " Range Multiplier: " << rangeMultiplier);
    DBG("Tuning: " << tuning << " semitones");

    double frequency = baseFrequency * rangeMultiplier * std::pow(2.0, tuning / 12.0);
    
    // Limit the frequency to a reasonable range
    frequency = juce::jlimit(20.0, 20000.0, frequency);

    DBG("Calculated Frequency: " << frequency << " Hz");

    return frequency;
}

void OscillatorVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    currentSampleRate = sampleRate;
    juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32> (samplesPerBlock), static_cast<juce::uint32> (outputChannels) };
    osc1.prepare(spec);
    osc2.prepare(spec);
    formantFilter.prepareToPlay(sampleRate, samplesPerBlock);

    // Set sample rate for ADSRs
    adsr1.setSampleRate(sampleRate);
    adsr2.setSampleRate(sampleRate);

    // Reset oscillators
    osc1.reset();
    osc2.reset();

    DBG("Prepared to play with sample rate: " << sampleRate);
}

void OscillatorVoice::setOscillatorWaveform(juce::dsp::Oscillator<float>& osc, int waveformType, std::atomic<float>* pwmParam)
{
    switch (static_cast<Waveform>(waveformType))
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
