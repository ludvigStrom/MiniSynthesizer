#pragma once

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

    float calculateRate(float timeInSeconds);
};

inline bool ADSR::isActive() const
{
    return currentStage != Stage::Idle;
}
