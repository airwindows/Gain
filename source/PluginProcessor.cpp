#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginProcessor::PluginProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
{
}

PluginProcessor::~PluginProcessor()
{
}

//==============================================================================
const juce::String PluginProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PluginProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PluginProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PluginProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PluginProcessor::getCurrentProgram()
{
    return 0;
}

void PluginProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String PluginProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void PluginProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void PluginProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool PluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this metering code we only support stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void PluginProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    
    
    
  /*  UIToAudioMessage uim;
    while (uiToAudio.pop(uim))
    {
        switch (uim.what)
        {
        case UIToAudioMessage::NEW_VALUE:
            params[uim.which]->setValueNotifyingHost(params[uim.which]->convertTo0to1(uim.newValue)); break;
        case UIToAudioMessage::BEGIN_EDIT:
            break;
        case UIToAudioMessage::END_EDIT:
            break;
        }
    } //This is several ways to change the controls: JUCE handles this stuff. */

    if (!(getBus(false, 0)->isEnabled() && getBus(true, 0)->isEnabled())) return;
    auto mainOutput = getBusBuffer(buffer, false, 0); //if we have audio busses at all,
    auto mainInput = getBusBuffer(buffer, true, 0); //they're now mainOutput and mainInput.
    double rmsSize = (1881.0 / 44100.0)*getSampleRate(); //higher is slower with larger RMS buffers
    
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        auto outL = mainOutput.getWritePointer(0, i);
        auto outR = mainOutput.getWritePointer(1, i);
        auto inL = mainInput.getReadPointer(0, i); //in isBussesLayoutSupported, we have already
        auto inR = mainInput.getReadPointer(1, i); //specified that we can only be stereo and never mono

        float currentslewL = (fabs(*inL-previousLeft)/28000.0f)*getSampleRate();
        float currentslewR = (fabs(*inR-previousRight)/28000.0f)*getSampleRate();
        if (currentslewL > slewLeft) slewLeft = currentslewL;
        if (currentslewR > slewRight) slewRight = currentslewR;
        previousLeft = *inL;
        previousRight = *inR;
        //slew measurement is NOT rectified
        
        float rectifiedL = fabs(*inL);
        float rectifiedR = fabs(*inR);
        if (rectifiedL > peakLeft) peakLeft = rectifiedL;
        if (rectifiedR > peakRight) peakRight = rectifiedR;
        rmsLeft += (rectifiedL * rectifiedL);
        rmsRight += (rectifiedR * rectifiedR);
        rmsCount++; //rms loudness IS rectified
        
        *outL = *inL * 0.5f;
        *outR = *inR * 0.5f; //this is a meter. Raw pass-through
    }


    if (rmsCount > rmsSize)
    {
        AudioToUIMessage msg; //define the thing we're telling JUCE
        
        msg.what = AudioToUIMessage::RMS_LEFT;
        msg.newValue = sqrt(sqrt(rmsLeft / rmsCount));
        audioToUI.push(msg);
        
        msg.what = AudioToUIMessage::RMS_RIGHT;
        msg.newValue = sqrt(sqrt(rmsRight / rmsCount));
        audioToUI.push(msg);
        
        msg.what = AudioToUIMessage::PEAK_LEFT;
        msg.newValue = sqrt(peakLeft);
        audioToUI.push(msg);
        
        msg.what = AudioToUIMessage::PEAK_RIGHT;
        msg.newValue = sqrt(peakRight);
        audioToUI.push(msg);
        
        msg.what = AudioToUIMessage::SLEW_LEFT;
        msg.newValue = slewLeft;
        audioToUI.push(msg);
        
        msg.what = AudioToUIMessage::SLEW_RIGHT;
        msg.newValue = slewRight;
        audioToUI.push(msg);
        
        msg.what = AudioToUIMessage::INCREMENT;
        msg.newValue = 1200; //wrap to this value. Only here because we are sending a value anyway
        audioToUI.push(msg);
        //tell JUCE. we have WHAT as where to send the message and newValue as the number we're sending.
        
        rmsLeft = 0.0;
        rmsRight = 0.0;
        peakLeft = 0.0;
        peakRight = 0.0;
        slewLeft = 0.0;
        slewRight = 0.0; //reset our variables to do the RMS again
        rmsCount = 0;
    }


 /*   for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelIn = buffer.getReadPointer (channel);
        auto* channelOut = buffer.getWritePointer (channel);
        
        for (int i = 0; i < buffer.getNumSamples(); ++i)  {
            channelOut[i] = channelIn[i] * 0.5f;
        }
    } */
    
}

//==============================================================================
bool PluginProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor (*this);
}

//==============================================================================
void PluginProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}
