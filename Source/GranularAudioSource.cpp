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
        grainID(0),
        keyboardState(keyState),
        envelopeAttack(0.05), // 50 ms
        envelopeRelease(2.0)  // 2 seconds
        
{
    for(auto i = 0; i < 4; ++i)
        synth.addVoice(new GranularVoice());
}


//==============================================================================
const SampleData GranularAudioSource::setSourceFile(const File& newFile)
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
    
    grains = granulateSourceFile();
    
    auto granularSound = new GranularSound ("Grain",
                                         *grains[grainID],
                                         reader->sampleRate,
                                         allNotes,
                                         64,
                                         envelopeAttack,
                                         envelopeRelease,
                                         4.0);
            
    synth.addSound(granularSound);
    
    // const SampleData parsedData = {reader->sampleRate, reader->lengthInSamples, grainSize};
    
    DBG("reader->sampleRate: " << reader->sampleRate);
    
    return SampleData{reader->sampleRate, reader->lengthInSamples, grainSize};
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

std::vector<AudioBuffer<float>*> GranularAudioSource::granulateSourceFile()
{
    std::vector<AudioBuffer<float>*> paritionedGrains;
    
    if (reader != nullptr)
        for (int i = 0; i < reader->lengthInSamples - samplesPerGrain; i += samplesPerGrain)
        {
            AudioBuffer<float>* currGrainBuffer = new AudioBuffer<float>(2, samplesPerGrain);
            reader->read(currGrainBuffer, 0, samplesPerGrain, i, true, true);
            
            DBG("Sample at i = " << i << " " << currGrainBuffer->getSample(0, 0));
            
            paritionedGrains.push_back(currGrainBuffer);
        }
    else
        DBG("AudioFormatReader was null?");
    
    return paritionedGrains;
}

void GranularAudioSource::setGrainID(const int newGrainID)
{
    if (newGrainID < grains.size())
    {
        // Update the grainID
        grainID = newGrainID;
        
        // There should only be one sound so we getSound 0
        // We case this SynthesiserSound* to a GranularSound* and update its data.
        auto sound = static_cast<GranularSound*>(synth.getSound(0).get());
        sound->setAudioData(*grains[grainID]);
    }
}

GranularAudioSource::~GranularAudioSource()
{
    delete reader;
    
    for (const auto& buffer : grains)
        delete buffer;
}
