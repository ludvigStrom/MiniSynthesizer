#include "FormantFilter.h"

FormantFilter::FormantFilter()
    : currentSampleRate(44100.0),
      frequency1(500.0f), frequency2(1500.0f), frequency3(2500.0f),
      q1(10.0f), q2(10.0f), q3(10.0f),
      gain1(10.0f), gain2(10.0f), gain3(10.0f)
{
}

FormantFilter::~FormantFilter()
{
}

void FormantFilter::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    juce::dsp::ProcessSpec spec{ sampleRate, static_cast<juce::uint32>(samplesPerBlock), 2 };
    filters.prepare(spec);
    reset();
}

void FormantFilter::processBlock(juce::AudioBuffer<float>& buffer)
{
    if (buffer.getNumChannels() > 0 && buffer.getNumSamples() > 0)
    {
        juce::dsp::AudioBlock<float> block(buffer);
        filters.process(juce::dsp::ProcessContextReplacing<float>(block));
    }
}

void FormantFilter::setFormantParameters(float freq1, float freq2, float freq3,
                                         float q1, float q2, float q3,
                                         float gain1, float gain2, float gain3)
{
    this->frequency1 = freq1;
    this->frequency2 = freq2;
    this->frequency3 = freq3;
    this->q1 = q1;
    this->q2 = q2;
    this->q3 = q3;
    this->gain1 = gain1;
    this->gain2 = gain2;
    this->gain3 = gain3;

    reset();
}

void FormantFilter::reset()
{
    filters.get<0>().coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(currentSampleRate, frequency1, q1, gain1);
    filters.get<1>().coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(currentSampleRate, frequency2, q2, gain2);
    filters.get<2>().coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(currentSampleRate, frequency3, q3, gain3);
}
