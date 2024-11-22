#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginProcessor::PluginProcessor():AudioProcessor (
                    BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
){
    fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
    fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
    //this is reset: values being initialized only once. Startup values, whatever they are.

    // (internal ID, how it's shown in DAW generic view, {min, max}, default)
    addParameter(params[KNOBHIP] = new juce::AudioParameterFloat("highpass", "Highpass", {0.0f, 1.0f}, 0.0f));              params[KNOBHIP]->addListener(this);
    addParameter(params[KNOBLOP] = new juce::AudioParameterFloat("lowpass", "Lowpass", {0.0f, 1.0f}, 0.0f));                params[KNOBLOP]->addListener(this);
    addParameter(params[KNOBAIR] = new juce::AudioParameterFloat("air", "Air", {0.0f, 1.0f}, 0.5f));                        params[KNOBAIR]->addListener(this);
    addParameter(params[KNOBFIR] = new juce::AudioParameterFloat("fire", "Fire", {0.0f, 1.0f}, 0.5f));                      params[KNOBFIR]->addListener(this);
    addParameter(params[KNOBSTO] = new juce::AudioParameterFloat("stone", "Stone", {0.0f, 1.0f}, 0.5f));                    params[KNOBSTO]->addListener(this);
    addParameter(params[KNOBRNG] = new juce::AudioParameterFloat("range", "Range", {0.0f, 1.0f}, 0.5f));                    params[KNOBRNG]->addListener(this);
    addParameter(params[KNOBFCT] = new juce::AudioParameterFloat("fcthreshold", "FC Threshold", {0.0f, 1.0f}, 1.0f));       params[KNOBFCT]->addListener(this);
    addParameter(params[KNOBSCT] = new juce::AudioParameterFloat("scthreshold", "SC Threshold", {0.0f, 1.0f}, 1.0f));       params[KNOBSCT]->addListener(this);
    addParameter(params[KNOBFCR] = new juce::AudioParameterFloat("fcratio", "FC Ratio", {0.0f, 1.0f}, 1.0f));               params[KNOBFCR]->addListener(this);
    addParameter(params[KNOBSCR] = new juce::AudioParameterFloat("scratio", "SC Ratio", {0.0f, 1.0f}, 1.0f));               params[KNOBSCR]->addListener(this);
    addParameter(params[KNOBFCA] = new juce::AudioParameterFloat("fcattack", "FC Attack", {0.0f, 1.0f}, 0.5f));             params[KNOBFCA]->addListener(this);
    addParameter(params[KNOBSCA] = new juce::AudioParameterFloat("scattack", "SC ATtack", {0.0f, 1.0f}, 0.5f));             params[KNOBSCA]->addListener(this);
    addParameter(params[KNOBFCL] = new juce::AudioParameterFloat("fcrelease", "FC Release", {0.0f, 1.0f}, 0.5f));           params[KNOBFCL]->addListener(this);
    addParameter(params[KNOBSCL] = new juce::AudioParameterFloat("screlease", "SC Release", {0.0f, 1.0f}, 0.5f));           params[KNOBSCL]->addListener(this);
    addParameter(params[KNOBFGT] = new juce::AudioParameterFloat("fgthreshold", "FG Threshold", {0.0f, 1.0f}, 0.0f));       params[KNOBFGT]->addListener(this);
    addParameter(params[KNOBSGT] = new juce::AudioParameterFloat("sgthreshold", "SG Threshold", {0.0f, 1.0f}, 0.0f));       params[KNOBSGT]->addListener(this);
    addParameter(params[KNOBFGR] = new juce::AudioParameterFloat("fgratio", "FG Ratio", {0.0f, 1.0f}, 1.0f));               params[KNOBFGR]->addListener(this);
    addParameter(params[KNOBSGR] = new juce::AudioParameterFloat("sgratio", "SG Ratio", {0.0f, 1.0f}, 1.0f));               params[KNOBSGR]->addListener(this);
    addParameter(params[KNOBFGS] = new juce::AudioParameterFloat("fgsustain", "FG Sustain", {0.0f, 1.0f}, 0.5f));           params[KNOBFGS]->addListener(this);
    addParameter(params[KNOBSGS] = new juce::AudioParameterFloat("sgsustain", "SG Sustain", {0.0f, 1.0f}, 0.5f));           params[KNOBSGS]->addListener(this);
    addParameter(params[KNOBFGL] = new juce::AudioParameterFloat("fgrelease", "FG Release", {0.0f, 1.0f}, 0.5f));           params[KNOBFGL]->addListener(this);
    addParameter(params[KNOBSGL] = new juce::AudioParameterFloat("sgrelease", "SG Release", {0.0f, 1.0f}, 0.5f));           params[KNOBSGL]->addListener(this);
    addParameter(params[KNOBTRF] = new juce::AudioParameterFloat("treblefreq", "Treble Freq", {0.0f, 1.0f}, 0.5f));         params[KNOBTRF]->addListener(this);
    addParameter(params[KNOBTRG] = new juce::AudioParameterFloat("treble", "Treble", {0.0f, 1.0f}, 0.5f));                  params[KNOBTRG]->addListener(this);
    addParameter(params[KNOBTRR] = new juce::AudioParameterFloat("treblereso", "Treble Reso", {0.0f, 1.0f}, 0.5f));         params[KNOBTRR]->addListener(this);
    addParameter(params[KNOBHMF] = new juce::AudioParameterFloat("highmidfreq", "HighMid Freq", {0.0f, 1.0f}, 0.5f));       params[KNOBHMF]->addListener(this);
    addParameter(params[KNOBHMG] = new juce::AudioParameterFloat("highmid", "HighMid", {0.0f, 1.0f}, 0.5f));                params[KNOBHMG]->addListener(this);
    addParameter(params[KNOBHMR] = new juce::AudioParameterFloat("highmidreso", "HighMid Reso", {0.0f, 1.0f}, 0.5f));       params[KNOBHMR]->addListener(this);
    addParameter(params[KNOBLMF] = new juce::AudioParameterFloat("lowmidfreq", "LowMid Freq", {0.0f, 1.0f}, 0.5f));         params[KNOBLMF]->addListener(this);
    addParameter(params[KNOBLMG] = new juce::AudioParameterFloat("lowmid", "LowMid", {0.0f, 1.0f}, 0.5f));                  params[KNOBLMG]->addListener(this);
    addParameter(params[KNOBLMR] = new juce::AudioParameterFloat("lowmidreso", "LowMid Reso", {0.0f, 1.0f}, 0.5f));         params[KNOBLMR]->addListener(this);
    addParameter(params[KNOBBSF] = new juce::AudioParameterFloat("bassfreq", "Bass Freq", {0.0f, 1.0f}, 0.5f));             params[KNOBBSF]->addListener(this);
    addParameter(params[KNOBBSG] = new juce::AudioParameterFloat("bass", "Bass", {0.0f, 1.0f}, 0.5f));                      params[KNOBBSG]->addListener(this);
    addParameter(params[KNOBBSR] = new juce::AudioParameterFloat("bassreso", "Bass Reso", {0.0f, 1.0f}, 0.5f));             params[KNOBBSR]->addListener(this);
    addParameter(params[KNOBDSC] = new juce::AudioParameterFloat("topdb", "Top dB", {0.0f, 1.0f}, 0.5f));                   params[KNOBDSC]->addListener(this);
    addParameter(params[KNOBPAN] = new juce::AudioParameterFloat("pan", "Pan", {0.0f, 1.0f}, 0.5f));                        params[KNOBPAN]->addListener(this);
    addParameter(params[KNOBFAD] = new juce::AudioParameterFloat("fader", "Fader", {0.0f, 1.0f}, 0.5f));                    params[KNOBFAD]->addListener(this);
}

