/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

#include "SamplerAudioSource.h"

//==============================================================================
MainComponent::MainComponent()
: samplerAudioSource(keyboardState),
    keyboardComponent (keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
    setupMidi();

    addAndMakeVisible (keyboardComponent);
    setAudioChannels (0, 2);
    
    setSample.setBounds(10, 10, 100, 20);
    addAndMakeVisible(&setSample);

    setSample.setButtonText("Set sample");
    setSample.onClick = [this] { setSampleButtonClicked(); };
    
    setSize (800, 300);
    startTimer (400);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    samplerAudioSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    samplerAudioSource.getNextAudioBlock (bufferToFill);
}

void MainComponent::releaseResources()
{
    samplerAudioSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    midiInputList    .setBounds (200, 10, getWidth() - 210, 20);
    keyboardComponent.setBounds (10,  40, getWidth() - 20, getHeight() - 50);
}


void MainComponent::setMidiInput (int index)
{
    auto list = MidiInput::getDevices();
 
    deviceManager.removeMidiInputCallback (list[lastInputIndex], samplerAudioSource.getMidiCollector()); // [13]
    
    auto newInput = list[index];
 
    if (! deviceManager.isMidiInputEnabled (newInput))
        deviceManager.setMidiInputEnabled (newInput, true);
 
    deviceManager.addMidiInputCallback (newInput, samplerAudioSource.getMidiCollector()); // [12]
    midiInputList.setSelectedId (index + 1, dontSendNotification);
 
    lastInputIndex = index;
}


void MainComponent::setupMidi()
{
    addAndMakeVisible (midiInputListLabel);
    midiInputListLabel.setText ("MIDI Input:", dontSendNotification);
    midiInputListLabel.attachToComponent (&midiInputList, true);

    auto midiInputs = MidiInput::getAvailableDevices();
    addAndMakeVisible (midiInputList);
    midiInputList.setTextWhenNoChoicesAvailable ("No MIDI Inputs Enabled");
    
    StringArray midiInputNames;
    for (auto input : midiInputs)
        midiInputNames.add (input.name);
    
    midiInputList.addItemList (midiInputNames, 1);
    midiInputList.onChange = [this] { setMidiInput (midiInputList.getSelectedItemIndex()); };

    for (auto input : midiInputs)
    {
        if (deviceManager.isMidiInputDeviceEnabled (input.identifier))
        {
            setMidiInput (midiInputs.indexOf (input));
            break;
        }
    }

    if (midiInputList.getSelectedId() == 0)
        setMidiInput (0);
}

void MainComponent::setSampleButtonClicked()
{
    FileChooser chooser ("Select a Wave file to play...",
                         {},
                         "*.wav");
 
    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        samplerAudioSource.setSourceFile(file);
    }
}
