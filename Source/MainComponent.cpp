/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

#include "Sampler.h"

//==============================================================================
MainComponent::MainComponent()
: sampler(keyboardState)
{
    keyboardComponent = std::make_unique<MidiKeyboardComponent>(keyboardState, MidiKeyboardComponent::horizontalKeyboard);

    setupMidi();

    addAndMakeVisible(&sampler);
    
    addAndMakeVisible (keyboardComponent.get());
    
    setAudioChannels (0, 2);
    
    addAndMakeVisible(&setSample);

    setSample.setButtonText("Set sample");
    setSample.onClick = [this] { setSampleButtonClicked(); };
    
    
    setSize (800, 600);
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
    sampler.prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    sampler.getNextAudioBlock (bufferToFill);
}

void MainComponent::releaseResources()
{
    sampler.releaseResources();
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
    midiInputList.setBounds (200, 10, getWidth() - 210, 20);
    sampler.setBounds(10, 30, getWidth() - 20, 170);
    keyboardComponent->setBounds (10,  210, getWidth() - 20, getHeight() - 50);
    setSample.setBounds(10, 10, 100, 20);

}


void MainComponent::setMidiInput (int index)
{
    auto list = MidiInput::getDevices();
 
    deviceManager.removeMidiInputCallback (list[lastInputIndex], sampler.getMidiCollector()); // [13]
    
    auto newInput = list[index];
 
    if (! deviceManager.isMidiInputEnabled (newInput))
        deviceManager.setMidiInputEnabled (newInput, true);
 
    deviceManager.addMidiInputCallback (newInput, sampler.getMidiCollector()); // [12]
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
        sampler.setSourceFile(file);
    }
}
