#pragma once
#include <cmath>

class ADSR
{
public:
    ADSR();

    void setSampleRate(double sampleRate);
    void setParameters(float attack, float decay, float sustain, float release);

    void noteOn();
    void noteOff();

    float getNextSample();
    bool isActive() const;
    
    void setAttack(float attack);
    void setDecay(float decay);
    void setSustain(float sustain);
    void setRelease(float release);

private:
    enum class Stage
    {
        Idle,
        Attack,
        Decay,
        Sustain,
        Release
    };

    Stage currentStage;
    float envelope;
    double sampleRate;
    float attackRate;
    float decayRate;
    float sustainLevel;
    float releaseRate;
    float attackShape;
    float decayShape;
    float releaseShape;

    float calculateRate(float timeInSeconds);
    float expCurve(float x, float shape);
    float bicubicCurve(float x);
};

inline bool ADSR::isActive() const
{
    return currentStage != Stage::Idle;
}
