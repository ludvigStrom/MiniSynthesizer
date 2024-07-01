#include "ADSR.h"

ADSR::ADSR()
    : currentStage(Stage::Idle), envelope(0.0f), sampleRate(44100.0),
      attackRate(0.0f), decayRate(0.0f), sustainLevel(1.0f), releaseRate(0.0f),
      attackShape(0.3f), decayShape(0.3f), releaseShape(0.3f)
{
}

float ADSR::getNextSample()
{
    switch (currentStage)
    {
        case Stage::Attack:
            envelope += attackRate;
            if (envelope >= 1.0f)
            {
                envelope = 1.0f;
                currentStage = Stage::Decay;
            }
            return bicubicCurve(envelope);

        case Stage::Decay:
            envelope -= decayRate;
            if (envelope <= sustainLevel)
            {
                envelope = sustainLevel;
                currentStage = Stage::Sustain;
            }
            return expCurve(envelope, decayShape);

        case Stage::Sustain:
            return sustainLevel;

        case Stage::Release:
            envelope -= releaseRate;
            if (envelope <= 0.0f)
            {
                envelope = 0.0f;
                currentStage = Stage::Idle;
            }
            return expCurve(envelope, releaseShape);

        default:
            return 0.0f;
    }
}

void ADSR::setAttack(float attack)
{
    attackRate = 1.0f / (attack * sampleRate);
}

void ADSR::setDecay(float decay)
{
    decayRate = 1.0f / (decay * sampleRate);
}

void ADSR::setSustain(float sustain)
{
    sustainLevel = sustain;
}

void ADSR::setRelease(float release)
{
    releaseRate = 1.0f / (release * sampleRate);
}

void ADSR::noteOn()
{
    currentStage = Stage::Attack;
    envelope = 0.0f;
}

void ADSR::noteOff()
{
    currentStage = Stage::Release;
}

void ADSR::setSampleRate(double newSampleRate)
{
    sampleRate = newSampleRate;
}

void ADSR::setParameters(float attack, float decay, float sustain, float release)
{
    setAttack(attack);
    setDecay(decay);
    setSustain(sustain);
    setRelease(release);
}

float ADSR::calculateRate(float timeInSeconds)
{
    return 1.0f / (timeInSeconds * sampleRate);
}

float ADSR::expCurve(float x, float shape)
{
    return (1.0f - std::exp(-shape * x)) / (1.0f - std::exp(-shape));
}

float ADSR::bicubicCurve(float x)
{
    if (x < 0.5f)
        return 4.0f * x * x * x;
    else
        return 1.0f - std::pow(-2.0f * x + 2.0f, 3.0f) / 2.0f;
}
