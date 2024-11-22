#include "PluginEditor.h"

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    setResizable(true, true);
    setLookAndFeel(&airwindowsLookAndFeel);
    if (hostTrackColour != juce::Colour()) {
        airwindowsLookAndFeel.setColour(juce::ResizableWindow::backgroundColourId, hostTrackColour);
        airwindowsLookAndFeel.setColour(juce::Slider::thumbColourId, hostTrackColour);
    } else {
        airwindowsLookAndFeel.setColour(juce::ResizableWindow::backgroundColourId, airwindowsLookAndFeel.defaultColour);
        airwindowsLookAndFeel.setColour(juce::Slider::thumbColourId, airwindowsLookAndFeel.defaultColour);
    }
    updateTrackProperties();

    idleTimer = std::make_unique<IdleTimer>(this);
    idleTimer->startTimer(1000/30); //space between UI screen updates. Larger is slower updates to screen

    meter.setOpaque(true);
    meter.resetArrays();
    meter.displayTrackName = hostTrackName;
    meter.meterInterpolation = airwindowsLookAndFeel.alfInterpolation;
    addAndMakeVisible(meter);

    HIPKnob.setSliderStyle(juce::Slider::Rotary);
    HIPKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    HIPKnob.setRange(0.0f, 1.0f);
    HIPKnob.setValue(processorRef.params[PluginProcessor::KNOBHIP]->get(), juce::NotificationType::dontSendNotification);
    HIPKnob.addListener(this);
    addAndMakeVisible(HIPKnob);
    
    LOPKnob.setSliderStyle(juce::Slider::Rotary);
    LOPKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    LOPKnob.setRange(0.0f, 1.0f);
    LOPKnob.setValue(processorRef.params[PluginProcessor::KNOBLOP]->get(), juce::NotificationType::dontSendNotification);
    LOPKnob.addListener(this);
    addAndMakeVisible(LOPKnob);
    
    AIRKnob.setSliderStyle(juce::Slider::Rotary);
    AIRKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    AIRKnob.setRange(0.0f, 1.0f);
    AIRKnob.setValue(processorRef.params[PluginProcessor::KNOBAIR]->get(), juce::NotificationType::dontSendNotification);
    AIRKnob.addListener(this);
    addAndMakeVisible(AIRKnob);
    AIRKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.949f, 0.913f, 0.117f, 1.0f)); //fixed thumb color yellow

    FIRKnob.setSliderStyle(juce::Slider::Rotary);
    FIRKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    FIRKnob.setRange(0.0f, 1.0f);
    FIRKnob.setValue(processorRef.params[PluginProcessor::KNOBFIR]->get(), juce::NotificationType::dontSendNotification);
    FIRKnob.addListener(this);
    addAndMakeVisible(FIRKnob);
    FIRKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(1.0f, 0.486f, 0.129f, 1.0f)); //fixed thumb color orange

    STOKnob.setSliderStyle(juce::Slider::Rotary);
    STOKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    STOKnob.setRange(0.0f, 1.0f);
    STOKnob.setValue(processorRef.params[PluginProcessor::KNOBSTO]->get(), juce::NotificationType::dontSendNotification);
    STOKnob.addListener(this);
    addAndMakeVisible(STOKnob);
    STOKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.466f, 0.313f, 0.505f, 1.0f)); //fixed thumb color purple

    RNGKnob.setSliderStyle(juce::Slider::Rotary);
    RNGKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    RNGKnob.setRange(0.0f, 1.0f);
    RNGKnob.setValue(processorRef.params[PluginProcessor::KNOBRNG]->get(), juce::NotificationType::dontSendNotification);
    RNGKnob.addListener(this);
    addAndMakeVisible(RNGKnob);
    
    FCTKnob.setSliderStyle(juce::Slider::LinearHorizontal);
    FCTKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    FCTKnob.setRange(0.0f, 1.0f);
    FCTKnob.setValue(processorRef.params[PluginProcessor::KNOBFCT]->get(), juce::NotificationType::dontSendNotification);
    FCTKnob.addListener(this);
    addAndMakeVisible(FCTKnob);
    
    SCTKnob.setSliderStyle(juce::Slider::LinearHorizontal);
    SCTKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    SCTKnob.setRange(0.0f, 1.0f);
    SCTKnob.setValue(processorRef.params[PluginProcessor::KNOBSCT]->get(), juce::NotificationType::dontSendNotification);
    SCTKnob.addListener(this);
    addAndMakeVisible(SCTKnob);
    
    FCRKnob.setSliderStyle(juce::Slider::Rotary);
    FCRKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    FCRKnob.setRange(0.0f, 1.0f);
    FCRKnob.setValue(processorRef.params[PluginProcessor::KNOBFCR]->get(), juce::NotificationType::dontSendNotification);
    FCRKnob.addListener(this);
    addAndMakeVisible(FCRKnob);
    
    SCRKnob.setSliderStyle(juce::Slider::Rotary);
    SCRKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    SCRKnob.setRange(0.0f, 1.0f);
    SCRKnob.setValue(processorRef.params[PluginProcessor::KNOBSCR]->get(), juce::NotificationType::dontSendNotification);
    SCRKnob.addListener(this);
    addAndMakeVisible(SCRKnob);
    
    FCAKnob.setSliderStyle(juce::Slider::Rotary);
    FCAKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    FCAKnob.setRange(0.0f, 1.0f);
    FCAKnob.setValue(processorRef.params[PluginProcessor::KNOBFCA]->get(), juce::NotificationType::dontSendNotification);
    FCAKnob.addListener(this);
    addAndMakeVisible(FCAKnob);
    
    SCAKnob.setSliderStyle(juce::Slider::Rotary);
    SCAKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    SCAKnob.setRange(0.0f, 1.0f);
    SCAKnob.setValue(processorRef.params[PluginProcessor::KNOBSCA]->get(), juce::NotificationType::dontSendNotification);
    SCAKnob.addListener(this);
    addAndMakeVisible(SCAKnob);
    
    FCLKnob.setSliderStyle(juce::Slider::Rotary);
    FCLKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    FCLKnob.setRange(0.0f, 1.0f);
    FCLKnob.setValue(processorRef.params[PluginProcessor::KNOBFCL]->get(), juce::NotificationType::dontSendNotification);
    FCLKnob.addListener(this);
    addAndMakeVisible(FCLKnob);
    
    SCLKnob.setSliderStyle(juce::Slider::Rotary);
    SCLKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    SCLKnob.setRange(0.0f, 1.0f);
    SCLKnob.setValue(processorRef.params[PluginProcessor::KNOBSCL]->get(), juce::NotificationType::dontSendNotification);
    SCLKnob.addListener(this);
    addAndMakeVisible(SCLKnob);
    
    FGTKnob.setSliderStyle(juce::Slider::LinearHorizontal);
    FGTKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    FGTKnob.setRange(0.0f, 1.0f);
    FGTKnob.setValue(processorRef.params[PluginProcessor::KNOBFGT]->get(), juce::NotificationType::dontSendNotification);
    FGTKnob.addListener(this);
    addAndMakeVisible(FGTKnob);
    
    SGTKnob.setSliderStyle(juce::Slider::LinearHorizontal);
    SGTKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    SGTKnob.setRange(0.0f, 1.0f);
    SGTKnob.setValue(processorRef.params[PluginProcessor::KNOBSGT]->get(), juce::NotificationType::dontSendNotification);
    SGTKnob.addListener(this);
    addAndMakeVisible(SGTKnob);
    
    FGRKnob.setSliderStyle(juce::Slider::Rotary);
    FGRKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    FGRKnob.setRange(0.0f, 1.0f);
    FGRKnob.setValue(processorRef.params[PluginProcessor::KNOBFGR]->get(), juce::NotificationType::dontSendNotification);
    FGRKnob.addListener(this);
    addAndMakeVisible(FGRKnob);
    FGRKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.2f, 0.466f, 0.2f, 1.0f)); //fixed thumb color green

    SGRKnob.setSliderStyle(juce::Slider::Rotary);
    SGRKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    SGRKnob.setRange(0.0f, 1.0f);
    SGRKnob.setValue(processorRef.params[PluginProcessor::KNOBSGR]->get(), juce::NotificationType::dontSendNotification);
    SGRKnob.addListener(this);
    addAndMakeVisible(SGRKnob);
    SGRKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.2f, 0.466f, 0.2f, 1.0f)); //fixed thumb color green

    FGSKnob.setSliderStyle(juce::Slider::Rotary);
    FGSKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    FGSKnob.setRange(0.0f, 1.0f);
    FGSKnob.setValue(processorRef.params[PluginProcessor::KNOBFGS]->get(), juce::NotificationType::dontSendNotification);
    FGSKnob.addListener(this);
    addAndMakeVisible(FGSKnob);
    FGSKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.2f, 0.466f, 0.2f, 1.0f)); //fixed thumb color green

    SGSKnob.setSliderStyle(juce::Slider::Rotary);
    SGSKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    SGSKnob.setRange(0.0f, 1.0f);
    SGSKnob.setValue(processorRef.params[PluginProcessor::KNOBSGS]->get(), juce::NotificationType::dontSendNotification);
    SGSKnob.addListener(this);
    addAndMakeVisible(SGSKnob);
    SGSKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.2f, 0.466f, 0.2f, 1.0f)); //fixed thumb color green

    FGLKnob.setSliderStyle(juce::Slider::Rotary);
    FGLKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    FGLKnob.setRange(0.0f, 1.0f);
    FGLKnob.setValue(processorRef.params[PluginProcessor::KNOBFGL]->get(), juce::NotificationType::dontSendNotification);
    FGLKnob.addListener(this);
    addAndMakeVisible(FGLKnob);
    FGLKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.2f, 0.466f, 0.2f, 1.0f)); //fixed thumb color green

    SGLKnob.setSliderStyle(juce::Slider::Rotary);
    SGLKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    SGLKnob.setRange(0.0f, 1.0f);
    SGLKnob.setValue(processorRef.params[PluginProcessor::KNOBSGL]->get(), juce::NotificationType::dontSendNotification);
    SGLKnob.addListener(this);
    addAndMakeVisible(SGLKnob);
    SGLKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.2f, 0.466f, 0.2f, 1.0f)); //fixed thumb color green

    TRFKnob.setSliderStyle(juce::Slider::Rotary);
    TRFKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    TRFKnob.setRange(0.0f, 1.0f);
    TRFKnob.setValue(processorRef.params[PluginProcessor::KNOBTRF]->get(), juce::NotificationType::dontSendNotification);
    TRFKnob.addListener(this);
    addAndMakeVisible(TRFKnob);
    TRFKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.698f, 0.152f, 0.164f, 1.0f)); //fixed thumb color red
    
    TRGKnob.setSliderStyle(juce::Slider::Rotary);
    TRGKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    TRGKnob.setRange(0.0f, 1.0f);
    TRGKnob.setValue(processorRef.params[PluginProcessor::KNOBTRG]->get(), juce::NotificationType::dontSendNotification);
    TRGKnob.addListener(this);
    addAndMakeVisible(TRGKnob);
    TRGKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.698f, 0.152f, 0.164f, 1.0f)); //fixed thumb color red

    TRRKnob.setSliderStyle(juce::Slider::Rotary);
    TRRKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    TRRKnob.setRange(0.0f, 1.0f);
    TRRKnob.setValue(processorRef.params[PluginProcessor::KNOBTRR]->get(), juce::NotificationType::dontSendNotification);
    TRRKnob.addListener(this);
    addAndMakeVisible(TRRKnob);
    TRRKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.698f, 0.152f, 0.164f, 1.0f)); //fixed thumb color red

    HMFKnob.setSliderStyle(juce::Slider::Rotary);
    HMFKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    HMFKnob.setRange(0.0f, 1.0f);
    HMFKnob.setValue(processorRef.params[PluginProcessor::KNOBHMF]->get(), juce::NotificationType::dontSendNotification);
    HMFKnob.addListener(this);
    addAndMakeVisible(HMFKnob);
    HMFKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.2f, 0.466f, 0.2f, 1.0f)); //fixed thumb color green

    HMGKnob.setSliderStyle(juce::Slider::Rotary);
    HMGKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    HMGKnob.setRange(0.0f, 1.0f);
    HMGKnob.setValue(processorRef.params[PluginProcessor::KNOBHMG]->get(), juce::NotificationType::dontSendNotification);
    HMGKnob.addListener(this);
    addAndMakeVisible(HMGKnob);
    HMGKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.2f, 0.466f, 0.2f, 1.0f)); //fixed thumb color green

    HMRKnob.setSliderStyle(juce::Slider::Rotary);
    HMRKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    HMRKnob.setRange(0.0f, 1.0f);
    HMRKnob.setValue(processorRef.params[PluginProcessor::KNOBHMR]->get(), juce::NotificationType::dontSendNotification);
    HMRKnob.addListener(this);
    addAndMakeVisible(HMRKnob);
    HMRKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.2f, 0.466f, 0.2f, 1.0f)); //fixed thumb color green

    LMFKnob.setSliderStyle(juce::Slider::Rotary);
    LMFKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    LMFKnob.setRange(0.0f, 1.0f);
    LMFKnob.setValue(processorRef.params[PluginProcessor::KNOBLMF]->get(), juce::NotificationType::dontSendNotification);
    LMFKnob.addListener(this);
    addAndMakeVisible(LMFKnob);
    LMFKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.078f, 0.466f, 0.662f, 1.0f)); //fixed thumb color blue

    LMGKnob.setSliderStyle(juce::Slider::Rotary);
    LMGKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    LMGKnob.setRange(0.0f, 1.0f);
    LMGKnob.setValue(processorRef.params[PluginProcessor::KNOBLMG]->get(), juce::NotificationType::dontSendNotification);
    LMGKnob.addListener(this);
    addAndMakeVisible(LMGKnob);
    LMGKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.078f, 0.466f, 0.662f, 1.0f)); //fixed thumb color blue

    LMRKnob.setSliderStyle(juce::Slider::Rotary);
    LMRKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    LMRKnob.setRange(0.0f, 1.0f);
    LMRKnob.setValue(processorRef.params[PluginProcessor::KNOBLMR]->get(), juce::NotificationType::dontSendNotification);
    LMRKnob.addListener(this);
    addAndMakeVisible(LMRKnob);
    LMRKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.078f, 0.466f, 0.662f, 1.0f)); //fixed thumb color blue

    BSFKnob.setSliderStyle(juce::Slider::Rotary);
    BSFKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    BSFKnob.setRange(0.0f, 1.0f);
    BSFKnob.setValue(processorRef.params[PluginProcessor::KNOBBSF]->get(), juce::NotificationType::dontSendNotification);
    BSFKnob.addListener(this);
    addAndMakeVisible(BSFKnob);
    BSFKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.513f, 0.27f, 0.153f, 1.0f)); //fixed thumb color brown

    BSGKnob.setSliderStyle(juce::Slider::Rotary);
    BSGKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    BSGKnob.setRange(0.0f, 1.0f);
    BSGKnob.setValue(processorRef.params[PluginProcessor::KNOBBSG]->get(), juce::NotificationType::dontSendNotification);
    BSGKnob.addListener(this);
    addAndMakeVisible(BSGKnob);
    BSGKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.513f, 0.27f, 0.153f, 1.0f)); //fixed thumb color brown

    BSRKnob.setSliderStyle(juce::Slider::Rotary);
    BSRKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    BSRKnob.setRange(0.0f, 1.0f);
    BSRKnob.setValue(processorRef.params[PluginProcessor::KNOBBSR]->get(), juce::NotificationType::dontSendNotification);
    BSRKnob.addListener(this);
    addAndMakeVisible(BSRKnob);
    BSRKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.513f, 0.27f, 0.153f, 1.0f)); //fixed thumb color brown

    DSCKnob.setSliderStyle(juce::Slider::Rotary);
    DSCKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    DSCKnob.setRange(0.0f, 1.0f);
    DSCKnob.setValue(processorRef.params[PluginProcessor::KNOBDSC]->get(), juce::NotificationType::dontSendNotification);
    DSCKnob.addListener(this);
    addAndMakeVisible(DSCKnob);
    DSCKnob.setColour(juce::Slider::thumbColourId, juce::Colour().fromFloatRGBA(0.698f, 0.152f, 0.164f, 1.0f)); //fixed thumb color red
    
    PANKnob.setSliderStyle(juce::Slider::Rotary);
    PANKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    PANKnob.setRange(0.0f, 1.0f);
    PANKnob.setValue(processorRef.params[PluginProcessor::KNOBPAN]->get(), juce::NotificationType::dontSendNotification);
    PANKnob.addListener(this);
    addAndMakeVisible(PANKnob);
    
    FADKnob.setSliderStyle(juce::Slider::Rotary);
    FADKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    FADKnob.setRange(0.0f, 1.0f);
    FADKnob.setValue(processorRef.params[PluginProcessor::KNOBFAD]->get(), juce::NotificationType::dontSendNotification);
    FADKnob.addListener(this);
    addAndMakeVisible(FADKnob);
    
    /*addAndMakeVisible (inspectButton);
    inspectButton.onClick = [&] {
        if (!inspector)
        {
            inspector = std::make_unique<melatonin::Inspector> (*this);
            inspector->onClose = [this]() { inspector.reset(); };
        }
        inspector->setVisible (true);
    };// this chunk of code instantiates and opens the melatonin inspector*/
    
    setSize (850, 512);
    // Make sure that before the constructor has finished, you've set the editor's size to whatever you need it to be.
}

