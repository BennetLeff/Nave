/*
  ==============================================================================
    GranularAudioSource.h
    Created: 18 Dec 2019 2:53:49pm
    Author:  Bennet Leff
    
  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GranularSound.h"
#include "GranularVoice.h"

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
 * For the time being, the method of Quasi-Synchronous Granular Synthesis, will be
 * what this class implements.
 */
class GranularAudioSource    : public AudioSource
{
public:
    GranularAudioSource(MidiKeyboardState& keyState);
    
    // The JUCE API still uses raw pointers in some places
    // so we'll have to manually delete them.
    ~GranularAudioSource();
        
    void setUsingSamplerSound()
    {
        synth.clearSounds();
    }
    
    /*
     * sampleRate is in hertz or cycles per second. The grain size is
     * dependent on sampleRate. If the sampleRate is 44100 samples per second
     * then a 10 ms grain size corresponds to 441 samples.
     */
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        samplesPerGrain = sampleRate * (grainSize / 1000);
        synth.setCurrentPlaybackSampleRate (sampleRate);
        midiCollector.reset (sampleRate);
    }
    
    void releaseResources() override;
    
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    
    MidiMessageCollector* getMidiCollector()
    {
        return &midiCollector;
    }
        
    void setSourceFile(const File& newFile);
    
    // getter method for grains
    std::vector<AudioBuffer<float>*> getGrains() { return grains; }
    
private:
    // Parses a audio file's samples and creates partitioned grains
    // of size samplesPerGrain.
    std::vector<AudioBuffer<float>*> granulateSourceFile();
    
    // Assuming a constant grain size for each grain.
    // The unit for grainSize is miliseconds and the default is 10 ms.
    double grainSize;
    double samplesPerGrain;
    
    // The index of the grain in grains which we want to playback.
    int grainID;
    
    /*
     * grains is set with the value of granulateSourceFile.
     * It is a vector of sequential grains in a sample.
     * Each grain is captured by an AudioBuffer
     * for ease of use with the JUCE API.
     */
    std::vector<AudioBuffer<float>*> grains;
    
    // Fields for MIDI interaction
    MidiKeyboardState& keyboardState;
    MidiMessageCollector midiCollector;
    Synthesiser synth;
    
    // allNotes will be a range of 0-127
    BigInteger allNotes;
    
    double envelopeAttack;
    double envelopeRelease;
    
    // The source file or sample we want to "granulate"
    File sourceFile;
    // Must be a raw pointer because of the JUCE API.
    AudioFormatReader* reader;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GranularAudioSource)
};
