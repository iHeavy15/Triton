#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

class Adapter
{
public:
    Adapter() {}
    ~Adapter() {}

    void prepareToPlay(double sr)
    {
        parameter.reset(sr, SMOOTHING_TIME);
        modAmount.reset(sr, SMOOTHING_TIME);
    }

    void setParameter(float newValue)
    {
        parameter.setTargetValue(newValue);
    }

    void setModAmount(float newValue)
    {
        modAmount.setTargetValue(newValue);
    }

    void processBlock(AudioBuffer<float>& buffer, const int numSamples)
    {
        auto bufferMono = buffer.getWritePointer(0);

        /* Scalo la modulante tra 0 e 1 */
        //Aggiungo 1
        FloatVectorOperations::add(bufferMono, 1.0, numSamples);
        //Divido per 2
        FloatVectorOperations::multiply(bufferMono, 0.5f, numSamples);

        // Scalo la modulante tra 0 e la modulaziuone massima desiderata
        modAmount.applyGain(buffer, numSamples);

        // Sommo alla modulante il valore proveniente dal parametro
        if (parameter.isSmoothing())
        {
            for (int smp = 0; smp < numSamples; ++smp)
            {
                const auto param = parameter.getNextValue();
                bufferMono[smp] += param;
            }
        }
        else
        {
            FloatVectorOperations::add(bufferMono, parameter.getCurrentValue(), numSamples); //Se non sta cambiando il parametro uso il valore attuale
        }
        
    }

private:

    SmoothedValue<float, ValueSmoothingTypes::Linear> modAmount;
    SmoothedValue<float, ValueSmoothingTypes::Linear> parameter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Adapter)

};
