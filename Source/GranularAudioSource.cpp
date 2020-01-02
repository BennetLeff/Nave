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
    :   grainSize(10), // 10 ms
        keyboardState(keyState)
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

    reader = formatManager.createReaderFor (sourceFile);
    
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
        
    synth.addSound(samplerSound);
    
    granulateSourceFile();
}

void GranularAudioSource::releaseResources()
{
}

void GranularAudioSource::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    
    MidiBuffer incomingMidi;
    midiCollector.removeNextBlockOfMessages(incomingMidi, bufferToFill.numSamples);
    
    keyboardState.processNextMidiBuffer(incomingMidi, bufferToFill.startSample, bufferToFill.numSamples, true);
    
    synth.renderNextBlock(*bufferToFill.buffer, incomingMidi,
                          bufferToFill.startSample, bufferToFill.numSamples);
}

void GranularAudioSource::granulateSourceFile()
{
    if (reader != nullptr)
        for (int i = 0; i < reader->lengthInSamples - samplesPerGrain; i += samplesPerGrain)
        {
            AudioBuffer<float>* currGrainBuffer = new AudioBuffer<float>(2, samplesPerGrain);
            reader->read(currGrainBuffer, 0, samplesPerGrain, i, true, true);
            
            DBG("Sample at i = " << i << " " << currGrainBuffer->getSample(0, 0));
            
            grains.push_back(currGrainBuffer);
        }
    else
        DBG("AudioFormatReader was null?");
}

GranularAudioSource::~GranularAudioSource()
{
    delete reader;
    
    for (const auto& buffer : grains)
        delete buffer;
}
