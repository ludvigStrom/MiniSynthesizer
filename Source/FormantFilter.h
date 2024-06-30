#pragma once

#include <JuceHeader.h>

class FormantFilter
{
public:
    FormantFilter();
    ~FormantFilter();

    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void processBlock(juce::AudioBuffer<float>& buffer);
    void setFormantParameters(float freq1, float freq2, float freq3,
                                  float q1, float q2, float q3,
                                  float gain1, float gain2, float gain3);
    void reset();
    
    

private:
    double currentSampleRate;
    juce::dsp::ProcessorChain<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Filter<float>> filters;
    float frequency1, frequency2, frequency3;
    float q1, q2, q3;
    float gain1, gain2, gain3;
};
