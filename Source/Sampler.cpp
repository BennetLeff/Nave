/*
  ==============================================================================

    Sampler.cpp
    Created: 13 Dec 2019 7:46:16pm
    Author:  Bennet Leff

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Sampler.h"

//==============================================================================
Sampler::Sampler(MidiKeyboardState& keyboardState)
        : samplerAudioSource(keyboardState),
          thumbnailCache (5),
          thumbnail(512, formatManager, thumbnailCache)
{
    formatManager.registerBasicFormats();
    // thumbnail.addChangeListener(this);
}

Sampler::~Sampler()
{
}

void Sampler::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("Sampler", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
    
    // Painting the audio waveform
    Rectangle<int> thumbnailBounds (10, 100, getWidth() - 20, getHeight() - 120);
    
    if (thumbnail.getNumChannels() == 0)
        paintIfNoFileLoaded (g, thumbnailBounds);
    else
        paintIfFileLoaded (g, thumbnailBounds);
}

void Sampler::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void Sampler::setSourceFile(const File& newFile)
{
    sampleSourceFile = newFile;
    samplerAudioSource.setSourceFile(newFile);
}
