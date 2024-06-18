/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class HaizhouYP04ChorusAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener
{
public:
    HaizhouYP04ChorusAudioProcessorEditor (HaizhouYP04ChorusAudioProcessor&);
    ~HaizhouYP04ChorusAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    HaizhouYP04ChorusAudioProcessor& audioProcessor;
    juce::Slider FCSlider;
    juce::Slider WGainSlider;
    juce::Slider DGainSlider;
    juce::Slider rateSlider;
    juce::Slider intSlider;
    juce::Slider delaySlider;
    juce::Label FCLabel;
    juce::Label WGainLabel;
    juce::Label DGainLabel;
    juce::Label rateLabel;
    juce::Label intLabel;
    juce::Label delayLabel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HaizhouYP04ChorusAudioProcessorEditor)
};
