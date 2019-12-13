/*
  ==============================================================================

    Sampler.cpp
    Created: 12 Dec 2019 3:18:03pm
    Author:  Bennet Leff

  ==============================================================================
*/

#include "SamplerAudioSource.h"

//==============================================================================
SamplerAudioSource::SamplerAudioSource(MidiKeyboardState& keyState)
    : keyboardState(keyState)
{
    auto file = File("/Users/bennetleff/Desktop/Nave/fill.wav");

    auto stream = std::make_unique<FileInputStream>(file);
    
    AudioFormatManager formatManager;
    formatManager.registerBasicFormats();

    auto* reader = formatManager.createReaderFor (stream.get()); // [2]

    AudioSampleBuffer fileBuffer;

    if (reader != nullptr)
    {
        auto duration = reader->lengthInSamples / reader->sampleRate;                 // [3]

        if (duration < 4)
        {
            fileBuffer.setSize (reader->numChannels, (int) reader->lengthInSamples);  // [4]
            reader->read (&fileBuffer,                                                // [5]
                          0,                                                          //  [5.1]
                          (int) reader->lengthInSamples,                              //  [5.2]
                          0,                                                          //  [5.3]
                          true,                                                       //  [5.4]
                          true);                                                      //  [5.5]
        }
        else
        {
            // handle the error that the file is 4 seconds or longer..
        }
    }
    
    
    for(auto i = 0; i < 4; ++i)
        synth.addVoice(new SamplerVoice());
    
    allNotes.setRange(0, 128, true);
    
    auto samplerSound = new SamplerSound("Sample", *reader, allNotes, 64, 0.05, 2.0, 4.0);
    
    synth.addSound(samplerSound);
}

void SamplerAudioSource::releaseResources()
{
//    int numVoices = synth.getNumVoices();
//    int numSounds = synth.getNumSounds();
//    
//    for (int i = 0; i < numVoices; i++)
//        synth.removeVoice(i);
//    for (int i = 0; i < numSounds; i++)
//        synth.removeSound(i);
}
