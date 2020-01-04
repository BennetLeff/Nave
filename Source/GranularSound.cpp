/*
  ==============================================================================

    GranularSound.cpp
    Created: 18 Dec 2019 5:04:57pm
    Author:  Bennet Leff

  ==============================================================================
*/

#include "GranularSound.h"

GranularSound::GranularSound (const String& soundName,
                              AudioBuffer<float>& grainData,
                              const int grainSampleRate,
                              const BigInteger& notes,
                              int midiNoteForNormalPitch,
                              double attackTimeSecs,
                              double releaseTimeSecs,
                              double maxSampleLengthSeconds)
    : name (soundName),
      grainData(grainData),
      sourceSampleRate (grainSampleRate),
      midiNotes (notes),
      midiRootNote (midiNoteForNormalPitch)
{
    //if (sourceSampleRate > 0 && source.lengthInSamples > 0)
    if (sourceSampleRate > 0 && grainData.getNumSamples() > 0)
    {
        //length = jmin ((int) source.lengthInSamples,
        length = jmin((int) grainData.getNumSamples(),
                    (int) (maxSampleLengthSeconds * sourceSampleRate));

        // data.reset (new AudioBuffer<float> (jmin (2, (int) source.numChannels), length + 4));
        // data.reset (new AudioBuffer<float> (jmin (2, (int) grainData.getNumChannels()), length + 4));
            
        // source.read (data.get(), 0, length + 4, 0, true, true);

        params.attack  = static_cast<float> (attackTimeSecs);
        params.release = static_cast<float> (releaseTimeSecs);
    }
}

GranularSound::~GranularSound()
{
}

bool GranularSound::appliesToNote (int midiNoteNumber)
{
    return midiNotes[midiNoteNumber];
}

bool GranularSound::appliesToChannel (int /*midiChannel*/)
{
    return true;
}
