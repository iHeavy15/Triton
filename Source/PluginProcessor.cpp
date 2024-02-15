#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TritonAudioProcessor::TritonAudioProcessor()
    : parameters(*this, nullptr, "TritonParameters", Parameters::createParameterLayout())
{
    //AGGIUNGO I LISTENER
    parameters.addParameterListener(VOLUME_NAME, this);
    parameters.addParameterListener(OSCFREQ_NAME, this);
    parameters.addParameterListener(OSCWAVE_NAME, this);
    parameters.addParameterListener(LFOFREQ_NAME, this);
    parameters.addParameterListener(LFOWAVE_NAME, this);
    parameters.addParameterListener(TYPE_NAME, this);
    
    //SET PARAMETERS
    Osc.setWaveform(DEFAULTWAVE);
    LFO1.setFrequency(SLOW_MOD);
    LFO1.setWaveform(DEFAULTWAVE);
    //Frequenza di Osc settata con l'adapter
    lfo1Adapter.setParameter(lowTone);
    //Setto l'ampiezza della modulante
    lfo1Adapter.setModAmount(midTone);
    LFO2.setFrequency(SLOW_MOD);
    LFO2.setWaveform(4);
    lfo2Adapter.setModAmount(SLOW_MOD);
    adsrparameters.attack = ATTACK;
    adsrparameters.release = RELEASE;
    adsr.setParameters(adsrparameters);
}

TritonAudioProcessor::~TritonAudioProcessor()
{
}

//==============================================================================

void TritonAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
    /* TRIGGER */
    triggerSignal.setSize(1, samplesPerBlock);
    
    /* ADSR */
    adsr.setSampleRate(sampleRate);
    
    /* LFO */
    LFO1.prepareToPlay(sampleRate);
    LFO2.prepareToPlay(sampleRate);
    LFO1Signal.setSize(1, samplesPerBlock);
    LFO2Signal.setSize(1, samplesPerBlock);
    lfo1Adapter.prepareToPlay(sampleRate);
    lfo2Adapter.prepareToPlay(sampleRate);
    
    /* OSC */
    Osc.prepareToPlay(sampleRate);
    
    /* FILTERS */
    filters.prepareToPlay(sampleRate);
    
    /* GAIN */
    smoothedGain.reset(sampleRate, SMOOTHING_TIME);
    smoothedGain.setTargetValue(gain);
    
}

void TritonAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void TritonAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    const auto numSamples = buffer.getNumSamples();
    
    /* TRIGGER */
    //Prendo MidiMessages e li metto in un buffer
    trigger.midiToBuffer(triggerSignal,midiMessages,numSamples);
    
    /* LFO */
    // Genero la prima modulante
    LFO1.getNextAudioBlock(LFO1Signal, numSamples);
    
    //Genero la seconda modulante
    LFO2.getNextAudioBlock(LFO2Signal, numSamples);
    
    //Scalo le modulanti
    lfo1Adapter.processBlock(LFO1Signal, numSamples);
    lfo2Adapter.processBlock(LFO2Signal, numSamples);
    
    /* OSC */
    //Genero l'oscillatore e lo modulo con i 2 LFO
    Osc.getNextAudioBlock(buffer, numSamples, LFO1Signal, LFO2Signal);
    
    auto bufferMono = buffer.getWritePointer(0);
    auto triggerSignalMono = triggerSignal.getWritePointer(0);

    //Controllo quando ci sono noteOn/noteOff e applico adsr (envelope)
    for (int smp = 0; smp < numSamples; ++smp)
    {
        //resetLFO serve a capire se attualemte sono già in noteOn o noteOff (altrimenti richiamerebbe sempre noteOn/noteOff e sarebbe sempre Active
        //Se c'è un valore > 0.0f (quindi noteOn)
        if (triggerSignalMono[smp] > 0.0f && resetLFO)
        {
            adsr.noteOn();
            resetLFO = false;
            
        }
        //Se il valore è 0.0f (quindi noteOff)
        else if (triggerSignalMono[smp] == 0.0f && !resetLFO)
        {
            adsr.noteOff();
            resetLFO = true;
            
        }
        //Resetto la fase solo quando finisce l'envelope
        if (!adsr.isActive())
        {
            //Resetto la fase degli LFO
            LFO1.reset();
            LFO2.reset();
            
        }
        //Applico envelope
        bufferMono[smp] *= adsr.getNextSample();
        
    }
    
    /* FILTERS */
    filters.processBlock(buffer, numSamples);
    
    /* GAIN */
    smoothedGain.applyGain(buffer, numSamples);
    
    //Seguo l'envelope (per il Meter)
    envelope.set( jmax( buffer.getMagnitude(0, buffer.getNumSamples()), envelope.get()) );
}

//==============================================================================

juce::AudioProcessorEditor* TritonAudioProcessor::createEditor()
{
    return new PluginEditor(*this, parameters);
}

//==============================================================================
void TritonAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void TritonAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(ValueTree::fromXml(*xmlState));
}

void TritonAudioProcessor::parameterChanged(const String& paramID, float newValue)
{
    if (paramID == VOLUME_NAME)
    {
        gain = juce::Decibels::decibelsToGain(newValue);
        smoothedGain.setTargetValue(gain);
    }
    
    if (paramID == OSCFREQ_NAME)
    {
        switch (static_cast<int>(newValue)) {
            case 0:
                lfo1Adapter.setParameter(lowTone);
                lfo1Adapter.setModAmount(midTone);
                break;
            case 1:
                lfo1Adapter.setParameter(midTone);
                lfo1Adapter.setModAmount(highTone);
                break;
            case 2:
                lfo1Adapter.setParameter(highTone);
                lfo1Adapter.setModAmount(highTone * 2.0f);
                break;
        }
    }
    
    if (paramID == OSCWAVE_NAME)
    {
        Osc.setWaveform(static_cast<int>(newValue));
    }
    
    if (paramID == LFOFREQ_NAME)
    {
        switch (static_cast<int>(newValue)) {
            case 0:
                LFO1.setFrequency(SLOW_MOD);
                LFO2.setFrequency(SLOW_MOD);
                break;
            case 1:
                LFO1.setFrequency(MID_MOD);
                LFO2.setFrequency(MID_MOD);
                break;
            case 2:
                LFO1.setFrequency(HIGH_MOD);
                LFO2.setFrequency(HIGH_MOD);
                break;
        }
    }
    
    if (paramID == LFOWAVE_NAME)
    {
        LFO1.setWaveform(static_cast<int>(newValue));
    }
    
    if (paramID == TYPE_NAME)
    {
        switch (static_cast<int>(newValue)) {
            case 0:
                LFO2.setWaveform(4);
                break;
            case 1:
                LFO2.setWaveform(3);
                break;
        }
    }

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TritonAudioProcessor();
}
