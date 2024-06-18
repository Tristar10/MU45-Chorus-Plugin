/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HaizhouYP04ChorusAudioProcessorEditor::HaizhouYP04ChorusAudioProcessorEditor (HaizhouYP04ChorusAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (330, 300);
    auto& params = processor.getParameters();
    juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(0);
    WGainSlider.setBounds(5, 25, 100, 100);
    WGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    WGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    WGainSlider.setRange(audioParam->range.start, audioParam->range.end);
    WGainSlider.setValue(*audioParam);
    WGainSlider.setColour(WGainSlider.textBoxTextColourId, juce::Colours::white);
    WGainSlider.setColour(WGainSlider.rotarySliderOutlineColourId, juce::Colours::white);
    WGainSlider.addListener(this);
    addAndMakeVisible(WGainSlider);
    WGainLabel.setText ("Wet Gain(dB)", juce::dontSendNotification);
    WGainLabel.setJustificationType(juce::Justification::centred);
    WGainLabel.attachToComponent(&WGainSlider, false);
    WGainLabel.setColour(WGainLabel.textColourId, juce::Colours::white);
    addAndMakeVisible (WGainLabel);
    juce::AudioParameterFloat* audioParam1 = (juce::AudioParameterFloat*)params.getUnchecked(1);
    DGainSlider.setBounds(5, 155, 100, 100);
    DGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    DGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    DGainSlider.setRange(audioParam1->range.start, audioParam1->range.end);
    DGainSlider.setValue(*audioParam1);
    DGainSlider.setColour(DGainSlider.textBoxTextColourId, juce::Colours::white);
    DGainSlider.setColour(DGainSlider.rotarySliderOutlineColourId, juce::Colours::white);
    DGainSlider.addListener(this);
    addAndMakeVisible(DGainSlider);
    DGainLabel.setText ("Dry Gain(dB)", juce::dontSendNotification);
    DGainLabel.setJustificationType(juce::Justification::centred);
    DGainLabel.attachToComponent(&DGainSlider, false);
    DGainLabel.setColour(DGainLabel.textColourId, juce::Colours::white);
    addAndMakeVisible (DGainLabel);
    juce::AudioParameterFloat* audioParam2 = (juce::AudioParameterFloat*)params.getUnchecked(2);
    intSlider.setBounds(115, 25, 100, 100);
    intSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    intSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    intSlider.setRange(audioParam2->range.start, audioParam2->range.end);
    intSlider.setValue(*audioParam2);
    intSlider.setColour(intSlider.textBoxTextColourId, juce::Colours::white);
    intSlider.setColour(intSlider.rotarySliderOutlineColourId, juce::Colours::white);
    intSlider.addListener(this);
    addAndMakeVisible(intSlider);
    intLabel.setText ("Intensity", juce::dontSendNotification);
    intLabel.setJustificationType(juce::Justification::centred);
    intLabel.attachToComponent(&intSlider, false);
    intLabel.setColour(intLabel.textColourId, juce::Colours::white);
    addAndMakeVisible (intLabel);
    juce::AudioParameterFloat* audioParam3 = (juce::AudioParameterFloat*)params.getUnchecked(3);
    rateSlider.setBounds(115, 155, 100, 100);
    rateSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    rateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    rateSlider.setRange(audioParam3->range.start, audioParam3->range.end);
    rateSlider.setValue(*audioParam3);
    rateSlider.setColour(rateSlider.textBoxTextColourId, juce::Colours::white);
    rateSlider.setColour(rateSlider.rotarySliderOutlineColourId, juce::Colours::white);
    rateSlider.addListener(this);
    addAndMakeVisible(rateSlider);
    rateLabel.setText ("Rate(Hz)", juce::dontSendNotification);
    rateLabel.setJustificationType(juce::Justification::centred);
    rateLabel.attachToComponent(&rateSlider, false);
    rateLabel.setColour(rateLabel.textColourId, juce::Colours::white);
    addAndMakeVisible (rateLabel);
    juce::AudioParameterFloat* audioParam4 = (juce::AudioParameterFloat*)params.getUnchecked(4);
    delaySlider.setBounds(225, 25, 100, 100);
    delaySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    delaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    delaySlider.setRange(audioParam4->range.start, audioParam4->range.end);
    delaySlider.setValue(*audioParam4);
    delaySlider.setColour(intSlider.textBoxTextColourId, juce::Colours::white);
    delaySlider.setColour(delaySlider.rotarySliderOutlineColourId, juce::Colours::white);
    delaySlider.addListener(this);
    addAndMakeVisible(delaySlider);
    delayLabel.setText ("Delay Time(ms)", juce::dontSendNotification);
    delayLabel.setJustificationType(juce::Justification::centred);
    delayLabel.attachToComponent(&delaySlider, false);
    delayLabel.setColour(delayLabel.textColourId, juce::Colours::white);
    addAndMakeVisible (delayLabel);
    juce::AudioParameterFloat* audioParam5 = (juce::AudioParameterFloat*)params.getUnchecked(5);
    FCSlider.setBounds(225, 155, 100, 100);
    FCSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    FCSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    FCSlider.setRange(audioParam5->range.start, audioParam5->range.end);
    FCSlider.setValue(*audioParam5);
    FCSlider.setColour(rateSlider.textBoxTextColourId, juce::Colours::white);
    FCSlider.setColour(FCSlider.rotarySliderOutlineColourId, juce::Colours::white);
    FCSlider.addListener(this);
    addAndMakeVisible(FCSlider);
    FCLabel.setText ("Corner Frequency(Hz)", juce::dontSendNotification);
    FCLabel.setJustificationType(juce::Justification::centred);
    FCLabel.attachToComponent(&FCSlider, false);
    FCLabel.setColour(rateLabel.textColourId, juce::Colours::white);
    addAndMakeVisible (FCLabel);
}


