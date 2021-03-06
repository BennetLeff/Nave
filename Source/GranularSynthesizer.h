/*
  ==============================================================================

    GranularSynthesizer.h
    Created: 18 Dec 2019 5:14:54pm
    Author:  Bennet Leff

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GranularAudioSource.h"
#include "InteractiveAudioThumbnail.h"

//==============================================================================
/*
 * This class wraps the GranularAudioSource class to add GUI functionality.
 * The GranularAudioSource only handles "Granularizing" sound and playback. This class draws
 * the AudioThumnail and overlays which allow a user to dictate where the
 * sample playback begins.
 * Any other GUI features that are directly related to the use of the GranularSynthesizer
 * should probably be here as well.
 */
class GranularSynthesizer    : public AudioSource, public Component, public ChangeListener
{
public:
    GranularSynthesizer(MidiKeyboardState& keyboardState);
    ~GranularSynthesizer();

    void paint (Graphics&) override;
    void resized() override;

    void setSourceFile(const File& newFile);
    
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        granularAudioSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    }
    
    void releaseResources() override { }
    
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override
    {
        granularAudioSource.getNextAudioBlock(bufferToFill);
    }
    
    MidiMessageCollector* getMidiCollector()
    {
        return granularAudioSource.getMidiCollector();
    }
    
private:
    void changeListenerCallback(ChangeBroadcaster* source) override;
    void updateCurrentGrain();
    
    GranularAudioSource granularAudioSource;
    File granularSourceFile;
    
    Slider grainSizeSlider;
    Label grainSizeLabel;
    
    const int grainSize;
    
    AudioThumbnailCache thumbnailCache;
    //AudioThumbnail thumbnail;
    InteractiveAudioThumbnail intThumbnail;
    AudioFormatManager formatManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GranularSynthesizer)
};
