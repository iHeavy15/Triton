#pragma once

#include <JuceHeader.h>
#include "Oscillator.h"
#include "Adapter.h"
#include "Trigger.h"
#include "Filters.h"
#include "Parameters.h"

//=============================================================================

class TritonAudioProcessor  : public juce::AudioProcessor, public AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    TritonAudioProcessor();
    ~TritonAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    //==============================================================================
    const juce::String getName() const override { return JucePlugin_Name; }

    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return true; }
    double getTailLengthSeconds() const override { return 0.0; }

    //==============================================================================
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int index) override {}
    const juce::String getProgramName(int index) override { return {}; }
    void changeProgramName(int index, const juce::String& newName) override {}

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    Atomic<float> envelope;
    
    //Useful
    const float lowTone = static_cast<float>(MidiMessage::getMidiNoteInHertz(LOW_TONE));
    const float midTone = static_cast<float>(MidiMessage::getMidiNoteInHertz(MID_TONE));
    const float highTone = static_cast<float>(MidiMessage::getMidiNoteInHertz(HIGH_TONE));
    
private:
    
    /* GAIN */
    float gain = Decibels::decibelsToGain(LEVEL_DEFAULT);
    SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedGain;
    
    /* PARAMETER */
    void parameterChanged(const juce::String& paramID, float newValue) override; //Metodo viene chiamato quando cambiano i parametri
    AudioProcessorValueTreeState parameters; //parametri
    
    /* TRIGGER */
    Trigger trigger;
    
    /* LFO */
    NaiveOscillator LFO1;
    NaiveOscillator LFO2;
    
    /* OSC */
    NaiveOscillator Osc;
    
    /* ADAPTER */
    Adapter lfo1Adapter;
    Adapter lfo2Adapter;
    
    /* ADSR */
    ADSR adsr;
    ADSR::Parameters adsrparameters;
    bool resetLFO = true;
    
    /* Filters */
    Filters filters;

    /* BUFFER */
    AudioBuffer<float> LFO1Signal;
    AudioBuffer<float> LFO2Signal;
    AudioBuffer<float> triggerSignal;
    
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TritonAudioProcessor)
};
