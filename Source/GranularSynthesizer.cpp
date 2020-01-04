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
          thumbnailCache (5),
          thumbnail(512, formatManager, thumbnailCache)
{
    formatManager.registerBasicFormats();
    thumbnail.addChangeListener(this);
    
    addAndMakeVisible (grainIDSlider);
    // set this once we know grain count
    grainIDSlider.setRange (0, 100);
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
    
    if (thumbnail.getNumChannels() == 0)
        paintIfNoFileLoaded (g, thumbnailBounds);
    else
        paintIfFileLoaded (g, thumbnailBounds);
}

void GranularSynthesizer::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void GranularSynthesizer::setSourceFile(const File& newFile)
{
    granularSourceFile = newFile;
    granularAudioSource.setSourceFile(newFile);
    thumbnail.setSource(new FileInputSource(newFile));
}

void GranularSynthesizer::changeListenerCallback(ChangeBroadcaster *source)
{
    if (source == &thumbnail)
    {
        repaint();
    }
}
