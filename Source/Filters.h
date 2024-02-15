#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

/* Classe per creare i filtri High e Low Pass */

class Filters
{
public:
    Filters(){}

    ~Filters() {}

    void prepareToPlay(double sr)
    {
        sampleRate = sr;

        reset();
       
        updateCoefficients();
    }

    void processBlock(AudioBuffer<float>& buffer, const int numSamples)
    {
        //MONO
        lpFilter.processSamples(buffer.getWritePointer(0), numSamples);
        hpFilter.processSamples(buffer.getWritePointer(0), numSamples);
    }

    void reset()
    {
        lpFilter.reset();
        hpFilter.reset();
    }

private:

   void updateCoefficients()
   {
       //Creo un filtro Low Pass
       const IIRCoefficients lpiirCoeffs = IIRCoefficients::makeLowPass(sampleRate, lpfrequency, quality);
       //Creo un filtro High Pass
       const IIRCoefficients hpiirCoeffs = IIRCoefficients::makeHighPass(sampleRate, hpfrequency, quality);
       
       lpFilter.setCoefficients(lpiirCoeffs);
       hpFilter.setCoefficients(hpiirCoeffs);
   }

    float lpfrequency = DEFAULT_LPF;
    float hpfrequency = DEFAULT_HPF;
    float quality = DEFAULT_Q;
    double sampleRate = DEFAULT_SR;
    
    /* Filtri IIR */
    IIRFilter lpFilter;
    IIRFilter hpFilter;
};
