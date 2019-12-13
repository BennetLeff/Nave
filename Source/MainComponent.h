/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "SamplerAudioSource.h"
#include "SynthAudioSource.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public  AudioAppComponent,
                        private Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    void timerCallback() override
    {
        keyboardComponent.grabKeyboardFocus();
        stopTimer();
    }
    
    void setMidiInput (int index);
    
    AudioDeviceManager deviceManager;           // [1]
    AudioFormatManager formatManager;
    
    ComboBox midiInputList;
    Label midiInputListLabel;
    int lastInputIndex = 0;

    std::unique_ptr<SamplerAudioSource> samplerAudioSource;
    SynthAudioSource synthAudioSource;
    
    MidiKeyboardComponent keyboardComponent;    // [6]
    
    MidiKeyboardState keyboardState;            // [5]
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
