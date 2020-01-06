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
    grainSelector.setColour(0, Colours::blue);
    grainSelector.setFill(Colours::blue);
    
}

void InteractiveAudioThumbnail::paint (Graphics&)
{
    
}

void InteractiveAudioThumbnail::resized()
{
    
}

void InteractiveAudioThumbnail::draw(Graphics &g, const Rectangle<int> &thumbnailBounds)
{
    audioThumbnailBounds = Rectangle<int>(thumbnailBounds);
    
    if (getNumChannels() == 0)
        paintIfNoFileLoaded (g, audioThumbnailBounds);
    else
    {
        paintIfFileLoaded (g, audioThumbnailBounds);
        
        // Create bounds for the grainSelector
        auto bounds = Rectangle<float>(grainSelector.getX(), audioThumbnailBounds.getY(), 20, audioThumbnailBounds.getHeight());
        
        // Constrain the bounds for the grain selector
        // Not sure why, but left sides requires 10, right side requires 20 pixels of room
        if (bounds.getX() < 10)
            bounds.setX(10);
        if (bounds.getX() > audioThumbnailBounds.getX() + audioThumbnailBounds.getWidth() - 20)
            bounds.setX(audioThumbnailBounds.getX() + audioThumbnailBounds.getWidth() - 20);
        
        // Set the bounds
        grainSelector.setRectangle(bounds);
        
        // Draw the grainSelector with an opacity of 30%
        grainSelector.draw(g, 0.3);
    }
}

void InteractiveAudioThumbnail::mouseDown(const MouseEvent &event)
{
    grainSelector.setBounds(event.getMouseDownX(), audioThumbnailBounds.getY(), 20, audioThumbnailBounds.getHeight());
    
    // sendChangeMessage which is intercepted in GranularSynthesizer to update
    // the grainSelector's position in the GUI.
    sendChangeMessage();
}

void InteractiveAudioThumbnail::paintIfNoFileLoaded (Graphics& g, const Rectangle<int>& thumbnailBounds)
{
    g.setColour (Colours::darkgrey);
    g.fillRect (thumbnailBounds);
    g.setColour (Colours::white);
    g.drawFittedText ("No File Loaded", thumbnailBounds, Justification::centred, 1.0f);
}

void InteractiveAudioThumbnail::paintIfFileLoaded (Graphics& g, const Rectangle<int>& thumbnailBounds)
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
