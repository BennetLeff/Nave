/*
  ==============================================================================

    Sampler.h
    Created: 13 Dec 2019 7:46:16pm
    Author:  Bennet Leff

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SamplerAudioSource.h"

//==============================================================================
/*
 * This class wraps the SamplerAudioSource class to add GUI functionality.
 * The SamplerAudioSource only handles Sample playback. This class draws
 * the AudioThumnail and overlays which allow a user to dictate where the
 * sample playback begins.
 * Any other GUI features that are directly related to the use of the Sampler
 * should probably be here as well.
 */
class Sampler    : public AudioSource, public Component, public ChangeListener
{
public:
    Sampler(MidiKeyboardState& keyboardState);
    ~Sampler();

    void paint (Graphics&) override;
    void resized() override;

    void setSourceFile(const File& newFile);
    
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        samplerAudioSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    }
    
    void releaseResources() override { } 
    
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override
    {
        samplerAudioSource.getNextAudioBlock(bufferToFill);
    }
    
    MidiMessageCollector* getMidiCollector()
    {
        return samplerAudioSource.getMidiCollector();
    }
    
private:
    void changeListenerCallback(ChangeBroadcaster* source) override;
    
    void paintIfNoFileLoaded (Graphics& g, const Rectangle<int>& thumbnailBounds)
    {
        g.setColour (Colours::darkgrey);
        g.fillRect (thumbnailBounds);
        g.setColour (Colours::white);
        g.drawFittedText ("No File Loaded", thumbnailBounds, Justification::centred, 1.0f);
    }

    void paintIfFileLoaded (Graphics& g, const Rectangle<int>& thumbnailBounds)
    {
        g.setColour (Colours::white);
        g.fillRect (thumbnailBounds);
     
        g.setColour (Colours::red);
     
        thumbnail.drawChannels (g,                                     
                                thumbnailBounds,
                                0.0,                                    // start time
                                thumbnail.getTotalLength(),             // end time
                                1.0f);                                  // vertical zoom
    }

    
    SamplerAudioSource samplerAudioSource;
    File sampleSourceFile;
    
    AudioThumbnailCache thumbnailCache;
    AudioThumbnail thumbnail;
    AudioFormatManager formatManager;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sampler)
};
