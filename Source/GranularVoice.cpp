/*
  ==============================================================================

    GranularVoice.cpp
    Created: 18 Dec 2019 5:05:09pm
    Author:  Bennet Leff

  ==============================================================================
*/

#include "GranularVoice.h"
#include "GranularSound.h"

GranularVoice::GranularVoice() {}
GranularVoice::~GranularVoice() {}

bool GranularVoice::canPlaySound (SynthesiserSound* sound)
{
    return dynamic_cast<const GranularSound*> (sound) != nullptr;
}

void GranularVoice::startNote (int midiNoteNumber, float velocity, SynthesiserSound* s, int /*currentPitchWheelPosition*/)
{
    if (auto* sound = dynamic_cast<const GranularSound*> (s))
    {
        pitchRatio = std::pow (2.0, (midiNoteNumber - sound->midiRootNote) / 12.0)
                        * sound->sourceSampleRate / getSampleRate();

        sourceSamplePosition = 0.0;
        lgain = velocity;
        rgain = velocity;

        adsr.setSampleRate (sound->sourceSampleRate);
        adsr.setParameters (sound->params);

        adsr.noteOn();
    }
    else
    {
        jassertfalse; // this object can only play GranularSounds!
    }
}

void GranularVoice::stopNote (float /*velocity*/, bool allowTailOff)
{
    if (allowTailOff)
    {
        adsr.noteOff();
    }
    else
    {
        clearCurrentNote();
        adsr.reset();
    }
}

void GranularVoice::pitchWheelMoved (int /*newValue*/) {}
void GranularVoice::controllerMoved (int /*controllerNumber*/, int /*newValue*/) {}

/*
 * This function currently renders each buffer by playing through the entire data
 * within a GranularSound. Instead, we'd like to play a section of that data, a Grain.
 */
void GranularVoice::renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if (auto* playingSound = static_cast<GranularSound*> (getCurrentlyPlayingSound().get()))
    {
        auto& data = playingSound->grainData;
        const float* const inL = data.getReadPointer (0);
        const float* const inR = data.getNumChannels() > 1 ? data.getReadPointer (1) : nullptr;

        // Are we in stereo? If yes then set outR to the second write buffer pointer of data,
        // else set outR to nullptr.
        float* outL = outputBuffer.getWritePointer (0, startSample);
        float* outR = outputBuffer.getNumChannels() > 1 ? outputBuffer.getWritePointer (1, startSample) : nullptr;

        // We will fill the buffer with numSamples samples,
        // but if the number of samples in a grain is less than numSamples,
        // we'll need to loop through the grain some number of times to fill the buffer.
        while (--numSamples >= 0)
        {
            auto pos = (int) sourceSamplePosition;
            auto alpha = (float) (sourceSamplePosition - pos);
            auto invAlpha = 1.0f - alpha;

            // Using a very simple linear interpolation here. Going to need to work on this.
            // Possibly use the CatmullRomInterpolator, or LagrangeInterpolator classes from
            // the JUCE library.
            float l = (inL[pos] * invAlpha + inL[pos + 1] * alpha);
            // Again, only process right channel if we're in stereo.
            float r = (inR != nullptr) ? (inR[pos] * invAlpha + inR[pos + 1] * alpha)
                                       : l;

            auto envelopeValue = adsr.getNextSample();

            l *= lgain * envelopeValue;
            r *= rgain * envelopeValue;

            if (outR != nullptr)
            {
                *outL++ += l;
                *outR++ += r;
            }
            else
            {
                *outL++ += (l + r) * 0.5f;
            }

            sourceSamplePosition += pitchRatio;

            if (sourceSamplePosition > playingSound->length)
            {
                stopNote (0.0f, false);
                break;
            }
        }
    }
}
