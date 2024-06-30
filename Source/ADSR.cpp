#include "ADSR.h"
#include <cmath> // Include for the exp function

ADSR::ADSR() : currentStage(Stage::Idle), envelope(0.0f), sampleRate(44100.0), attackRate(0.0f), decayRate(0.0f), sustainLevel(1.0f), releaseRate(0.0f)
{
}

float ADSR::getNextSample()
{
    switch (currentStage)
    {
        case Stage::Attack:
            envelope += attackRate * (1.0f - envelope);
            if (envelope >= 1.0f)
            {
                envelope = 1.0f;
                currentStage = Stage::Decay;
            }
            break;

        case Stage::Decay:
            envelope -= decayRate * (envelope - sustainLevel);
            if (envelope <= sustainLevel)
            {
                envelope = sustainLevel;
                currentStage = Stage::Sustain;
            }
            break;

        case Stage::Sustain:
            // Hold the sustain level
            envelope = sustainLevel;
            break;

        case Stage::Release:
            envelope -= releaseRate * envelope;
            if (envelope <= 0.0f)
            {
                envelope = 0.0f;
                currentStage = Stage::Idle;
            }
            break;

        default:
            break;
    }

    return envelope;
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
    attackRate = calculateRate(attack);
    decayRate = calculateRate(decay);
    sustainLevel = sustain;
    releaseRate = calculateRate(release);
}

float ADSR::calculateRate(float time)
{
    return (time > 0.0f) ? (1.0f - std::exp(-1.0f / (time * sampleRate))) : 1.0f;
}
