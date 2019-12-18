/*
  ==============================================================================

    GranularAudioSource.cpp
    Created: 18 Dec 2019 2:53:49pm
    Author:  Bennet Leff

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "GranularAudioSource.h"

//==============================================================================
GranularAudioSource::GranularAudioSource(MidiKeyboardState& keyState)
    : keyboardState(keyState)
{
    for(auto i = 0; i < 4; ++i)
        synth.addVoice(new SamplerVoice());
}



//==============================================================================
void GranularAudioSource::setSourceFile(const File& newFile)
{
    sourceFile = newFile;

    AudioFormatManager formatManager;
    formatManager.registerBasicFormats();

    auto* reader = formatManager.createReaderFor (sourceFile); 

    AudioSampleBuffer fileBuffer;

    if (reader != nullptr)
    {
        auto duration = reader->lengthInSamples / reader->sampleRate;

        if (duration < 4)
        {
            fileBuffer.setSize (reader->numChannels, (int) reader->lengthInSamples);
            reader->read (&fileBuffer,
                          0,
                          (int) reader->lengthInSamples,
                          0,
                          true,
                          true);
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

void GranularAudioSource::releaseResources()
{
}
