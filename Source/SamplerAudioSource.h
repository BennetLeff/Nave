/*
  ==============================================================================

    Sampler.h
    Created: 12 Dec 2019 3:18:03pm
    Author:  Bennet Leff

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


class SamplerAudioSource : public AudioSource
{
public:
    SamplerAudioSource (MidiKeyboardState& keyState);
    
    void setUsingSamplerSound()
    {
        synth.clearSounds();
    }
    
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        synth.setCurrentPlaybackSampleRate (sampleRate);
        midiCollector.reset (sampleRate);
    }
    
    void releaseResources() override;
    
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override
    {
        bufferToFill.clearActiveBufferRegion();
        
        MidiBuffer incomingMidi;
        midiCollector.removeNextBlockOfMessages(incomingMidi, bufferToFill.numSamples);
        
        keyboardState.processNextMidiBuffer(incomingMidi, bufferToFill.startSample, bufferToFill.numSamples, true);
        
        synth.renderNextBlock(*bufferToFill.buffer, incomingMidi,
                              bufferToFill.startSample, bufferToFill.numSamples);
    }
    
    MidiMessageCollector* getMidiCollector()
    {
        return &midiCollector;
    }
        
    void setSourceFile(const File& newFile);
    
private:
    MidiKeyboardState& keyboardState;
    MidiMessageCollector midiCollector;
    Synthesiser synth;
    
    BigInteger allNotes;
    
    File sourceFile;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SamplerAudioSource)
};

