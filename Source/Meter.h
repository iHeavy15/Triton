#pragma once
#include <JuceHeader.h>

#define FPS       25
#define DB_FLOOR -48.0f
#define RT         0.5f

class Meter : public Component, public Timer
{
public:
    Meter()
    {
        alpha = exp(-1.0f / (FPS * RT));
        startTimerHz(FPS);
    }

    ~Meter()
    {
        stopTimer();
    }

    void paint(Graphics& g) override
    {
        auto W = getWidth();
        auto H = getHeight();
        const Colour green = Colour(0,255,68);
        const Colour yellow = Colour(255,247,0);
        const Colour red = Colour(255,0,0);

        g.drawEllipse(0, 0, W, H, 1);
        g.fillAll(Colours::black);
        g.setColour(Colours::grey);
        

        if (observedEnvelope != nullptr)
        {
            
            auto envelopeSnapshot = observedEnvelope->get();
            observedEnvelope->set(envelopeSnapshot * alpha);

            auto peak = Decibels::gainToDecibels(envelopeSnapshot);

            if (peak >= 0.0f) {
                g.fillAll(red);
            }
            if (peak < 0.0f && peak >= -6.0f){
                g.fillAll(yellow);
            }
            if (peak < -6.0f && peak >= -48.0f){
                g.fillAll(green);
            }
            if (peak < -48.0f){
                g.fillAll(Colours::black);
            }
            
        }

    }

    void resized() override {}

    void connectTo(Atomic<float>& targetVariable)
    {
        observedEnvelope = &targetVariable;
    }

private:

    void timerCallback() override
    {
        repaint();
    }

    float alpha = 0.0f;

    Atomic<float>* observedEnvelope = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Meter);
};