PluginEditor::~PluginEditor(){
    setLookAndFeel(nullptr);
}

void PluginEditor::paint (juce::Graphics& g)
{
    if (airwindowsLookAndFeel.blurImage == juce::Image()) {
        g.fillAll (airwindowsLookAndFeel.defaultColour);
        if (hostTrackColour != juce::Colour()) {
            g.setFillType(juce::FillType(hostTrackColour)); g.setOpacity(airwindowsLookAndFeel.applyTrackColour); g.fillAll();
        }
        airwindowsLookAndFeel.setColour(juce::ResizableWindow::backgroundColourId, airwindowsLookAndFeel.defaultColour.interpolatedWith (hostTrackColour, airwindowsLookAndFeel.applyTrackColour));
        airwindowsLookAndFeel.setColour(juce::Slider::thumbColourId, airwindowsLookAndFeel.defaultColour.interpolatedWith (hostTrackColour, airwindowsLookAndFeel.applyTrackColour));
    } else {
        if (airwindowsLookAndFeel.usingNamedImage) {
            g.drawImageWithin(airwindowsLookAndFeel.backgroundImage, 0, 0, getLocalBounds().getWidth(), getLocalBounds().getHeight(), 0);
        } else {
            g.setTiledImageFill(airwindowsLookAndFeel.backgroundImage, 0, 0, 1.0f); g.fillAll();
        }
                
        if (hostTrackColour != juce::Colour()) {
            g.setFillType(juce::FillType(hostTrackColour)); g.setOpacity(airwindowsLookAndFeel.applyTrackColour); g.fillAll();
        }
        airwindowsLookAndFeel.defaultColour = juce::Colour::fromRGBA(airwindowsLookAndFeel.blurImage.getPixelAt(1,1).getRed(),airwindowsLookAndFeel.blurImage.getPixelAt(1,1).getGreen(),airwindowsLookAndFeel.blurImage.getPixelAt(1,1).getBlue(),1.0);
        airwindowsLookAndFeel.setColour(juce::ResizableWindow::backgroundColourId, airwindowsLookAndFeel.defaultColour);
        airwindowsLookAndFeel.setColour(juce::Slider::thumbColourId, airwindowsLookAndFeel.defaultColour);
    } //find the color of the background tile or image, if there is one. Please use low-contrast stuff, but I'm not your mom :)
    
    g.setFont(juce::Font(airwindowsLookAndFeel.newFont, g.getCurrentFont().getHeight(), 0));
    auto linewidth = getLocalBounds().getWidth(); if (getLocalBounds().getHeight() > linewidth) linewidth = getLocalBounds().getHeight();  linewidth = (int)cbrt(linewidth/2)/2;
    if ((hostTrackName == juce::String()) || (hostTrackName.length() < 1.0f)) hostTrackName = juce::String("ConsoleX Channel");
    meter.displayTrackName = hostTrackName; //if not track name, then name of plugin. To be displayed on the actual peakmeter
    meter.displayFont = airwindowsLookAndFeel.newFont; //in the custom font, if we're using one
        
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::white, 0.618f));
    g.fillRect(0, 0, getLocalBounds().getWidth(), linewidth); g.fillRect(0, 0, linewidth, getLocalBounds().getHeight());
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.382f));
    g.fillRect(linewidth, getLocalBounds().getHeight()-linewidth, getLocalBounds().getWidth(), linewidth); g.fillRect(getLocalBounds().getWidth()-linewidth, linewidth, linewidth, getLocalBounds().getHeight()-linewidth);
    g.setColour (juce::Colours::black); g.drawRect(0, 0, getLocalBounds().getWidth(), getLocalBounds().getHeight());
    //draw global bevel effect, either from the color or from the color of the blurred texture, and a black border
}

