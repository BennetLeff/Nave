/*
  ==============================================================================

    Sampler.h
    Created: 12 Dec 2019 3:18:03pm
    Author:  Bennet Leff

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class Sampler    : public Component
{
public:
    Sampler();
    ~Sampler();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sampler)
};
