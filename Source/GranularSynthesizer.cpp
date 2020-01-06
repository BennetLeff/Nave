/*
  ==============================================================================

    GranularSynthesizer.cpp
    Created: 18 Dec 2019 5:14:54pm
    Author:  Bennet Leff

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "GranularSynthesizer.h"

//==============================================================================
GranularSynthesizer::GranularSynthesizer(MidiKeyboardState& keyboardState)
        : granularAudioSource(keyboardState),
          grainSize(10), // 10 ms
          thumbnailCache (5),
          intThumbnail(512, formatManager, thumbnailCache)
{
    formatManager.registerBasicFormats();
    intThumbnail.addChangeListener(this);
    
    addMouseListener(&intThumbnail, true);
}

GranularSynthesizer::~GranularSynthesizer()
{
}

void GranularSynthesizer::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("Sampler", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
    
    // Painting the audio waveform
    Rectangle<int> thumbnailBounds (10, 10, getWidth() - 20, getHeight() - 20);
    
    intThumbnail.draw(g, thumbnailBounds);
}

void GranularSynthesizer::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void GranularSynthesizer::setSourceFile(const File& newFile)
{
    granularSourceFile = newFile;
    const auto granulatedSampleData = granularAudioSource.setSourceFile(newFile);
    intThumbnail.setSource(new FileInputSource(newFile));
    
//    const auto samplesPerGrain = granulatedSampleData.sampleRate * (granulatedSampleData.grainSize / 1000);
//    const auto numIntervals = static_cast<int>(granulatedSampleData.numSamples / samplesPerGrain);
}

void GranularSynthesizer::changeListenerCallback(ChangeBroadcaster *source)
{
    // Used when the audio thumbnail needs to be redrawn or the
    // grain selector needs to be redrawn.
    if (source == &intThumbnail)
    {
        repaint();
    }
}

void GranularSynthesizer::updateCurrentGrain()
{
    
}