HaizhouYP04ChorusAudioProcessorEditor::~HaizhouYP04ChorusAudioProcessorEditor()
{
}

//==============================================================================
void HaizhouYP04ChorusAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    juce::Image background = juce::ImageCache::getFromMemory (BinaryData::long_png, BinaryData::long_pngSize);
    g.drawImageAt (background, 0, 0);
    g.setFont (25.0f);
    g.drawFittedText ("Super Genshin Chorus Y2K Babadeleida", getLocalBounds(), juce::Justification::centredBottom, 15);
    g.setColour (juce::Colours::white);
}

void HaizhouYP04ChorusAudioProcessorEditor::sliderValueChanged(juce::Slider * slider)
{
    auto& params = processor.getParameters();
    if (slider == &WGainSlider)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(0);
        *audioParam = WGainSlider.getValue(); // set the AudioParameter
        DBG("Wet Gain Slider Changed");
    }
    else if (slider == &DGainSlider)
    {
        juce::AudioParameterFloat* audioParam1 = (juce::AudioParameterFloat*)params.getUnchecked(1);
        *audioParam1 = DGainSlider.getValue(); // set the param
        DBG("Dry Gain Slider Changed");
    }
    if (slider == &intSlider)
    {
        juce::AudioParameterFloat* audioParam2 = (juce::AudioParameterFloat*)params.getUnchecked(2);
        *audioParam2 = intSlider.getValue(); // set the AudioParameter
        DBG("Intensity Slider Changed");
    }
    else if (slider == &rateSlider)
    {
        juce::AudioParameterFloat* audioParam3 = (juce::AudioParameterFloat*)params.getUnchecked(3);
        *audioParam3 = rateSlider.getValue(); // set the param
        DBG("Rate Slider Changed");
    }
    else if (slider == &delaySlider)
    {
        juce::AudioParameterFloat* audioParam4 = (juce::AudioParameterFloat*)params.getUnchecked(4);
        *audioParam4 = delaySlider.getValue(); // set the param
        DBG("Rate Slider Changed");
    }
    else if (slider == &FCSlider)
    {
        juce::AudioParameterFloat* audioParam5 = (juce::AudioParameterFloat*)params.getUnchecked(5);
        *audioParam5 = FCSlider.getValue(); // set the param
        DBG("FC Slider Changed");
    }
}

void HaizhouYP04ChorusAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
