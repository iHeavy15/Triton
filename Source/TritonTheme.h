#pragma once
#include <JuceHeader.h>

#define BORDER_WIDTH 1.5f
#define SCALE_TO_KNOB_RATIO 0.85f

class TritonLookAndFeel : public LookAndFeel_V4
{
public:
    TritonLookAndFeel(){}

    void drawRotarySlider(Graphics& g,
        int x, int y, int width, int height,
        float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
        Slider& slider) override
    {
        // Useful variables
        const auto radius = jmin(width, height) * 0.5f * SCALE_TO_KNOB_RATIO - BORDER_WIDTH * 0.5f;
        const auto centreX = x + width * 0.5f;
        const auto centreY = y + height * 0.5f;
        const auto rx = centreX - radius;
        const auto ry = centreY - radius;
        const auto rw = radius * 2.0f;

        const Colour brightColour = Colour(79,133,148);
        const Colour darkColour = Colour(79,133,148);
        
        // Draw knob body
        g.setGradientFill(ColourGradient(brightColour, rx, ry, darkColour.brighter(0.05), rx, ry + rw, false));
        g.fillEllipse(rx, ry, rw, rw);

        // Draw outline
        g.setGradientFill(ColourGradient(brightColour.brighter(0.9), rx, ry, darkColour.darker(0.9), rx, ry + rw, false));
        g.drawEllipse(rx, ry, rw, rw, BORDER_WIDTH);
        
        // Prepare pointer shape
        Path p;
        const auto pointerLength = radius * 0.33f;
        const auto pointerThickness = 2.0f;
        p.addRectangle(pointerThickness * -0.5f, -radius, pointerThickness, pointerLength);

        // Draw pointer shape
        const auto angle = jmap(sliderPosProportional, rotaryStartAngle, rotaryEndAngle);
        p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));
        g.setGradientFill(ColourGradient(Colours::white, rx, ry, Colours::white.darker(0.2f), rx, ry + rw, false));
        g.fillPath(p);
        
        slider.setRotaryParameters(5.5f,7.0f,true);

    }

private:
};
