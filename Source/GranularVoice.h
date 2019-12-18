/*
  ==============================================================================

    GranularVoice.h
    Created: 18 Dec 2019 5:05:09pm
    Author:  Bennet Leff

    Code structure heavily borrows from Juce's SamplerVoice.
  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
    A subclass of SynthesiserVoice that can play a GranularSound.
    To use it, create a Synthesiser, add some GranularVoice objects to it, then
    give it some GranularSound objects to play.
*/
class GranularVoice    : public SynthesiserVoice
{
public:
    //==============================================================================
    /** Creates a SamplerVoice. */
    GranularVoice();

    /** Destructor. */
    ~GranularVoice() override;

    //==============================================================================
    bool canPlaySound (SynthesiserSound*) override;

    void startNote (int midiNoteNumber, float velocity, SynthesiserSound*, int pitchWheel) override;
    void stopNote (float velocity, bool allowTailOff) override;

    void pitchWheelMoved (int newValue) override;
    void controllerMoved (int controllerNumber, int newValue) override;

    void renderNextBlock (AudioBuffer<float>&, int startSample, int numSamples) override;
    using SynthesiserVoice::renderNextBlock;

private:
    //==============================================================================
    double pitchRatio = 0;
    double sourceSamplePosition = 0;
    float lgain = 0, rgain = 0;

    ADSR adsr;

    JUCE_LEAK_DETECTOR (GranularVoice)
};
