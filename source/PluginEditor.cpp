#include "PluginEditor.h"

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    
    idleTimer = std::make_unique<IdleTimer>(this);
    idleTimer->startTimer(1000/30); //space between UI screen updates. Larger is slower updates to screen

    tsdisplay = std::make_unique<TimeSeriesDisplay>();
    tsdisplay->setBounds(2, 2, 1200, 200);
    addAndMakeVisible(*tsdisplay);


    addAndMakeVisible (inspectButton);

    // this chunk of code instantiates and opens the melatonin inspector
    inspectButton.onClick = [&] {
        if (!inspector)
        {
            inspector = std::make_unique<melatonin::Inspector> (*this);
            inspector->onClose = [this]() { inspector.reset(); };
        }

        inspector->setVisible (true);
    };

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1204, 204);
    //setResizable(false, false);
}

PluginEditor::~PluginEditor()
{
}

void PluginEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    auto area = getLocalBounds();
    g.setColour (juce::Colours::white);
    g.setFont (16.0f);
    auto helloWorld = juce::String ("Hello says ") + PRODUCT_NAME_WITHOUT_VERSION + " running in " + CMAKE_BUILD_TYPE;
    g.drawText (helloWorld, area.removeFromTop (150), juce::Justification::centred, false);
}

void PluginEditor::resized()
{
    // layout the positions of your child components here
    auto area = getLocalBounds();
    area.removeFromBottom(50);
    inspectButton.setBounds (getLocalBounds().withSizeKeepingCentre(100, 50));
}

void PluginEditor::idle()
{
    PluginProcessor::AudioToUIMessage msg;
    bool repaintTS{false};
    while (processorRef.audioToUI.pop(msg))
    {
        switch (msg.what)
        {
        case PluginProcessor::AudioToUIMessage::NEW_VALUE:
            sliders[msg.which]->setValue(msg.newValue,
                                         juce::NotificationType::dontSendNotification);
            break; //if we get a message that's a new control value, update the control.
                
        case PluginProcessor::AudioToUIMessage::RMS_LEFT:
                tsdisplay->pushRMSL(msg.newValue);
            break;  //if we get a RMS message, tell the TimeSeriesDisplay to push the new value.
                
        case PluginProcessor::AudioToUIMessage::RMS_RIGHT:
                tsdisplay->pushRMSR(msg.newValue);
            break;  //if we get a RMS message, tell the TimeSeriesDisplay to push the new value.
                
        case PluginProcessor::AudioToUIMessage::PEAK_LEFT:
                tsdisplay->pushPeakL(msg.newValue);
            break;  //if we get a RMS message, tell the TimeSeriesDisplay to push the new value.
                
        case PluginProcessor::AudioToUIMessage::PEAK_RIGHT:
                tsdisplay->pushPeakR(msg.newValue);
            break;  //if we get a RMS message, tell the TimeSeriesDisplay to push the new value.
                
        case PluginProcessor::AudioToUIMessage::SLEW_LEFT:
                tsdisplay->pushSlewL(msg.newValue);
            break;  //if we get a RMS message, tell the TimeSeriesDisplay to push the new value.
                
        case PluginProcessor::AudioToUIMessage::SLEW_RIGHT:
                tsdisplay->pushSlewR(msg.newValue);
            break;  //if we get a RMS message, tell the TimeSeriesDisplay to push the new value.
                
        case PluginProcessor::AudioToUIMessage::INCREMENT:
                tsdisplay->pushIncrement(msg.newValue);
            repaintTS = true;
            break;  //if we get a RMS message, tell the TimeSeriesDisplay to push the new value.
                    //this appears to only fire when JUCE is idle for a moment.
        default:
            std::cout << "Unhandled message type " << msg.what << std::endl;
            break;
        }
    }
    if (repaintTS)
        tsdisplay->repaint();
}
