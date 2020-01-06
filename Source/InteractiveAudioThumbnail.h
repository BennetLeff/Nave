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
    
private:
    void mouseDown(const MouseEvent &event) override;
    
    void paintIfNoFileLoaded (Graphics& g, const Rectangle<int>& thumbnailBounds);
    void paintIfFileLoaded (Graphics& g, const Rectangle<int>& thumbnailBounds);
    
    DrawableRectangle grainSelector;
    Rectangle<int> audioThumbnailBounds;
};
