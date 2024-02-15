#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

class NaiveOscillator
{
public:
    NaiveOscillator() {};
    ~NaiveOscillator() {};
    
    void prepareToPlay(double sr)
    {
        samplePeriod = 1.0 / sr;
        frequency.reset(sr, GLIDE_TIME);
    }
    
    void setFrequency(float newValue)
    {
        frequency.setTargetValue(newValue);
    }
    
    void setWaveform(int newValue) {
        waveform = newValue;
    }; //Metodo per settare la forma d'onda
    
    void reset(){ //Reset della fase
        currentPhase = 0.0f;
    }
    
    //PER OSC
    void getNextAudioBlock(AudioBuffer<float>& buffer, const int numSamples, AudioBuffer<float>& lfo1Buffer, AudioBuffer<float> lfo2Buffer)
    {
        
        //Tutti i buffer Mono
        auto bufferMono = buffer.getWritePointer(0);
        auto lfo1BufferMono = lfo1Buffer.getWritePointer(0);
        auto lfo2BufferMono = lfo2Buffer.getWritePointer(0);
        
        for (int smp = 0; smp < numSamples; ++smp)
        {
            //A getNextAudioSample passo i valori dei buffer modulanti (LFO1 e LFO2)
            const auto sampleValue = getNextAudioSample(lfo1BufferMono[smp], lfo2BufferMono[smp]);
            bufferMono[smp] = sampleValue;
        }
        
    };
    
    float getNextAudioSample(float smpM, float smpV)
    {
        
        auto sampleValue = 0.0f;
        //Scelgo la forma d'onda
        switch (waveform) {
            case 0:
                //TRI WAVE
                sampleValue = 4 * abs(currentPhase - 0.5) - 1; //Calcolo il valore del campione
                break;
            case 1:
                //SQUARE WAVE
                sampleValue = (currentPhase > 0.5) - (currentPhase < 0.5);
                break;
            case 2:
                //SAW WAVE
                sampleValue = 2 * currentPhase - 1;
                break;
            default:
                break;
        }
        
        //Modulo con 2 valori degli LFO
        //smpM (dell'LFO1) modula la frequenza sommando il valore
        //smpV (dell'LFO2) modula la frqeunza come se fosse il volume (accende e spegne Osc)
        phaseIncrement = (frequency.getNextValue() + smpM) * smpV *  samplePeriod;
        
        currentPhase += phaseIncrement;
        currentPhase -= static_cast<int>(currentPhase);
        
        return sampleValue;
    };
    
    // PER LFO
    void getNextAudioBlock(AudioBuffer<float>& buffer, const int numSamples)
    {

        auto bufferMono = buffer.getWritePointer(0);

        for (int smp = 0; smp < numSamples; ++smp)
        {
            const auto sampleValue = getNextAudioSample();
            bufferMono[smp] = sampleValue;
        }
        
    };

    float getNextAudioSample()
    {
        
        auto sampleValue = 0.0f;
        //Scelgo la forma d'onda
        switch (waveform) {
            case 0:
                //MINUS SINE WAVE
                sampleValue = (-1) * std::sin(2.0f * juce::MathConstants<float>::pi * currentPhase);
                break;
            case 1:
                //SQUARE WAVE
                sampleValue = (currentPhase > 0.5) - (currentPhase < 0.5);
                break;
            case 2:
                //SAW WAVE
                sampleValue = 2 * currentPhase - 1; //Calcolo il valore del campione
                break;
            case 3:
                //SQUARE WAVE INVERSE
                //Inverto la fase rispetto al case 1 in modo tale da far sentire la parte "bassa" della modulazione e non quella "alta" di frequenza
                sampleValue = (currentPhase < 0.5) - (currentPhase > 0.5);
                break;
            case 4:
                //LINE
                //Per creare un segnale costante assegno direttamento 1
                sampleValue = 1.0f;
                break;
            default:
                break;
        }
        
        phaseIncrement = frequency.getNextValue() * samplePeriod;
        currentPhase += phaseIncrement;
        currentPhase -= static_cast<int>(currentPhase);
        
        return sampleValue;
    };
    
private:
    
    double samplePeriod = 1.0;
    float currentPhase = 0.0f;
    float phaseIncrement = 0.0f;
    SmoothedValue<float, ValueSmoothingTypes::Multiplicative> frequency;
    int waveform = 0;
    
};
