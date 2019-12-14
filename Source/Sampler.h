/*
  ==============================================================================

    Sampler.h
    Created: 13 Dec 2019 7:46:16pm
    Author:  Bennet Leff

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SamplerAudioSource.h"

//==============================================================================
/*
 * This class wraps the SamplerAudioSource class to add GUI functionality.
 * The SamplerAudioSource only handles Sample playback. This class draws
 * the AudioThumnail and overlays which allow a user to dictate where the
 * sample playback begins.
 * Any other GUI features that are directly related to the use of the Sampler
 * should probably be here as well.
 */
class Sampler    : public AudioSource, public Component
{
public:
    Sampler(MidiKeyboardState& keyboardState);
    ~Sampler();

    void paint (Graphics&) override;
    void resized() override;

    void setSourceFile(const File& newFile);
    
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        samplerAudioSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    }
    
    void releaseResources() override { } 
    
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override
    {
        samplerAudioSource.getNextAudioBlock(bufferToFill);
    }
    
    MidiMessageCollector* getMidiCollector()
    {
        return samplerAudioSource.getMidiCollector();
//        return &midiCollector;
    }
    
private:
    SamplerAudioSource samplerAudioSource;
    
//    MidiKeyboardState& keyboardState;
//    MidiMessageCollector midiCollector;
    
    // The synth which handles "playing" the sounds we load.
//    Synthesiser synth;
    
    // 0-127 MIDI notes in this case.
//    BigInteger allNotes;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sampler)
};