PluginProcessor::~PluginProcessor() {}
void PluginProcessor::parameterValueChanged(int parameterIndex, float newValue)
{
    AudioToUIMessage msg;
    msg.what = AudioToUIMessage::NEW_VALUE;
    msg.which = (PluginProcessor::Parameters)parameterIndex;
    msg.newValue = params[parameterIndex]->convertFrom0to1(newValue);
    audioToUI.push(msg);
}
void PluginProcessor::parameterGestureChanged(int parameterIndex, bool starting) {}
const juce::String PluginProcessor::getName() const {return JucePlugin_Name;}
bool PluginProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}
bool PluginProcessor::supportsDoublePrecisionProcessing() const
{
   #if JucePlugin_SupportsDoublePrecisionProcessing
    return true;
   #else
    return true;
    //note: I don't know whether that config option is set, so I'm hardcoding it
    //knowing I have enabled such support: keeping the boilerplate stuff tho
    //in case I can sort out where it's enabled as a flag
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
double PluginProcessor::getTailLengthSeconds() const {return 0.0;}
int PluginProcessor::getNumPrograms() {return 1;}
int PluginProcessor::getCurrentProgram() {return 0;}
void PluginProcessor::setCurrentProgram (int index) {juce::ignoreUnused (index);}
const juce::String PluginProcessor::getProgramName (int index) {juce::ignoreUnused (index); return {};}
void PluginProcessor::changeProgramName (int index, const juce::String& newName) {juce::ignoreUnused (index, newName);}

//==============================================================================
void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) {juce::ignoreUnused (sampleRate, samplesPerBlock);}
void PluginProcessor::releaseResources() {}
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

