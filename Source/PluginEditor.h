/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "TritonTheme.h"
#include "Meter.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PluginEditor  : public juce::AudioProcessorEditor
{
public:
    //==============================================================================
    PluginEditor (TritonAudioProcessor& p, AudioProcessorValueTreeState& vts);
    ~PluginEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;

    // Binary resources:
    static const char* triton_background_png;
    static const int triton_background_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    TritonAudioProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState;

    std::unique_ptr<SliderAttachment> oscfrAttachment;
    std::unique_ptr<SliderAttachment> oscwfAttachment;
    std::unique_ptr<SliderAttachment> lfofrAttachment;
    std::unique_ptr<SliderAttachment> lfowfAttachment;
    std::unique_ptr<SliderAttachment> typeAttachment;
    std::unique_ptr<SliderAttachment> gainAttachment;

    TritonLookAndFeel tritonLAF;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Slider> oscwfSlider;
    std::unique_ptr<juce::Slider> lfowfSlider;
    std::unique_ptr<juce::Slider> oscfrSlider;
    std::unique_ptr<juce::Slider> lfofrSlider;
    std::unique_ptr<juce::Slider> typeSlider;
    std::unique_ptr<juce::Slider> gainSlider;
    std::unique_ptr<Meter> meterComponent;
    juce::Image cachedImage_triton_background_png_1;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

