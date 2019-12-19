/*
  ==============================================================================

    GranularAudioSource.h
    Created: 18 Dec 2019 2:53:49pm
    Author:  Bennet Leff
    
  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================


/*
 * The Attack, Sustain, and Release of a Grain are necessary parameters
 * as grains should have some crossfading in volume between each other.
 * This splits the grains total duration into three phases which
 * the volume of the grain should be modulated by.
 * For instance if the grain size is 10 ms, attack might be 2ms,
 * and release could be 1ms. Then sustain would be 7ms.
 *
 * For these values I'll test linear attack and release,
 * as well as an updated version with a gain multiplier
 * equivalent to either the Guassian curve or the Quasi-Guassian
 * which is described as the Tukey window in Roads' "The Computer
 * Music Tutorial"
 */
struct GrainASR
{
    int attack;
    int sustain;
    int release;
};

/*
 * The GranularAudioSource should be similar to the SamplerAudioSource.
 * It is no longer leverages the SamplerSound and SamplerVoice classes but
 * instead overrides them. This enables more advanced functionality such as
 * moving the start and stop positions around throughout the sound.
 * Additionally, this will allow us to buffer the files data (which is itself a
 * buffer) into grains.
 *
 * We can store grains in a table for more efficient playback or we can just
 * store the grains start and stop positions in a table for lookup. We'll start
 * with storing stop and start positions though caching buffers may become useful
 * later as the synth is expanded.
 *
 * For the time being, the method of Quasi-Synchronous Granular Synthesis, will be
 * what this class implements.
 */
class GranularAudioSource    : public AudioSource
{
public:
    GranularAudioSource(MidiKeyboardState& keyState);
        
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
    // Assuming a constant grain size for each grain.
    int grainSize;
    
    // Fields for MIDI interaction
    MidiKeyboardState& keyboardState;
    MidiMessageCollector midiCollector;
    Synthesiser synth;
    
    // allNotes will be a range of 0-127
    BigInteger allNotes;
    
    // The source file or sample we want to "granulate"
    File sourceFile;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GranularAudioSource)
};
