#pragma once

#include <JuceHeader.h>
#include <cmath>
#include <stdexcept>

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

    class SineWaveVoice : public juce::SynthesiserVoice
    {
    public:
        SineWaveVoice(std::atomic<float>* osc1TuningParam, std::atomic<float>* osc2TuningParam);
        ~SineWaveVoice() override = default;

        bool canPlaySound(juce::SynthesiserSound* sound) override;
        void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int) override;
        void stopNote(float velocity, bool allowTailOff) override;
        void pitchWheelMoved(int) override {}
        void controllerMoved(int, int) override {}
        void renderNextBlock(juce::AudioBuffer<float>&, int startSample, int numSamples) override;
        void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);

    private:
        std::atomic<float>* osc1TuningParameter;
        std::atomic<float>* osc2TuningParameter;
        double currentSampleRate = 44100.0;
        double currentAngle1 = 0.0;
        double currentAngle2 = 0.0;
        double angleDelta1 = 0.0;
        double angleDelta2 = 0.0;
        float level = 0.0f;
        float tailOff = 0.0f;

        // Envelope parameters
        float attackTime = 0.05f;  // Attack time in seconds
        float decayTime = 0.1f;    // Decay time in seconds
        float sustainLevel = 0.8f; // Sustain level (0.0 to 1.0)
        float releaseTime = 0.5f;  // Release time in seconds

        bool isNoteOn = false;

        // Function to calculate carrier pitch
        double calculateCarrierPitch(double targetPitch, double currentPitch)
        {
            if (currentPitch == 0)
            {
                throw std::invalid_argument("Current pitch cannot be zero");
            }
            return targetPitch / currentPitch;
        }
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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MiniSynthesizerAudioProcessor)
};
