#include "FormantFilter.h"

FormantFilter::FormantFilter()
    : currentSampleRate(44100.0),
      frequency1(500.0f), frequency2(1500.0f), frequency3(2500.0f),
      q1(1.0f), q2(1.0f), q3(1.0f),
      gain1(1.0f), gain2(1.0f), gain3(1.0f)
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

void FormantFilter::setFormantFrequencies(float f1, float f2, float f3)
{
    frequency1 = f1;
    frequency2 = f2;
    frequency3 = f3;
    reset();
    DBG("Formant frequencies set to: " << f1 << ", " << f2 << ", " << f3);
}

void FormantFilter::reset()
{
    filters.get<0>().coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(currentSampleRate, frequency1, q1, gain1);
    filters.get<1>().coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(currentSampleRate, frequency2, q2, gain2);
    filters.get<2>().coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(currentSampleRate, frequency3, q3, gain3);
}
