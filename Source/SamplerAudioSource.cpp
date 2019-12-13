/*
  ==============================================================================

    Sampler.cpp
    Created: 12 Dec 2019 3:18:03pm
    Author:  Bennet Leff

  ==============================================================================
*/

#include "SamplerAudioSource.h"

//==============================================================================
SamplerAudioSource::SamplerAudioSource(MidiKeyboardState& keyState, AudioFormatReader& audioFormatReader)
    : keyboardState(keyState)
{
    for(auto i = 0; i < 4; ++i)
        synth.addVoice(new SamplerVoice());
    
    BigInteger allNotes;
    allNotes.setRange(0, 128, true);
    
    samplerSound = std::make_unique<SamplerSound>("Sample", audioFormatReader, allNotes, 64, 0.05, 2.0, 4.0);
    
    synth.addSound(samplerSound.get());
}
