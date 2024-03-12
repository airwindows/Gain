#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#if (MSVC)
#include "ipps.h"
#endif

class PluginProcessor : public juce::AudioProcessor
{
public:
    PluginProcessor();
    ~PluginProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //now we can declare variables used in the audio thread
    
    enum Parameters
    {
        GAIN,
        CUTOFF,
        RESONANCE,
        n_params
    };
    std::array<juce::AudioParameterFloat *, n_params> params;
    //This is where we're defining things that go into the plugin's interface.
    
    struct UIToAudioMessage
    {
        enum What
        {
            NEW_VALUE,
            BEGIN_EDIT,
            END_EDIT
        } what{NEW_VALUE};
        Parameters which;
        float newValue = 0.0;
    };
    //This is things the interface can tell the audio thread about.
    
    struct AudioToUIMessage
    {
        enum What
        {
            NEW_VALUE,
            RMS_LEFT,
            RMS_RIGHT,
            PEAK_LEFT,
            PEAK_RIGHT,
            SLEW_LEFT,
            SLEW_RIGHT,
            INCREMENT
        } what{NEW_VALUE};
        Parameters which;
        float newValue = 0.0;
    };
    //This is kinds of information the audio thread can give the interface.
    
    template <typename T, int qSize = 4096> class LockFreeQueue
    {
      public:
        LockFreeQueue() : af(qSize) {}
        bool push(const T &ad)
        {
            auto ret = false;
            int start1, size1, start2, size2;
            af.prepareToWrite(1, start1, size1, start2, size2);
            if (size1 > 0)
            {
                dq[start1] = ad;
                ret = true;
            }
            af.finishedWrite(size1 + size2);
            return ret;
        }
        bool pop(T &ad)
        {
            bool ret = false;
            int start1, size1, start2, size2;
            af.prepareToRead(1, start1, size1, start2, size2);
            if (size1 > 0)
            {
                ad = dq[start1];
                ret = true;
            }
            af.finishedRead(size1 + size2);
            return ret;
        }
        juce::AbstractFifo af;
        T dq[qSize];
    };
    
    LockFreeQueue<UIToAudioMessage> uiToAudio;
    LockFreeQueue<AudioToUIMessage> audioToUI;


    float rmsLeft = 0.0;
    float rmsRight = 0.0;
    float peakLeft = 0.0;
    float peakRight = 0.0;
    float slewLeft = 0.0;
    float slewRight = 0.0;
    float previousLeft = 0.0;
    float previousRight = 0.0;
    int rmsCount = 0;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};