void PluginProcessor::updateTrackProperties(const TrackProperties& properties)
{
    trackProperties = properties;
    // call the verison in the editor to update there
    if (auto* editor = dynamic_cast<PluginEditor*> (getActiveEditor()))
        editor->updateTrackProperties();
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}


void PluginProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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
    
    if (!(getBus(false, 0)->isEnabled() && getBus(true, 0)->isEnabled())) return;
    auto mainOutput = getBusBuffer(buffer, false, 0); //if we have audio busses at all,
    auto mainInput = getBusBuffer(buffer, true, 0); //they're now mainOutput and mainInput.
    
    UIToAudioMessage uim;
    while (uiToAudio.pop(uim)) {
        switch (uim.what) {
        case UIToAudioMessage::NEW_VALUE: params[uim.which]->setValueNotifyingHost(params[uim.which]->convertTo0to1(uim.newValue)); break;
        case UIToAudioMessage::BEGIN_EDIT: params[uim.which]->beginChangeGesture(); break;
        case UIToAudioMessage::END_EDIT: params[uim.which]->endChangeGesture(); break;
        }
    } //Handle inbound messages from the UI thread

    double overallscale = 1.0;
    overallscale /= 44100.0;
    overallscale *= getSampleRate();
    double rmsSize = (1881.0 / 44100.0)*getSampleRate(); //higher is slower with larger RMS buffers
    double zeroCrossScale = (1.0 / getSampleRate())*44100.0;

    auto gainL = params[KNOBHIP]->get();
    auto gainR = params[KNOBLOP]->get();
    auto gainX = params[KNOBAIR]->get();

    int inFramesToProcess = buffer.getNumSamples(); //vst doesn't give us this as a separate variable so we'll make it
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        auto outL = mainOutput.getWritePointer(0, i);
        auto outR = mainOutput.getWritePointer(1, i);
        auto inL = mainInput.getReadPointer(0, i); //in isBussesLayoutSupported, we have already
        auto inR = mainInput.getReadPointer(1, i); //specified that we can only be stereo and never mono
        double inputSampleL = *inL;
        double inputSampleR = *inR;
        if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
        if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
        //double drySampleL = inputSampleL;
        //double drySampleR = inputSampleR;
        //double temp = (double)i/inFramesToProcess;
        //double gain = (gainA*(1.0-temp))+(gainB*temp);
        //NOTE: I don't yet know whether counting the buffer backwards means that gainA and gainB must be reversed.
        //If the audio flow is in fact reversed we must swap the temp and (1.0-temp) and I have done this provisionally
        //Original VST2 is counting DOWN with -- operator, but this counts UP with ++
        //If this doesn't create zipper noise on sine processing then it's correct
        
        

        //begin bar display section
        if ((fabs(*inL-previousLeft)/28000.0f)*getSampleRate() > slewLeft) slewLeft =  (fabs(*inL-previousLeft)/28000.0f)*getSampleRate();
        if ((fabs(*inR-previousRight)/28000.0f)*getSampleRate() > slewRight) slewRight = (fabs(*inR-previousRight)/28000.0f)*getSampleRate();
        previousLeft = *inL; previousRight = *inR; //slew measurement is NOT rectified
        double rectifiedL = fabs(*inL);
        double rectifiedR = fabs(*inR);
        if (rectifiedL > peakLeft) peakLeft = rectifiedL;
        if (rectifiedR > peakRight) peakRight = rectifiedR;
        rmsLeft += (rectifiedL * rectifiedL);
        rmsRight += (rectifiedR * rectifiedR);
        rmsCount++;
        zeroLeft += zeroCrossScale;
        if (longestZeroLeft < zeroLeft) longestZeroLeft = zeroLeft;
        if (wasPositiveL && *inL < 0.0) {
            wasPositiveL = false;
            zeroLeft = 0.0;
        } else if (!wasPositiveL && *inL > 0.0) {
            wasPositiveL = true;
            zeroLeft = 0.0;
        }
        zeroRight += zeroCrossScale;
        if (longestZeroRight < zeroRight) longestZeroRight = zeroRight;
        if (wasPositiveR && *inR < 0.0) {
            wasPositiveR = false;
            zeroRight = 0.0;
        } else if (!wasPositiveR && *inR > 0.0) {
            wasPositiveR = true;
            zeroRight = 0.0;
        }

        
        //begin 32 bit stereo floating point dither
        int expon; frexpf((float)inputSampleL, &expon);
        fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
        inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
        frexpf((float)inputSampleR, &expon);
        fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
        inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
        //end 32 bit stereo floating point dither

        
        *outL = inputSampleL;
        *outR = inputSampleR;
    }


    if (rmsCount > rmsSize)
    {
        AudioToUIMessage msg; //define the thing we're telling JUCE
        msg.what = AudioToUIMessage::SLEW_LEFT; msg.newValue = (float)slewLeft; audioToUI.push(msg);
        msg.what = AudioToUIMessage::SLEW_RIGHT; msg.newValue = (float)slewRight; audioToUI.push(msg);
        msg.what = AudioToUIMessage::PEAK_LEFT; msg.newValue = (float)sqrt(peakLeft); audioToUI.push(msg);
        msg.what = AudioToUIMessage::PEAK_RIGHT; msg.newValue = (float)sqrt(peakRight); audioToUI.push(msg);
        msg.what = AudioToUIMessage::RMS_LEFT; msg.newValue = (float)sqrt(sqrt(rmsLeft/rmsCount)); audioToUI.push(msg);
        msg.what = AudioToUIMessage::RMS_RIGHT; msg.newValue = (float)sqrt(sqrt(rmsRight/rmsCount)); audioToUI.push(msg);
        msg.what = AudioToUIMessage::ZERO_LEFT; msg.newValue = (float)longestZeroLeft; audioToUI.push(msg);
        msg.what = AudioToUIMessage::ZERO_RIGHT; msg.newValue = (float)longestZeroRight; audioToUI.push(msg);
        msg.what = AudioToUIMessage::INCREMENT; msg.newValue = 0.0f; audioToUI.push(msg);
        slewLeft = 0.0;
        slewRight = 0.0;
        peakLeft = 0.0;
        peakRight = 0.0;
        rmsLeft = 0.0;
        rmsRight = 0.0;
        zeroLeft = 0.0;
        zeroRight = 0.0;
        longestZeroLeft = 0.0;
        longestZeroRight = 0.0;
        rmsCount = 0;
    }
}

