/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Mu45FilterCalc/Mu45FilterCalc.h"
#include "StkLite-4.6.2/BiQuad.h"
#include "StkLite-4.6.2/Delay.h"
#include "Mu45LFO/Mu45LFO.h"

//==============================================================================
/**
*/
class HaizhouYP04ChorusAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    HaizhouYP04ChorusAudioProcessor();
    ~HaizhouYP04ChorusAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    juce::AudioParameterFloat* wetGain;
    juce::AudioParameterFloat* dryGain;
    void calcAlgorithmParams();
    juce::AudioParameterFloat* mLfoDepthParam;
    juce::AudioParameterFloat* mLfoSpeedParam;
    juce::AudioParameterFloat* mDelayParam;
    juce::AudioParameterFloat* mFCParam;
    stk::Delay mDelay1L, mDelay1R;
    float mWetGain, mDryGain, mDelayTime, mDelaysamps, mDepthsamps;
    float mFs;
    Mu45LFO mLFOL, mLFOR;
    stk::BiQuad filterL, filterR;
    void calcFilterCoeffs();
    void calcGains();
    void calcDelays();
    int calcDelaySampsFromMsec(float time);
    float calcMiditoHz(float midiNote){return 440.0*powf(2, (midiNote-60.0)/12.0);}
    float calcHztoMidi(float Hz){return 12*log2f(Hz/440.0)+60.0;}
    float mFilterQ = 2;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HaizhouYP04ChorusAudioProcessor)
};
