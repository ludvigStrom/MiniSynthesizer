#pragma once

#include <JuceHeader.h>
#include "ADSR.h"
#include "FormantFilter.h"
#include "SynthesizerEnums.h"
#include "SineWaveSound.h"

class OscillatorVoice : public juce::SynthesiserVoice
{
public:
    OscillatorVoice(std::atomic<float>* osc1TuningParam, std::atomic<float>* osc2TuningParam,
                    std::atomic<float>* osc1RangeParam, std::atomic<float>* osc2RangeParam,
                    std::atomic<float>* osc1WaveformParam, std::atomic<float>* osc2WaveformParam,
                    std::atomic<float>* osc1PWMParam, std::atomic<float>* osc2PWMParam,
                    std::atomic<float>* osc1AttackParam, std::atomic<float>* osc1DecayParam,
                    std::atomic<float>* osc1SustainParam, std::atomic<float>* osc1ReleaseParam,
                    std::atomic<float>* osc2AttackParam, std::atomic<float>* osc2DecayParam,
                    std::atomic<float>* osc2SustainParam, std::atomic<float>* osc2ReleaseParam,
                    std::atomic<float>* osc1VolumeParam, std::atomic<float>* osc2VolumeParam,
                    std::atomic<float>* formantFrequency1Param, std::atomic<float>* formantFrequency2Param,
                    std::atomic<float>* formantFrequency3Param);

    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);

    FormantFilter formantFilter;
    
    bool isVoiceActive() const override
    {
        return isNoteOn || adsr1.isActive() || adsr2.isActive();
    }

private:
    std::atomic<float>* osc1TuningParameter;
    std::atomic<float>* osc2TuningParameter;
    std::atomic<float>* osc1RangeParameter;
    std::atomic<float>* osc2RangeParameter;
    std::atomic<float>* osc1WaveformParameter;
    std::atomic<float>* osc2WaveformParameter;
    std::atomic<float>* osc1PWMParameter;
    std::atomic<float>* osc2PWMParameter;
    std::atomic<float>* osc1AttackParameter;
    std::atomic<float>* osc1DecayParameter;
    std::atomic<float>* osc1SustainParameter;
    std::atomic<float>* osc1ReleaseParameter;
    std::atomic<float>* osc2AttackParameter;
    std::atomic<float>* osc2DecayParameter;
    std::atomic<float>* osc2SustainParameter;
    std::atomic<float>* osc2ReleaseParameter;
    std::atomic<float>* osc1VolumeParameter;
    std::atomic<float>* osc2VolumeParameter;
    std::atomic<float>* formantFrequency1Parameter;
    std::atomic<float>* formantFrequency2Parameter;
    std::atomic<float>* formantFrequency3Parameter;

    juce::dsp::Oscillator<float> osc1;
    juce::dsp::Oscillator<float> osc2;

    ADSR adsr1;
    ADSR adsr2;

    int noteNumber = -1;
    float currentSampleRate = 44100.0f;
    bool isNoteOn = false;

    void updateFrequencies();
    double calculateFrequency(std::atomic<float>* tuningParam, std::atomic<float>* rangeParam);
    void setOscillatorWaveform(juce::dsp::Oscillator<float>& osc, int waveformType, std::atomic<float>* pwmParam);
};
