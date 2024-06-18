/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HaizhouYP04ChorusAudioProcessor::HaizhouYP04ChorusAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter(wetGain = new juce::AudioParameterFloat("wGain", // parameterID,
        "Wet Gain", // parameterName,
        -50.0f, // minValue,
        2.0f, // maxValue,
        -6.0f)); // defaultValue
        // Filter Q parameter
    addParameter(dryGain = new juce::AudioParameterFloat("dGain", // parameterID,
        "Dry Gain", // parameterName,
        -10.0f, // minValue,
        2.0f, // maxValue,
        0.0f)); // defaultValue
    addParameter(mLfoDepthParam = new juce::AudioParameterFloat("depth", // parameterID,
        "LFO Depth", // parameterName,
        0.0f, // minValue,
        48.0f, // maxValue,
        0.5f)); // defaultValue
    addParameter(mLfoSpeedParam = new juce::AudioParameterFloat("speed", // parameterID,
        "LFO Speed", // parameterName,
        0.0f, // minValue,
        20.0f, // maxValue,
        2.0f)); // defaultValue
    addParameter(mDelayParam = new juce::AudioParameterFloat("delay", // parameterID,
        "Delay Time", // parameterName,
        2.0f, // minValue,
        25.0f, // maxValue,
        15.0f)); // defaultValue
    addParameter(mFCParam = new juce::AudioParameterFloat("FC", // parameterID,
        "Corner Frequency", // parameterName,
        20.0f, // minValue,
        10000.0f, // maxValue,
        20.0f)); // defaultValue
}

int HaizhouYP04ChorusAudioProcessor::calcDelaySampsFromMsec(float time){
    return (time * mFs) / 1000;
}

void HaizhouYP04ChorusAudioProcessor::calcDelays()
{
    mDelayTime = mDelayParam->get();
    int samps = calcDelaySampsFromMsec(mDelayTime);
    mDelay1L.setDelay(samps);
    mDelay1R.setDelay(samps);
}

void HaizhouYP04ChorusAudioProcessor::calcGains()
{
    mWetGain = wetGain->get();
    mDryGain = dryGain->get();
    mWetGain = powf(10.0, mWetGain*0.05);
    mDryGain = powf(10.0, mDryGain*0.05);
}

HaizhouYP04ChorusAudioProcessor::~HaizhouYP04ChorusAudioProcessor()
{
}

//==============================================================================
const juce::String HaizhouYP04ChorusAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HaizhouYP04ChorusAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HaizhouYP04ChorusAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HaizhouYP04ChorusAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HaizhouYP04ChorusAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HaizhouYP04ChorusAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HaizhouYP04ChorusAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HaizhouYP04ChorusAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String HaizhouYP04ChorusAudioProcessor::getProgramName (int index)
{
    return {};
}

void HaizhouYP04ChorusAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void HaizhouYP04ChorusAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    mFs = sampleRate;
    int maxSamps = calcDelaySampsFromMsec(50);
    mDelay1L.setMaximumDelay(maxSamps);
    mDelay1R.setMaximumDelay(maxSamps);
    calcAlgorithmParams();
}

void HaizhouYP04ChorusAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HaizhouYP04ChorusAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void HaizhouYP04ChorusAudioProcessor::calcAlgorithmParams() {
    mLFOL.setFreq(mLfoSpeedParam->get(), mFs);
    mLFOR.setFreq(mLfoSpeedParam->get(), mFs);
    calcGains();
    calcDelays();
    float coeffs[5];
    Mu45FilterCalc::calcCoeffsHPF(coeffs, mFCParam->get(), 0.707, mFs);
    filterL.setCoefficients(coeffs[0], coeffs[1], coeffs[2], coeffs[3], coeffs[4]);
    filterR.setCoefficients(coeffs[0], coeffs[1], coeffs[2], coeffs[3], coeffs[4]);
    mDelaysamps = calcDelaySampsFromMsec(mDelayTime);
    mDepthsamps = calcDelaySampsFromMsec((mDelayTime - 1) * mLfoDepthParam->get()/100.0);
}

void HaizhouYP04ChorusAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
        auto totalNumInputChannels  = getTotalNumInputChannels();
        auto totalNumOutputChannels  = getTotalNumOutputChannels();
        for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
            buffer.clear (i, 0, buffer.getNumSamples());
        auto* channelDataR = buffer.getWritePointer(1);
        auto* channelDataL = buffer.getWritePointer(0);
        calcAlgorithmParams();
        float lSamp, rSamp;
        // In case we have more outputs than inputs, this code clears any output
        // channels that didn't contain input data, (because these aren't
        // guaranteed to be empty - they may contain garbage).
        // This is here to avoid people getting screaming feedback
        // when they first compile a plugin, but obviously you don't need to keep
        // this code if your algorithm always overwrites all the output channels.
        // This is the place where you'd normally do the guts of your plugin's
        // audio processing...
        // Make sure to reset the state if your inner loop is processing
        // the samples and the outer loop is handling the channels.
        // Alternatively, you can process the samples with the channels
        // interleaved by keeping the same state.
        const int numSamples = buffer.getNumSamples();
        // This version assumes we always have 2 channels
        calcAlgorithmParams();
        // The "inner loop"
        for (int i = 0; i < numSamples; ++i)
        {
            mDelay1L.setDelay(calcDelaySampsFromMsec(mDelayTime) + mLFOL.tick() * mDepthsamps);
            mDelay1R.setDelay(calcDelaySampsFromMsec(mDelayTime) + mLFOR.flipPhaseTick() * mDepthsamps);
            lSamp = channelDataL[i];
            rSamp = channelDataR[i];
            //make it a choice if want to flip
            channelDataL[i] = lSamp * mDryGain + filterL.tick(mDelay1L.tick(lSamp)) * mWetGain;
            channelDataR[i] = rSamp * mDryGain + filterR.tick(mDelay1R.tick(rSamp)) * mWetGain;
        }
}

//==============================================================================
bool HaizhouYP04ChorusAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HaizhouYP04ChorusAudioProcessor::createEditor()
{
    return new HaizhouYP04ChorusAudioProcessorEditor (*this);
}

//==============================================================================
void HaizhouYP04ChorusAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void HaizhouYP04ChorusAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HaizhouYP04ChorusAudioProcessor();
}