void PluginEditor::resized()
{
    auto area = getLocalBounds(); // this is a huge huge routine, but not all of it runs at all times!
    auto linewidth = area.getWidth();
    float aspectRatio = (float)linewidth / (float)area.getHeight(); //Larger than 1: horisontal. Smaller than 1: vertical
    // 12h-1w = 0.11  (0.26)  6h-2w = 0.41   4h-3w = 1.0    3h-4w = 1.8    2h-6w = 3.85     1h-12w = 15.42
    if (area.getHeight() > linewidth) linewidth = area.getHeight();
    linewidth = (int)cbrt(linewidth/2)/2;
    area.reduce(linewidth, linewidth);
    meter.setBounds(area.getProportion(juce::Rectangle{((float)linewidth*2.0f)/area.getWidth(), (float)(linewidth*2.0f)/area.getHeight(), 1.0f-(((float)linewidth*4.0f)/area.getWidth()), (float)((linewidth*8.0f)/area.getHeight())}));
    inspectButton.setBounds(0, 0, 30, 10);
    area.removeFromTop((int)(linewidth*8.0f)); //remaining area is for controls. getProportion sets first start X and Y placement, then size X and Y placement
    float smallKnob = 0.4f; float medKnob = 0.5f; float largeKnob = 0.89f; float hugeKnob = 0.98f; float sliderW = 0.98f; float sliderH = 0.3f;
    //these are knob sizes scaled to the size of the block
    float skew = airwindowsLookAndFeel.applyTilt; //this is the amount of tilt the knobs experience at top and bottom. MAX 0.5 becomes full tilt
    
    float A1x = 0.06f; float A1y = 0.02f; float A2x = 0.05f; float A3x = 0.55f; float A23y = 0.66f;
    float B1x = 0.01f; float B1y = 0.2f; float B2x = 0.55f; float B2y = 0.01f; //knobs go large to small, then top to bottom, left to right
    float C1x = 0.1f; float C1y = 0.3f; float C2x = 0.01f; float C2y = 0.01f; //threshold slider is biggest, gets to be first
    //###now do the three knob and slider ones. 1 is always the slider, 234y are the same
    float D1x = 0.01f; float D1y = 0.1f; float D2x = 0.01f; float D3x = 0.33f; float D4x = 0.66f; float D234y = 0.45f;
    float E1x = 0.01f; float E1y = 0.5f; float E2x = 0.01f; float E3x = 0.33f; float E4x = 0.66f; float E234y = 0.15f;
    //these are always the same for all aspect ratios, but panels and offsets are unique to the aspect ratios, as are the sequence of control blocks
    
    if (aspectRatio >= 0.0f && aspectRatio < 0.18f) { //12h-1w
        float pY = (float)area.getHeight()/12.0f; //size of each movable block
        float pX = (float)area.getWidth()/1.0f;
        float offsetY = 0.0f; float offsetX = 0.0f; float panelTilt = 0.5f; //update the new panel tilt each time offsetY is updated. 0.0 is top panel, seen from underneath. 1.0 is bottom panel, seen from above
        FCTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D1x))/area.getWidth(), (offsetY+(pY*D1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH)/area.getHeight()}));
        FCRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D2x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        FCAKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCAKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D3x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        FCLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D4x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()})); //FireComp Thresh Ratio Attack Release D block
        offsetY = pY; //put between vertical SECTIONS (not knobs)
        FIRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FIRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*hugeKnob)/area.getWidth(), (pY*hugeKnob)/area.getHeight()}));
        AIRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        AIRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*medKnob)/area.getWidth(), (pY*medKnob)/area.getHeight()})); //Fire Air B block
        offsetY = pY*2.0f; //put between vertical SECTIONS (not knobs)
        FGTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E1x))/area.getWidth(), (offsetY+(pY*E1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH)/area.getHeight()}));
        FGRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E2x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        FGSKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGSKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E3x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        FGLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E4x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()})); //FireGate Thresh Ratio Sustain Release E block
        offsetY = pY*3.0f; //put between vertical SECTIONS (not knobs)
        SCTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D1x))/area.getWidth(), (offsetY+(pY*D1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH)/area.getHeight()}));
        SCRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D2x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        SCAKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCAKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D3x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        SCLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D4x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()})); //StoneComp Thresh Ratio Attack Release D block
        offsetY = pY*4.0f; //put between vertical SECTIONS (not knobs)
        STOKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        STOKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*hugeKnob)/area.getWidth(), (pY*hugeKnob)/area.getHeight()}));
        RNGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        RNGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*medKnob)/area.getWidth(), (pY*medKnob)/area.getHeight()})); //Stone Range B block
        offsetY = pY*5.0f; //put between vertical SECTIONS (not knobs)
        SGTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E1x))/area.getWidth(), (offsetY+(pY*E1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH)/area.getHeight()}));
        SGRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E2x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        SGSKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGSKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E3x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        SGLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E4x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()})); //StoneGate Thresh Ratio Sustain Release E block
        offsetY = pY*6.0f; //put between vertical SECTIONS (not knobs)
        LOPKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LOPKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob)/area.getHeight()}));
        HIPKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HIPKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        DSCKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        DSCKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()})); //Lowpass Highpass Discontinuity A block
        offsetY = pY*7.0f; //put between vertical SECTIONS (not knobs)
        TRGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob)/area.getHeight()}));
        TRRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        TRFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()})); //Treble Width Freq A block
        offsetY = pY*8.0f; //put between vertical SECTIONS (not knobs)
        HMGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob)/area.getHeight()}));
        HMRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        HMFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()})); //HighMid Width Freq A block
        offsetY = pY*9.0f; //put between vertical SECTIONS (not knobs)
        LMGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob)/area.getHeight()}));
        LMRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        LMFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()})); //LowMid Width Freq A block
        offsetY = pY*10.0f; //put between vertical SECTIONS (not knobs)
        BSGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob)/area.getHeight()}));
        BSRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        BSFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()})); //Bass Width Freq A block
        offsetY = pY*11.0f; //put between vertical SECTIONS (not knobs)
        FADKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FADKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*hugeKnob)/area.getWidth(), (pY*hugeKnob)/area.getHeight()}));
        PANKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        PANKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C2y))/area.getHeight(), (pX*medKnob)/area.getWidth(), (pY*medKnob)/area.getHeight()})); //Pan Fader C block
    } //12h-1w
    
    if (aspectRatio >= 0.18f && aspectRatio < 0.72f) { //6h-2w
        float pY = (float)area.getHeight()/6.0f; //size of each movable block
        float pX = (float)area.getWidth()/2.0f;
        float offsetY = 0.0f; float offsetX = 0.0f; float panelTilt = 0.5f-skew; //update the new panel tilt each time offsetY is updated. 0.0 is top panel, seen from underneath. 1.0 is bottom panel, seen from above
        FCTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D1x))/area.getWidth(), (offsetY+(pY*D1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH*(1.0f-(skew*0.5f)))/area.getHeight()}));
        FCRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D2x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        FCAKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCAKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D3x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        FCLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D4x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()})); //FireComp Thresh Ratio Attack Release D block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        TRGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        TRRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        TRFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()})); //Treble Width Freq A block
        
        offsetY = pY; offsetX = 0.0f; panelTilt = 0.5f-(skew*0.5f); //put between vertical SECTIONS (not knobs)
        FIRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FIRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*hugeKnob)/area.getWidth(), (pY*hugeKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        AIRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        AIRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*medKnob)/area.getWidth(), (pY*medKnob*(1.0f-(skew*0.25f)))/area.getHeight()})); //Fire Air B block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        HMGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        HMRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        HMFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.25f)))/area.getHeight()})); //HighMid Width Freq A block

        offsetY = pY*2.0f; offsetX = 0.0f; panelTilt = 0.5f-(skew*0.125f); //put between vertical SECTIONS (not knobs)
        FGTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E1x))/area.getWidth(), (offsetY+(pY*E1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH*(1.0f-(skew*0.125f)))/area.getHeight()}));
        FGRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E2x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.125f)))/area.getHeight()}));
        FGSKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGSKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E3x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.125f)))/area.getHeight()}));
        FGLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E4x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.125f)))/area.getHeight()})); //FireGate Thresh Ratio Sustain Release E block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        LMGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob*(1.0f-(skew*0.125f)))/area.getHeight()}));
        LMRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.125f)))/area.getHeight()}));
        LMFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.125f)))/area.getHeight()})); //LowMid Width Freq A block
 
        offsetY = pY*3.0f; offsetX = 0.0f; panelTilt = 0.5f+(skew*0.125f); //put between vertical SECTIONS (not knobs)
        SCTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D1x))/area.getWidth(), (offsetY+(pY*D1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH*(1.0f-(skew*0.125f)))/area.getHeight()}));
        SCRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D2x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.125f)))/area.getHeight()}));
        SCAKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCAKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D3x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.125f)))/area.getHeight()}));
        SCLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D4x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.125f)))/area.getHeight()})); //StoneComp Thresh Ratio Attack Release D block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        BSGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob*(1.0f-(skew*0.125f)))/area.getHeight()}));
        BSRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.125f)))/area.getHeight()}));
        BSFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.125f)))/area.getHeight()})); //Bass Width Freq A block

        offsetY = pY*4.0f; offsetX = 0.0f; panelTilt = 0.5f+(skew*0.5f); //put between vertical SECTIONS (not knobs)
        STOKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        STOKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*hugeKnob)/area.getWidth(), (pY*hugeKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        RNGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        RNGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*medKnob)/area.getWidth(), (pY*medKnob*(1.0f-(skew*0.25f)))/area.getHeight()})); //Stone Range B block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        LOPKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LOPKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        HIPKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HIPKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.25f)))/area.getHeight()}));
        DSCKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        DSCKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.25f)))/area.getHeight()})); //Lowpass Highpass Discontinuity A block

        offsetY = pY*5.0f; offsetX = 0.0f; panelTilt = 0.5f+skew; //put between vertical SECTIONS (not knobs)
        SGTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E1x))/area.getWidth(), (offsetY+(pY*E1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH)/area.getHeight()}));
        SGRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E2x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        SGSKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGSKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E3x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        SGLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E4x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()})); //StoneGate Thresh Ratio Sustain Release E block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        FADKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FADKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*hugeKnob)/area.getWidth(), (pY*hugeKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        PANKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        PANKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C2y))/area.getHeight(), (pX*medKnob)/area.getWidth(), (pY*medKnob*(1.0f-(skew*0.5f)))/area.getHeight()})); //Pan Fader C block
    } //6h-2w

    if (aspectRatio >= 0.72f && aspectRatio < 1.4f) { //4h-3w
        float pY = (float)area.getHeight()/4.0f; //size of each movable block
        float pX = (float)area.getWidth()/3.0f;
        float offsetY = 0.0f; float offsetX = 0.0f; float panelTilt = 0.5f-skew; //update the new panel tilt each time offsetY is updated. 0.0 is top panel, seen from underneath. 1.0 is bottom panel, seen from above
        TRGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        TRRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        TRFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()})); //Treble Width Freq A block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        SCTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D1x))/area.getWidth(), (offsetY+(pY*D1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH*(1.0f-(skew*0.5f)))/area.getHeight()}));
        SCRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D2x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        SCAKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCAKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D3x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        SCLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D4x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()})); //StoneComp Thresh Ratio Attack Release D block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        FCTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D1x))/area.getWidth(), (offsetY+(pY*D1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH*(1.0f-(skew*0.5f)))/area.getHeight()}));
        FCRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D2x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        FCAKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCAKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D3x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        FCLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D4x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()})); //FireComp Thresh Ratio Attack Release D block
        
        offsetY = (pY-(skew*0.5f)); offsetX = 0.0f; panelTilt = 0.5f-(skew*0.25f); //put between vertical SECTIONS (not knobs)
        HMGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob*(1.0f-(skew*0.125f)))/area.getHeight()}));
        HMRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.125f)))/area.getHeight()}));
        HMFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.125f)))/area.getHeight()})); //HighMid Width Freq A block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        STOKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        STOKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*hugeKnob)/area.getWidth(), (pY*hugeKnob*(1.0f-(skew*0.125f)))/area.getHeight()}));
        RNGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        RNGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*medKnob)/area.getWidth(), (pY*medKnob*(1.0f-(skew*0.125f)))/area.getHeight()})); //Stone Range B block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        FIRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FIRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*hugeKnob)/area.getWidth(), (pY*hugeKnob*(1.0f-(skew*0.125f)))/area.getHeight()}));
        AIRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        AIRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*medKnob)/area.getWidth(), (pY*medKnob*(1.0f-(skew*0.125f)))/area.getHeight()})); //Fire Air B block

        offsetY = (pY*2.0f); offsetX = 0.0f; panelTilt = 0.5f+(skew*0.25f); //put between vertical SECTIONS (not knobs)
        LMGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob*(1.0f-(skew*0.125f)))/area.getHeight()}));
        LMRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.125f)))/area.getHeight()}));
        LMFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.125f)))/area.getHeight()})); //LowMid Width Freq A block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        SGTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E1x))/area.getWidth(), (offsetY+(pY*E1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH*(1.0f-(skew*0.125f)))/area.getHeight()}));
        SGRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E2x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.125f)))/area.getHeight()}));
        SGSKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGSKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E3x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.125f)))/area.getHeight()}));
        SGLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E4x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.125f)))/area.getHeight()})); //StoneGate Thresh Ratio Sustain Release E block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        FGTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E1x))/area.getWidth(), (offsetY+(pY*E1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH*(1.0f-(skew*0.125f)))/area.getHeight()}));
        FGRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E2x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.125f)))/area.getHeight()}));
        FGSKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGSKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E3x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.125f)))/area.getHeight()}));
        FGLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E4x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.125f)))/area.getHeight()})); //FireGate Thresh Ratio Sustain Release E block
        
        offsetY = (pY*3.0f)+(skew*0.5f); offsetX = 0.0f; panelTilt = 0.5f+skew; //put between vertical SECTIONS (not knobs)
        BSGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        BSRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        BSFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()})); //Bass Width Freq A block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        LOPKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LOPKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        HIPKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HIPKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        DSCKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        DSCKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()})); //Lowpass Highpass Discontinuity A block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        FADKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FADKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*hugeKnob)/area.getWidth(), (pY*hugeKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        PANKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        PANKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C2y))/area.getHeight(), (pX*medKnob)/area.getWidth(), (pY*medKnob*(1.0f-(skew*0.5f)))/area.getHeight()})); //Pan Fader C block
    } //4h-3w
    
    if (aspectRatio >= 1.4f && aspectRatio < 3.0f) { //3h-4w
        float pY = (float)area.getHeight()/3.0f; //size of each movable block
        float pX = (float)area.getWidth()/4.0f;
        float offsetY = 0.0f; float offsetX = 0.0f; float panelTilt = 0.5f-skew; //update the new panel tilt each time offsetY is updated. 0.0 is top panel, seen from underneath. 1.0 is bottom panel, seen from above
        BSGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        BSRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        BSFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()})); //Bass Width Freq A block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        LMGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        LMRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        LMFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()})); //LowMid Width Freq A block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        HMGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        HMRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        HMFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()})); //HighMid Width Freq A block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        TRGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        TRRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        TRFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()})); //Treble Width Freq A block
        
        offsetY = pY-(skew*0.5f); offsetX = 0.0f; panelTilt = 0.5f; //put between vertical SECTIONS (not knobs)
        FCTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D1x))/area.getWidth(), (offsetY+(pY*D1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH)/area.getHeight()}));
        FCRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D2x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        FCAKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCAKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D3x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        FCLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D4x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()})); //FireComp Thresh Ratio Attack Release D block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        FIRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FIRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*hugeKnob)/area.getWidth(), (pY*hugeKnob)/area.getHeight()}));
        AIRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        AIRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*medKnob)/area.getWidth(), (pY*medKnob)/area.getHeight()})); //Fire Air B block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        FGTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E1x))/area.getWidth(), (offsetY+(pY*E1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH)/area.getHeight()}));
        FGRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E2x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        FGSKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGSKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E3x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        FGLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E4x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()})); //FireGate Thresh Ratio Sustain Release E block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        LOPKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LOPKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob)/area.getHeight()}));
        HIPKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HIPKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        DSCKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        DSCKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()})); //Lowpass Highpass Discontinuity A block
        
        offsetY = (pY*2.0f)+(skew*0.5f); offsetX = 0.0f; panelTilt = 0.5f+skew; //put between vertical SECTIONS (not knobs)
        SCTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D1x))/area.getWidth(), (offsetY+(pY*D1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH*(1.0f-(skew*0.5f)))/area.getHeight()}));
        SCRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D2x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        SCAKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCAKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D3x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        SCLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D4x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()})); //StoneComp Thresh Ratio Attack Release D block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        STOKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        STOKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*hugeKnob)/area.getWidth(), (pY*hugeKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        RNGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        RNGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*medKnob)/area.getWidth(), (pY*medKnob*(1.0f-(skew*0.5f)))/area.getHeight()})); //Stone Range B block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        SGTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E1x))/area.getWidth(), (offsetY+(pY*E1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH*(1.0f-(skew*0.5f)))/area.getHeight()}));
        SGRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E2x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        SGSKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGSKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E3x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        SGLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E4x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.5f)))/area.getHeight()})); //StoneGate Thresh Ratio Sustain Release E block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        FADKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FADKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*hugeKnob)/area.getWidth(), (pY*hugeKnob*(1.0f-(skew*0.5f)))/area.getHeight()}));
        PANKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        PANKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C2y))/area.getHeight(), (pX*medKnob)/area.getWidth(), (pY*medKnob*(1.0f-(skew*0.5f)))/area.getHeight()})); //Pan Fader C block
    } //3h-4w

    if (aspectRatio >= 3.0f && aspectRatio < 12.0f) { //2h-6w
        float pY = (float)area.getHeight()/2.0f; //size of each movable block
        float pX = (float)area.getWidth()/6.0f;
        float offsetY = 0.0f; float offsetX = 0.0f; float panelTilt = 0.5f-(skew*0.66f); //update the new panel tilt each time offsetY is updated. 0.0 is top panel, seen from underneath. 1.0 is bottom panel, seen from above
        SCTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D1x))/area.getWidth(), (offsetY+(pY*D1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH)/area.getHeight()}));
        SCRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D2x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.375f)))/area.getHeight()}));
        SCAKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCAKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D3x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.375f)))/area.getHeight()}));
        SCLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D4x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.375f)))/area.getHeight()})); //StoneComp Thresh Ratio Attack Release D block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        STOKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        STOKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*hugeKnob)/area.getWidth(), (pY*hugeKnob*(1.0f-(skew*0.375f)))/area.getHeight()}));
        RNGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        RNGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*medKnob)/area.getWidth(), (pY*medKnob*(1.0f-(skew*0.375f)))/area.getHeight()})); //Stone Range B block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        SGTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E1x))/area.getWidth(), (offsetY+(pY*E1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH*(1.0f-(skew*0.375f)))/area.getHeight()}));
        SGRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E2x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.375f)))/area.getHeight()}));
        SGSKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGSKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E3x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.375f)))/area.getHeight()}));
        SGLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E4x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.375f)))/area.getHeight()})); //StoneGate Thresh Ratio Sustain Release E block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        FCTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D1x))/area.getWidth(), (offsetY+(pY*D1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH*(1.0f-(skew*0.375f)))/area.getHeight()}));
        FCRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D2x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.375f)))/area.getHeight()}));
        FCAKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCAKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D3x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.375f)))/area.getHeight()}));
        FCLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D4x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.375f)))/area.getHeight()})); //FireComp Thresh Ratio Attack Release D block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        FIRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FIRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*hugeKnob)/area.getWidth(), (pY*hugeKnob*(1.0f-(skew*0.375f)))/area.getHeight()}));
        AIRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        AIRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*medKnob)/area.getWidth(), (pY*medKnob*(1.0f-(skew*0.375f)))/area.getHeight()})); //Fire Air B block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        FGTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E1x))/area.getWidth(), (offsetY+(pY*E1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH*(1.0f-(skew*0.375f)))/area.getHeight()}));
        FGRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E2x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.375f)))/area.getHeight()}));
        FGSKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGSKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E3x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.375f)))/area.getHeight()}));
        FGLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E4x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.375f)))/area.getHeight()})); //FireGate Thresh Ratio Sustain Release E block

        offsetY = pY; offsetX = 0.0f; panelTilt = 0.5f+(skew*0.66f); //put between vertical SECTIONS (not knobs)
        BSGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob*(1.0f-(skew*0.375f)))/area.getHeight()}));
        BSRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.375f)))/area.getHeight()}));
        BSFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.375f)))/area.getHeight()})); //Bass Width Freq A block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        LMGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob*(1.0f-(skew*0.375f)))/area.getHeight()}));
        LMRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.375f)))/area.getHeight()}));
        LMFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.375f)))/area.getHeight()})); //LowMid Width Freq A block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        HMGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob*(1.0f-(skew*0.375f)))/area.getHeight()}));
        HMRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.375f)))/area.getHeight()}));
        HMFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.375f)))/area.getHeight()})); //HighMid Width Freq A block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        TRGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob*(1.0f-(skew*0.375f)))/area.getHeight()}));
        TRRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.375f)))/area.getHeight()}));
        TRFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.375f)))/area.getHeight()})); //Treble Width Freq A block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        LOPKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LOPKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob*(1.0f-(skew*0.375f)))/area.getHeight()}));
        HIPKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HIPKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.375f)))/area.getHeight()}));
        DSCKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        DSCKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob*(1.0f-(skew*0.375f)))/area.getHeight()})); //Lowpass Highpass Discontinuity A block
        offsetX += pX; //put between horizontal SECTIONS (not knobs)
        FADKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FADKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*hugeKnob)/area.getWidth(), (pY*hugeKnob*(1.0f-(skew*0.375f)))/area.getHeight()}));
        PANKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        PANKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C2y))/area.getHeight(), (pX*medKnob)/area.getWidth(), (pY*medKnob*(1.0f-(skew*0.375f)))/area.getHeight()})); //Pan Fader C block
     } //2h-6w
    
    if (aspectRatio >= 12.0f && aspectRatio < 999.0f) { //1h-12w
        float pY = (float)area.getHeight()/1.0f; //size of each movable block
        float pX = (float)area.getWidth()/12.0f;
        float offsetY = 0.0f; float offsetX = 0.0f; float panelTilt = 0.5f; //update the new panel tilt each time offsetY is updated. 0.0 is top panel, seen from underneath. 1.0 is bottom panel, seen from above
        FCTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D1x))/area.getWidth(), (offsetY+(pY*D1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH)/area.getHeight()}));
        FCRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D2x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        FCAKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCAKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D3x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        FCLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FCLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D4x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()})); //FireComp Thresh Ratio Attack Release D block
        offsetX = pX; //put between vertical SECTIONS (not knobs)
        FIRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FIRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*hugeKnob)/area.getWidth(), (pY*hugeKnob)/area.getHeight()}));
        AIRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        AIRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*medKnob)/area.getWidth(), (pY*medKnob)/area.getHeight()})); //Fire Air B block
        offsetX = pX*2.0f; //put between vertical SECTIONS (not knobs)
        FGTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E1x))/area.getWidth(), (offsetY+(pY*E1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH)/area.getHeight()}));
        FGRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E2x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        FGSKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGSKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E3x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        FGLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FGLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E4x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()})); //FireGate Thresh Ratio Sustain Release E block
        offsetX = pX*3.0f; //put between vertical SECTIONS (not knobs)
        SCTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D1x))/area.getWidth(), (offsetY+(pY*D1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH)/area.getHeight()}));
        SCRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D2x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        SCAKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCAKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D3x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        SCLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SCLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*D4x))/area.getWidth(), (offsetY+(pY*D234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()})); //StoneComp Thresh Ratio Attack Release D block
        offsetX = pX*4.0f; //put between vertical SECTIONS (not knobs)
        STOKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        STOKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B1x))/area.getWidth(), (offsetY+(pY*B1y))/area.getHeight(), (pX*hugeKnob)/area.getWidth(), (pY*hugeKnob)/area.getHeight()}));
        RNGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        RNGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*B2x))/area.getWidth(), (offsetY+(pY*B2y))/area.getHeight(), (pX*medKnob)/area.getWidth(), (pY*medKnob)/area.getHeight()})); //Stone Range B block
        offsetX = pX*5.0f; //put between vertical SECTIONS (not knobs)
        SGTKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E1x))/area.getWidth(), (offsetY+(pY*E1y))/area.getHeight(), (pX*sliderW)/area.getWidth(), (pY*sliderH)/area.getHeight()}));
        SGRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E2x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        SGSKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGSKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E3x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        SGLKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        SGLKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*E4x))/area.getWidth(), (offsetY+(pY*E234y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()})); //StoneGate Thresh Ratio Sustain Release E block
        offsetX = pX*6.0f; //put between vertical SECTIONS (not knobs)
        LOPKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LOPKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob)/area.getHeight()}));
        HIPKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HIPKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        DSCKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        DSCKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()})); //Lowpass Highpass Discontinuity A block
        offsetX = pX*7.0f; //put between vertical SECTIONS (not knobs)
        TRGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob)/area.getHeight()}));
        TRRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        TRFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        TRFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()})); //Treble Width Freq A block
        offsetX = pX*8.0f; //put between vertical SECTIONS (not knobs)
        HMGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob)/area.getHeight()}));
        HMRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        HMFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        HMFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()})); //HighMid Width Freq A block
        offsetX = pX*9.0f; //put between vertical SECTIONS (not knobs)
        LMGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob)/area.getHeight()}));
        LMRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        LMFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        LMFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()})); //LowMid Width Freq A block
        offsetX = pX*10.0f; //put between vertical SECTIONS (not knobs)
        BSGKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSGKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A1x))/area.getWidth(), (offsetY+(pY*A1y))/area.getHeight(), (pX*largeKnob)/area.getWidth(), (pY*largeKnob)/area.getHeight()}));
        BSRKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSRKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A2x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()}));
        BSFKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        BSFKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*A3x))/area.getWidth(), (offsetY+(pY*A23y))/area.getHeight(), (pX*smallKnob)/area.getWidth(), (pY*smallKnob)/area.getHeight()})); //Bass Width Freq A block
        offsetX = pX*11.0f; //put between vertical SECTIONS (not knobs)
        FADKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        FADKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C1x))/area.getWidth(), (offsetY+(pY*C1y))/area.getHeight(), (pX*hugeKnob)/area.getWidth(), (pY*hugeKnob)/area.getHeight()}));
        PANKnob.setColour(juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(panelTilt, 0.0f, 0.0f, 1.0f));
        PANKnob.setBounds(area.getProportion(juce::Rectangle{(offsetX+(pX*C2x))/area.getWidth(), (offsetY+(pY*C2y))/area.getHeight(), (pX*medKnob)/area.getWidth(), (pY*medKnob)/area.getHeight()})); //Pan Fader C block
    } //1h-12w

}

