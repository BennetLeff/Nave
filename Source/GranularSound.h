/*
  ==============================================================================

    GranularSound.h
    Created: 18 Dec 2019 5:04:57pm
    Author:  Bennet Leff
    
    Code structure heavily borrows from Juce's SamplerSound.
  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class GranularSound : public SynthesiserSound
{
public:

    GranularSound (const String& name,
                   AudioBuffer<float>& grainData,
                   const int grainSampleRate,
                   const BigInteger& midiNotes,
                   int midiNoteForNormalPitch,
                   double attackTimeSecs,
                   double releaseTimeSecs,
                   double maxSampleLengthSeconds);

    /** Destructor. */
    ~GranularSound() override;

    //==============================================================================
    /** Returns the sample's name */
    const String& getName() const noexcept                  { return name; }

    /** Returns the audio sample data.
        This could return nullptr if there was a problem loading the data.
    */
    AudioBuffer<float>* getAudioData() const noexcept       { return &grainData; }
    
    void setAudioData(AudioBuffer<float>& newData) { grainData = newData; }

    //==============================================================================
    /** Changes the parameters of the ADSR envelope which will be applied to the sample. */
    void setEnvelopeParameters (ADSR::Parameters parametersToUse)    { params = parametersToUse; }

    //==============================================================================
    bool appliesToNote (int midiNoteNumber) override;
    bool appliesToChannel (int midiChannel) override;

private:
    //==============================================================================
    friend class GranularVoice;

    String name;
    AudioBuffer<float>& grainData;
    double sourceSampleRate;
    BigInteger midiNotes;
    int length = 0, midiRootNote = 0;

    ADSR::Parameters params;

    JUCE_LEAK_DETECTOR (GranularSound)
};
