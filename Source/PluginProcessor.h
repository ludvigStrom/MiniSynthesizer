#pragma once

#include <JuceHeader.h>
#include <cmath>
#include <stdexcept>
#include "FormantFilter.h"
#include "ADSR.h"

class MiniSynthesizerAudioProcessor : public juce::AudioProcessor
{
public:
    MiniSynthesizerAudioProcessor();
    ~MiniSynthesizerAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
   #endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState parameters;
    void applyBitcrusher(juce::AudioBuffer<float>& buffer, int numSamples);
    
    


    enum Waveform
    {
        Sine = 0,
        Saw,
        Pulse
    };

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
                        std::atomic<float>* formantFrequency1Param, std::atomic<float>* formantFrequency2Param, std::atomic<float>* formantFrequency3Param,
                        std::atomic<float>* formantQ1Param, std::atomic<float>* formantQ2Param, std::atomic<float>* formantQ3Param,
                        std::atomic<float>* formantGain1Param, std::atomic<float>* formantGain2Param, std::atomic<float>* formantGain3Param);
        
        ~OscillatorVoice() override = default;

        bool canPlaySound(juce::SynthesiserSound* sound) override;
        void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int) override;
        void stopNote(float velocity, bool allowTailOff) override;
        void pitchWheelMoved(int) override {}
        void controllerMoved(int, int) override {}
        void renderNextBlock(juce::AudioBuffer<float>&, int startSample, int numSamples) override;
        void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
        FormantFilter formantFilter;
        
        void updateADSRParameter(float attack1, float decay1, float sustain1, float release1,
                                     float attack2, float decay2, float sustain2, float release2);

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
        std::atomic<float>* bitcrusherEnabledParam = nullptr;
        std::atomic<float>* bitDepthParam = nullptr;
        std::atomic<float>* sampleRateReductionParam = nullptr;
        std::atomic<float>* formantFrequency1Parameter;
        std::atomic<float>* formantFrequency2Parameter;
        std::atomic<float>* formantFrequency3Parameter;
        std::atomic<float>* formantQ1Parameter;
        std::atomic<float>* formantQ2Parameter;
        std::atomic<float>* formantQ3Parameter;
        std::atomic<float>* formantGain1Parameter;
        std::atomic<float>* formantGain2Parameter;
        std::atomic<float>* formantGain3Parameter;

        juce::dsp::Oscillator<float> osc1;
        juce::dsp::Oscillator<float> osc2;
        
        ADSR osc1ADSR;
        ADSR osc2ADSR;
                
        void updateFrequencies();

        int noteNumber = -1;
        float currentSampleRate = 44100.0f;
        float level = 0.0f;
        float tailOff = 0.0f;

        bool isNoteOn = false;
        
        void setOscillatorWaveform(juce::dsp::Oscillator<float>& osc, int waveformType, std::atomic<float>* pwmParam);
        double calculateFrequency(std::atomic<float>* tuningParam, std::atomic<float>* rangeParam);
    };

private:
    class SineWaveSound : public juce::SynthesiserSound
    {
    public:
        SineWaveSound() {}
        bool appliesToNote (int) override { return true; }
        bool appliesToChannel (int) override { return true; }
    };

    juce::Synthesiser synth;
    std::atomic<float>* osc1TuningParam = nullptr;
    std::atomic<float>* osc2TuningParam = nullptr;
    std::atomic<float>* osc1RangeParam = nullptr;
    std::atomic<float>* osc2RangeParam = nullptr;
    std::atomic<float>* osc1WaveformParam = nullptr;
    std::atomic<float>* osc2WaveformParam = nullptr;
    std::atomic<float>* osc1PWMParam = nullptr;
    std::atomic<float>* osc2PWMParam = nullptr;
    std::atomic<float>* osc1AttackParam = nullptr;
    std::atomic<float>* osc1DecayParam = nullptr;
    std::atomic<float>* osc1SustainParam = nullptr;
    std::atomic<float>* osc1ReleaseParam = nullptr;
    std::atomic<float>* osc2AttackParam = nullptr;
    std::atomic<float>* osc2DecayParam = nullptr;
    std::atomic<float>* osc2SustainParam = nullptr;
    std::atomic<float>* osc2ReleaseParam = nullptr;
    std::atomic<float>* osc1VolumeParameter = nullptr;
    std::atomic<float>* osc2VolumeParameter = nullptr;
    std::atomic<float>* bitcrusherEnabledParam = nullptr;
    std::atomic<float>* bitDepthParam = nullptr;
    std::atomic<float>* sampleRateReductionParam = nullptr;
    std::atomic<float>* formantFrequency1Param = nullptr;
    std::atomic<float>* formantFrequency2Param = nullptr;
    std::atomic<float>* formantFrequency3Param = nullptr;
    std::atomic<float>* formantQ1Param = nullptr;
    std::atomic<float>* formantQ2Param = nullptr;
    std::atomic<float>* formantQ3Param = nullptr;
    std::atomic<float>* formantGain1Param = nullptr;
    std::atomic<float>* formantGain2Param = nullptr;
    std::atomic<float>* formantGain3Param = nullptr;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MiniSynthesizerAudioProcessor)
};
