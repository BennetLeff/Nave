/*
  ==============================================================================

    InteractiveAudioThumbnail.h
    Created: 6 Jan 2020 2:08:51pm
    Author:  Bennet Leff

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class InteractiveAudioThumbnail : public Component, public AudioThumbnail
{
public:
    InteractiveAudioThumbnail(int sourceSamplesPerThumbnailSample, AudioFormatManager &formatManagerToUse, AudioThumbnailCache &cacheToUse);
    
    void paint(Graphics& g) override;
    void resized() override;
    
    void draw(Graphics& g, const Rectangle<int>& thumbnailBounds);
    
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
     
        drawChannels (g,
                      thumbnailBounds,
                      0.0,                                    // start time
                      getTotalLength(),                       // end time
                      1.0f);                                  // vertical zoom
    }
    
private:
    void paintOverChildren(Graphics& g) override;
};
