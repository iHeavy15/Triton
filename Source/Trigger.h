#pragma once
#include <JuceHeader.h>

class Trigger {
public:
    Trigger() {};
    ~Trigger() {};
    
    void midiToBuffer(AudioBuffer<float>& controlSignal, const MidiBuffer& midiMessages, const int numSamples)
    {
        
        // Buffer Mono
        auto data = controlSignal.getWritePointer(0);
        int timeStamp = 0;
        
        // per motivi di performance meglio tenere la condizione MENO probabile dentro all'IF
        if (!midiMessages.isEmpty())
        {
            MidiMessage m;
            int oldTimeStamp = 0;
            
            // Tutto si basa sul fatto che (in teoria) gli eventi sono nel buffer in ordine cronologico
            for (MidiBuffer::Iterator i(midiMessages); i.getNextEvent(m, timeStamp);)
                if (m.isNoteOn())
                {
                    FloatVectorOperations::fill(data + oldTimeStamp, value, timeStamp - oldTimeStamp);
                    oldTimeStamp = timeStamp;
                    noteCount++; //Se viene premuta una nota, incremento il contatore delle note premute
                    value = m.getFloatVelocity(); //Il valore che andrà nel buffer sarà la velocity (>0.0f)
                } else if (m.isNoteOff())
                {
                    noteCount--; //Se viene rilasciata una nota, decremento il contatore
                    value = (noteCount > 0) ? value : 0.0f; //Se non ci sono più note premute value va a 0.0f
                }
            
        }
        
        FloatVectorOperations::fill(data + timeStamp, value, numSamples - timeStamp);
    }
    
private:
    
    float value = 0.0f;
    int noteCount = 0;
    
};
