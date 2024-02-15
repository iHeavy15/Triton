#pragma once
#include <JuceHeader.h>

//Useful
#define SMOOTHING_TIME 0.04
#define GLIDE_TIME 0.01

//Oscillator


//Filters
#define DEFAULT_LPF 8000.0f //Frequenza di taglio Low Pass
#define DEFAULT_HPF 200.0f //Frequenza di taglio High Pass
#define DEFAULT_Q 0.707f //Quality dei filtri
#define DEFAULT_SR 44100.0
#define NAME_Q "quality"

//Gain
#define LEVEL_DEFAULT  -12.0f

//Parameters
#define VOLUME_NAME  "Volume"
#define OSCFREQ_NAME "OscFreq"
#define OSCWAVE_NAME "OscWave"
#define LFOFREQ_NAME "LfoFreq"
#define LFOWAVE_NAME "LfoWave"
#define TYPE_NAME "SirenShot"
#define DEFAULTOSC_FREQ 0 //Frequenza di default di OSC [0,1,2]
#define DEFAULTLFO_FREQ 0 //Frequenza di default di LFO [0,1,2]
#define DEFAULTWAVE 0 //Waveform di default di Osc e LFO
#define LOW_TONE 52 //In Midi Note
#define MID_TONE 64 //In Midi Note
#define HIGH_TONE 76 //In Midi Note
#define SLOW_MOD 1.0f
#define MID_MOD 5.0f
#define HIGH_MOD 10.0f

//Adsr
#define ATTACK 0.005f
#define RELEASE 0.01f

namespace Parameters
{
    static AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
    {
        std::vector<std::unique_ptr<RangedAudioParameter>> params;
        
        params.push_back(std::make_unique<juce::AudioParameterFloat>(VOLUME_NAME, "Gain", -48.0f, 0.0f, LEVEL_DEFAULT));
        params.push_back(std::make_unique<juce::AudioParameterChoice>(OSCFREQ_NAME, "Osc Frequency", juce::StringArray {"Low", "Mid", "High"}, DEFAULTOSC_FREQ));
        params.push_back(std::make_unique<juce::AudioParameterChoice>(OSCWAVE_NAME, "Osc WaveForm", juce::StringArray {"Tri", "Square", "Saw"}, DEFAULTWAVE));
        params.push_back(std::make_unique<juce::AudioParameterChoice>(LFOFREQ_NAME, "LFO Frequency", juce::StringArray {"Slow", "Mid", "Fast"}, DEFAULTLFO_FREQ));
        params.push_back(std::make_unique<juce::AudioParameterChoice>(LFOWAVE_NAME, "LFO WaveForm", juce::StringArray {"Inverse Sin", "Square", "Saw"}, DEFAULTWAVE));
        params.push_back(std::make_unique<juce::AudioParameterChoice>(TYPE_NAME, "Siren / Shot", juce::StringArray {"Siren", "Shot"}, 0));

        return {params.begin(), params.end()};
    }
}
