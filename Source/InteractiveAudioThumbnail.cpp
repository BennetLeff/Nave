/*
  ==============================================================================

    InteractiveAudioThumbnail.cpp
    Created: 6 Jan 2020 2:08:51pm
    Author:  Bennet Leff

  ==============================================================================
*/

#include "InteractiveAudioThumbnail.h"

InteractiveAudioThumbnail::InteractiveAudioThumbnail(int sourceSamplesPerThumbnailSample,
                                                     AudioFormatManager &formatManagerToUse,
                                                     AudioThumbnailCache &cacheToUse)
    : AudioThumbnail(sourceSamplesPerThumbnailSample, formatManagerToUse, cacheToUse)
{
    // thumbnail = std::make_unique<AudioThumbnail>(sourceSamplesPerThumbnailSample, formatManagerToUse, cacheToUse);
}

void InteractiveAudioThumbnail::paintOverChildren(Graphics& g)
{
    Rectangle<int> area (10, 10, 40, 40); // [1]
 
    g.setColour (Colours::orange);
    g.fillRect (area);
    
    DBG("Called");
}

void InteractiveAudioThumbnail::paint (Graphics&)
{
    
}

void InteractiveAudioThumbnail::resized()
{
    
}

void InteractiveAudioThumbnail::draw(Graphics &g, const Rectangle<int> &thumbnailBounds)
{
    if (getNumChannels() == 0)
        paintIfNoFileLoaded (g, thumbnailBounds);
    else
        paintIfFileLoaded (g, thumbnailBounds);
}
