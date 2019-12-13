/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

#include "SamplerAudioSource.h"

//==============================================================================
MainComponent::MainComponent()
: synthAudioSource{keyboardState},
    keyboardComponent (keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
    auto file = File("/Users/bennetleff/Desktop/Nave/fill.wav");

    AudioFormatManager formatManager;
    formatManager.registerBasicFormats();

    auto* reader = formatManager.createReaderFor (file); // [2]

    AudioSampleBuffer fileBuffer;

    if (reader != nullptr)
    {
        auto duration = reader->lengthInSamples / reader->sampleRate;                 // [3]

        if (duration < 4)
        {
            fileBuffer.setSize (reader->numChannels, (int) reader->lengthInSamples);  // [4]
            reader->read (&fileBuffer,                                                // [5]
                          0,                                                          //  [5.1]
                          (int) reader->lengthInSamples,                              //  [5.2]
                          0,                                                          //  [5.3]
                          true,                                                       //  [5.4]
                          true);                                                      //  [5.5]
        }
        else
        {
            // handle the error that the file is 4 seconds or longer..
        }
    }

    samplerAudioSource = std::make_unique<SamplerAudioSource>(keyboardState, *reader);
    
    addAndMakeVisible (midiInputListLabel);
    midiInputListLabel.setText ("MIDI Input:", dontSendNotification);
    midiInputListLabel.attachToComponent (&midiInputList, true);

    //auto midiInputs = MidiInput::getAvailableDevices();
    auto midiInputs = MidiInput::getDevices();
    addAndMakeVisible (midiInputList);
    midiInputList.setTextWhenNoChoicesAvailable ("No MIDI Inputs Enabled");
    
    StringArray midiInputNames;
    for (auto input : midiInputs)
        midiInputNames.add (input);
    
    midiInputList.addItemList (midiInputNames, 1);
    midiInputList.onChange = [this] { setMidiInput (midiInputList.getSelectedItemIndex()); };

    for (auto input : midiInputs)
    {
        //if (deviceManager.isMidiInputDeviceEnabled (input.identifier))
        if (deviceManager.isMidiInputEnabled(input))
        {
            std::cout << "GOT MIDI DEVICE\n";
            setMidiInput (midiInputs.indexOf (input));
            break;
        }
    }

    if (midiInputList.getSelectedId() == 0)
        setMidiInput (0);

    addAndMakeVisible (keyboardComponent);
    setAudioChannels (0, 2);
    
        
    setSize (600, 190);
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
    // if (samplerAudioSource != nullptr)
    samplerAudioSource->prepareToPlay (samplesPerBlockExpected, sampleRate);
    //synthAudioSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // if (samplerAudioSource != nullptr)
    samplerAudioSource->getNextAudioBlock (bufferToFill);
    // synthAudioSource.getNextAudioBlock (bufferToFill);
}

void MainComponent::releaseResources()
{
    // if (samplerAudioSource != nullptr)
    samplerAudioSource->releaseResources();
    // synthAudioSource.releaseResources();
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
 
//    deviceManager.removeMidiInputCallback (list[lastInputIndex], samplerAudioSource->getMidiCollector()); // [13]
    deviceManager.removeMidiInputCallback (list[lastInputIndex], synthAudioSource.getMidiCollector()); // [13]
    
    auto newInput = list[index];
 
    if (! deviceManager.isMidiInputEnabled (newInput))
        deviceManager.setMidiInputEnabled (newInput, true);
 
    // deviceManager.addMidiInputCallback (newInput, samplerAudioSource->getMidiCollector()); // [12]
    deviceManager.addMidiInputCallback (newInput, synthAudioSource.getMidiCollector());
    midiInputList.setSelectedId (index + 1, dontSendNotification);
 
    lastInputIndex = index;
}

