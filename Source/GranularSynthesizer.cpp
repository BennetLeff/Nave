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
    
    addAndMakeVisible (grainIDSlider);
    // set this once we know grain count
    grainIDSlider.setRange (0, 1);
    grainIDSlider.setTextValueSuffix (" Grain ID");

    grainIDSlider.onValueChange = [this] {
        DBG("Value changed to" << grainIDSlider.getValue());
        granularAudioSource.setGrainID(grainIDSlider.getValue());
    };
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
    
    
    grainIDSlider.setBounds (100, 20, getWidth() - 100 - 10, 20);
    
    // Painting the audio waveform
    Rectangle<int> thumbnailBounds (10, 50, getWidth() - 20, getHeight() - 40);
    
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
    
    const auto samplesPerGrain = granulatedSampleData.sampleRate * (granulatedSampleData.grainSize / 1000);
    
    DBG("granulatedSampleData.sampleRate: " << granulatedSampleData.sampleRate);
    DBG("granulatedSampleData.grainSize / 1000: " << granulatedSampleData.grainSize / 1000);
    DBG("granulatedSampleData.numSamples / samplesPerGrain: " << static_cast<int>(granulatedSampleData.numSamples / samplesPerGrain));
    
    const auto numIntervals = static_cast<int>(granulatedSampleData.numSamples / samplesPerGrain);
    grainIDSlider.setRange (0, numIntervals);
}

void GranularSynthesizer::changeListenerCallback(ChangeBroadcaster *source)
{
    if (source == &intThumbnail)
    {
        repaint();
    }
}