void PluginEditor::sliderValueChanged(juce::Slider *s)
{
    if (s == &HIPKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBHIP;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &LOPKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBLOP;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &AIRKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBAIR;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
}

void PluginEditor::sliderDragStarted(juce::Slider *s) {sliderDragInternal(s, true);}
void PluginEditor::sliderDragEnded(juce::Slider *s) {sliderDragInternal(s, false);}
void PluginEditor::sliderDragInternal(juce::Slider *s, bool bv)
{
    if (s == &HIPKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBHIP;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &LOPKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBLOP;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &AIRKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBAIR;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FIRKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFIR;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &STOKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBSTO;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &RNGKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBRNG;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FCTKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFCT;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &SCTKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBSCT;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FCRKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFCR;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &SCRKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBSCR;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FCAKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFCA;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &SCAKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBSCA;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FCLKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFCL;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &SCLKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBSCL;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FGTKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFGT;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &SGTKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBSGT;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FGRKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFGR;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &SGRKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBSGR;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FGSKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFGS;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &SGSKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBSGS;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FGLKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFGL;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &SGLKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBSGL;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &TRFKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBTRF;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &TRGKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBTRG;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &TRRKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBTRR;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &HMFKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBHMF;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &HMGKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBHMG;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &HMRKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBHMR;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &LMFKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBLMF;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &LMGKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBLMG;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &LMRKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBLMR;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &BSFKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBBSF;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &BSGKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBBSG;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &BSRKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBBSR;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &DSCKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBDSC;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &PANKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBPAN;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &FADKnob) {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBFAD;
        processorRef.uiToAudio.push(msg);
    }
}

void PluginEditor::updateTrackProperties() {hostTrackColour=processorRef.trackProperties.colour; hostTrackName=processorRef.trackProperties.name; repaint();}

void PluginEditor::idle()
{
    PluginProcessor::AudioToUIMessage msg;
    bool repaintTS{false}; //we don't redraw interface just for getting data into the GUI section
    while (processorRef.audioToUI.pop(msg)) {
        switch (msg.what) {
        case PluginProcessor::AudioToUIMessage::NEW_VALUE:
                if (msg.which == PluginProcessor::KNOBHIP) {HIPKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBLOP) {LOPKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBAIR) {AIRKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBFIR) {FIRKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBSTO) {STOKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBRNG) {RNGKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBFCT) {FCTKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBSCT) {SCTKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBFCR) {FCRKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBSCR) {SCRKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBFCA) {FCAKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBSCA) {SCAKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBFCL) {FCLKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBSCL) {SCLKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBFGT) {FGTKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBSGT) {SGTKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBFGR) {FGRKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBSGR) {SGRKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBFGS) {FGSKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBSGS) {SGSKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBFGL) {FGLKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBSGL) {SGLKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBTRF) {TRFKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBTRG) {TRGKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBTRR) {TRRKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBHMF) {HMFKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBHMG) {HMGKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBHMR) {HMRKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBLMF) {LMFKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBLMG) {LMGKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBLMR) {LMRKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBBSF) {BSFKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBBSG) {BSGKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBBSR) {BSRKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBDSC) {DSCKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBPAN) {PANKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBFAD) {FADKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                break; //this can grab the knobs away from the user! Should cause the knob to repaint, too.
                
        case PluginProcessor::AudioToUIMessage::SLEW_LEFT: meter.pushA(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::SLEW_RIGHT: meter.pushB(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::PEAK_LEFT: meter.pushC(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::PEAK_RIGHT: meter.pushD(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::RMS_LEFT: meter.pushE(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::RMS_RIGHT: meter.pushF(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::ZERO_LEFT: meter.pushG(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::ZERO_RIGHT: meter.pushH(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::INCREMENT: //Increment is running at 24 FPS and giving the above calculations
                meter.pushIncrement(); repaintTS = true; //we will repaint GUI after doing the following
                
                FCTKnob.setColour(juce::Slider::thumbColourId, juce::Colour::fromFloatRGBA (meter.inputRMSL*airwindowsLookAndFeel.LEDColour.getFloatRed(), meter.inputRMSL*airwindowsLookAndFeel.LEDColour.getFloatGreen(), meter.inputRMSL*airwindowsLookAndFeel.LEDColour.getFloatBlue(), 1.0f));
                
                SCTKnob.setColour(juce::Slider::thumbColourId, juce::Colour::fromFloatRGBA (meter.inputRMSR*airwindowsLookAndFeel.LEDColour.getFloatRed(), meter.inputRMSL*airwindowsLookAndFeel.LEDColour.getFloatGreen(), meter.inputRMSL*airwindowsLookAndFeel.LEDColour.getFloatBlue(), 1.0f));
                
                FGTKnob.setColour(juce::Slider::thumbColourId, juce::Colour::fromFloatRGBA (meter.inputRMSL*airwindowsLookAndFeel.LEDColour.getFloatRed(), meter.inputRMSL*airwindowsLookAndFeel.LEDColour.getFloatGreen(), meter.inputRMSL*airwindowsLookAndFeel.LEDColour.getFloatBlue(), 1.0f));
                
                SGTKnob.setColour(juce::Slider::thumbColourId, juce::Colour::fromFloatRGBA (meter.inputRMSR*airwindowsLookAndFeel.LEDColour.getFloatRed(), meter.inputRMSL*airwindowsLookAndFeel.LEDColour.getFloatGreen(), meter.inputRMSL*airwindowsLookAndFeel.LEDColour.getFloatBlue(), 1.0f));
                
                //User color LEDS are done like this: choose the same meter.data selection for each, and then the color will always be the user color
                //SGTKnob.setColour(juce::Slider::thumbColourId, juce::Colour::fromFloatRGBA (meter.inputRMSL, meter.inputRMSL, meter.inputRMSL, 1.0f));
                //for an RGB or specified color blinken-knob, we don't reference user color, instead we just use the multiple meter.data directly without bringing in LEDColour
                
                //here is where we can make any control's thumb be a continuous blinkenlight with any value in meter. It runs at about 24fps.
                //We can do if statements etc. here, only thing we can NOT do is instantiate new variables. Do it as shown above and it works.
                //Also, this defaults to colors flashing against black, like LEDs: that's a good way to distinguish blinken-knobs from knobs
                break;
        default: break;
        } //end of switch statement for msg.what
    } if (repaintTS) meter.repaint();
}
