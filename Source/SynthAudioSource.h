/*
  ==============================================================================

    SynthAudioSource.h
    Created: 12 Dec 2019 3:23:56pm
    Author:  Bennet Leff

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class SineWaveSound : public SynthesiserSound
{
public:
    SineWaveSound() {}
    
    bool appliesToNote (int) override { return true; }
    bool appliesToChannel (int) override { return true; }
};

class SineWaveVoice : public SynthesiserVoice
{
public:

    bool canPlaySound(SynthesiserSound* sound) override
    {
        return dynamic_cast<SineWaveSound*>(sound) != nullptr;
    }
    
    void startNote(int midiNoteNumber, float velocity,
                   SynthesiserSound*, int) override
    {
        currentAngle = 0.0;
        level = velocity * 0.15;
        tailOff = 0.0;
        
        auto cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        auto cyclesPerSample = cyclesPerSecond / getSampleRate();
        
        angleDelta = cyclesPerSample * 2.0 * MathConstants<double>::pi;
    }
 
    void renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {
        if (angleDelta != 0.0) {
            if (tailOff > 0.0) {
                while (--numSamples >= 0)
                {
                    auto currentSample = (float) (std::sin (currentAngle) * level * tailOff);
                    
                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                    {
                        outputBuffer.addSample(i, startSample, currentSample);
                    }
                    
                    currentAngle += angleDelta;
                    ++startSample;
                    
                    tailOff *= 0.99;
                    
                    if (tailOff <= 0.005)
                    {
                        clearCurrentNote();
                        
                        angleDelta = 0.0;
                        break;
                    }
                }
            }
            else {
                while (--numSamples >= 0)
                {
                    auto currentSample = (float) (std::sin (currentAngle) * level);
                    
                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                        outputBuffer.addSample(i, startSample, currentSample);
                    
                    currentAngle += angleDelta;
                    ++startSample;
                }
            }
        }
    }
    
    void stopNote (float velocity, bool allowTailOff) override
    {
        if (allowTailOff)
        {
            if (tailOff == 0.0)
                tailOff = 1.0;
        }
        else
        {
            clearCurrentNote();
            angleDelta = 0.0;
        }
    }
    
    void pitchWheelMoved (int newPitchWheelValue) override { /* not using at the moment */ }

    void controllerMoved (int controllerNumber, int newControllerValue) override { /* not using at the moment */ }
    
private:
    double currentAngle = 0.0, angleDelta = 0.0, level = 0.0, tailOff = 0.0;
};

class SynthAudioSource : public AudioSource
{
public:
    SynthAudioSource (MidiKeyboardState& keyState)
        : keyboardState(keyState)
    {
        for(auto i = 0; i < 4; ++i)
            synth.addVoice(new SineWaveVoice());
        
        synth.addSound(new SineWaveSound());
    }
    
    void setUsingSineWaveSound()
    {
        synth.clearSounds();
    }
    
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        synth.setCurrentPlaybackSampleRate (sampleRate);
        midiCollector.reset (sampleRate);
    }
    
    void releaseResources() override {}
    
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override
    {
        bufferToFill.clearActiveBufferRegion();
        
        MidiBuffer incomingMidi;
        midiCollector.removeNextBlockOfMessages(incomingMidi, bufferToFill.numSamples);
        
        keyboardState.processNextMidiBuffer(incomingMidi, bufferToFill.startSample, bufferToFill.numSamples, true);
        
        synth.renderNextBlock(*bufferToFill.buffer, incomingMidi,
                              bufferToFill.startSample, bufferToFill.numSamples);
    }
    
    MidiMessageCollector* getMidiCollector()
    {
        return &midiCollector;
    }
    
private:
    MidiKeyboardState& keyboardState;
    MidiMessageCollector midiCollector;
    Synthesiser synth;
};