//==============================================================================

void PluginProcessor::processBlock (juce::AudioBuffer<double>& buffer, juce::MidiBuffer& midiMessages)
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
    if (!(getBus(false, 0)->isEnabled() && getBus(true, 0)->isEnabled())) return;
    auto mainOutput = getBusBuffer(buffer, false, 0); //if we have audio busses at all,
    auto mainInput = getBusBuffer(buffer, true, 0); //they're now mainOutput and mainInput.
    UIToAudioMessage uim;
    while (uiToAudio.pop(uim)) {
        switch (uim.what) {
        case UIToAudioMessage::NEW_VALUE: params[uim.which]->setValueNotifyingHost(params[uim.which]->convertTo0to1(uim.newValue)); break;
        case UIToAudioMessage::BEGIN_EDIT: params[uim.which]->beginChangeGesture(); break;
        case UIToAudioMessage::END_EDIT: params[uim.which]->endChangeGesture(); break;
        }
    } //Handle inbound messages from the UI thread
    
    double overallscale = 1.0;
    overallscale /= 44100.0;
    overallscale *= getSampleRate();
     
    auto gainL = params[KNOBHIP]->get();
    auto gainR = params[KNOBLOP]->get();
    auto gainX = params[KNOBAIR]->get();

    double rmsSize = (1881.0 / 44100.0)*getSampleRate(); //higher is slower with larger RMS buffers
    double zeroCrossScale = (1.0 / getSampleRate())*44100.0;

    int inFramesToProcess = buffer.getNumSamples(); //vst doesn't give us this as a separate variable so we'll make it
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        auto outL = mainOutput.getWritePointer(0, i);
        auto outR = mainOutput.getWritePointer(1, i);
        auto inL = mainInput.getReadPointer(0, i); //in isBussesLayoutSupported, we have already
        auto inR = mainInput.getReadPointer(1, i); //specified that we can only be stereo and never mono
        double inputSampleL = *inL;
        double inputSampleR = *inR;
        if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
        if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
        //double drySampleL = inputSampleL;
        //double drySampleR = inputSampleR;
        //double temp = (double)i/inFramesToProcess;
        //double gain = (gainA*(1.0-temp))+(gainB*temp);
        //NOTE: I don't yet know whether counting the buffer backwards means that gainA and gainB must be reversed.
        //If the audio flow is in fact reversed we must swap the temp and (1.0-temp) and I have done this provisionally
        //Original VST2 is counting DOWN with -- operator, but this counts UP with ++
        //If this doesn't create zipper noise on sine processing then it's correct

        //begin bar display section
        if ((fabs(*inL-previousLeft)/28000.0f)*getSampleRate() > slewLeft) slewLeft =  (fabs(*inL-previousLeft)/28000.0f)*getSampleRate();
        if ((fabs(*inR-previousRight)/28000.0f)*getSampleRate() > slewRight) slewRight = (fabs(*inR-previousRight)/28000.0f)*getSampleRate();
        previousLeft = *inL; previousRight = *inR; //slew measurement is NOT rectified
        double rectifiedL = fabs(*inL);
        double rectifiedR = fabs(*inR);
        if (rectifiedL > peakLeft) peakLeft = rectifiedL;
        if (rectifiedR > peakRight) peakRight = rectifiedR;
        rmsLeft += (rectifiedL * rectifiedL);
        rmsRight += (rectifiedR * rectifiedR);
        rmsCount++;
        zeroLeft += zeroCrossScale;
        if (longestZeroLeft < zeroLeft) longestZeroLeft = zeroLeft;
        if (wasPositiveL && *inL < 0.0) {
            wasPositiveL = false;
            zeroLeft = 0.0;
        } else if (!wasPositiveL && *inL > 0.0) {
            wasPositiveL = true;
            zeroLeft = 0.0;
        }
        zeroRight += zeroCrossScale;
        if (longestZeroRight < zeroRight) longestZeroRight = zeroRight;
        if (wasPositiveR && *inR < 0.0) {
            wasPositiveR = false;
            zeroRight = 0.0;
        } else if (!wasPositiveR && *inR > 0.0) {
            wasPositiveR = true;
            zeroRight = 0.0;
        }


        //begin 64 bit stereo floating point dither
        //int expon; frexp((double)inputSampleL, &expon);
        fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
        //inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
        //frexp((double)inputSampleR, &expon);
        fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
        //inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
        //end 64 bit stereo floating point dither

        *outL = inputSampleL;
        *outR = inputSampleR; //this is a meter. Raw pass-through
    }


    if (rmsCount > rmsSize)
    {
        AudioToUIMessage msg; //define the thing we're telling JUCE
        msg.what = AudioToUIMessage::SLEW_LEFT; msg.newValue = (float)slewLeft; audioToUI.push(msg);
        msg.what = AudioToUIMessage::SLEW_RIGHT; msg.newValue = (float)slewRight; audioToUI.push(msg);
        msg.what = AudioToUIMessage::PEAK_LEFT; msg.newValue = (float)sqrt(peakLeft); audioToUI.push(msg);
        msg.what = AudioToUIMessage::PEAK_RIGHT; msg.newValue = (float)sqrt(peakRight); audioToUI.push(msg);
        msg.what = AudioToUIMessage::RMS_LEFT; msg.newValue = (float)sqrt(sqrt(rmsLeft/rmsCount)); audioToUI.push(msg);
        msg.what = AudioToUIMessage::RMS_RIGHT; msg.newValue = (float)sqrt(sqrt(rmsRight/rmsCount)); audioToUI.push(msg);
        msg.what = AudioToUIMessage::ZERO_LEFT; msg.newValue = (float)longestZeroLeft; audioToUI.push(msg);
        msg.what = AudioToUIMessage::ZERO_RIGHT; msg.newValue = (float)longestZeroRight; audioToUI.push(msg);
        msg.what = AudioToUIMessage::INCREMENT; msg.newValue = 0.0f; audioToUI.push(msg);
        slewLeft = 0.0;
        slewRight = 0.0;
        peakLeft = 0.0;
        peakRight = 0.0;
        rmsLeft = 0.0;
        rmsRight = 0.0;
        zeroLeft = 0.0;
        zeroRight = 0.0;
        longestZeroLeft = 0.0;
        longestZeroRight = 0.0;
        rmsCount = 0;
    }
}

