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
    setSourceFile({"/Users/bennetleff/Desktop/Nave/resources/fill.wav"});
    
    for(auto i = 0; i < 4; ++i)
        synth.addVoice(new SamplerVoice());
}

void SamplerAudioSource::setSourceFile(const File& newFile)
{
    sourceFile = newFile;

    AudioFormatManager formatManager;
    formatManager.registerBasicFormats();

    auto* reader = formatManager.createReaderFor (sourceFile); // [2]

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
    
    allNotes.setRange(0, 128, true);
    
    auto samplerSound = new SamplerSound("Sample", *reader, allNotes, 64, 0.05, 2.0, 4.0);
    
    delete reader;
    
    synth.addSound(samplerSound);
}

void SamplerAudioSource::releaseResources()
{
}
