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
    addAndMakeVisible(meter);

    gainKnob.setSliderStyle(juce::Slider::LinearHorizontal);
    gainKnob.setSliderStyle(juce::Slider::TwoValueHorizontal); //see below: you can get a min and max value out of this, but it's gonna be inside the slider where it forces them to be only certain values
    gainKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    gainKnob.setRange(0.f, 1.f);
    gainKnob.setValue(processorRef.params[PluginProcessor::KNOBA]->get(), juce::NotificationType::dontSendNotification);
    gainKnob.addListener(this);
    addAndMakeVisible(gainKnob);
    
    cutoffKnob.setSliderStyle(juce::Slider::Rotary);
    cutoffKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    cutoffKnob.setRange(0.f, 1.f);
    cutoffKnob.setValue(processorRef.params[PluginProcessor::KNOBB]->get(), juce::NotificationType::dontSendNotification);
    cutoffKnob.addListener(this);
    addAndMakeVisible(cutoffKnob);
    
    resonanceKnob.setSliderStyle(juce::Slider::Rotary);
    resonanceKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    resonanceKnob.setRange(0.f, 1.f);
    resonanceKnob.setValue(processorRef.params[PluginProcessor::KNOBC]->get(), juce::NotificationType::dontSendNotification);
    resonanceKnob.addListener(this);
    addAndMakeVisible(resonanceKnob);
    
    addAndMakeVisible (inspectButton);
    inspectButton.onClick = [&] {
        if (!inspector)
        {
            inspector = std::make_unique<melatonin::Inspector> (*this);
            inspector->onClose = [this]() { inspector.reset(); };
        }
        inspector->setVisible (true);
    };// this chunk of code instantiates and opens the melatonin inspector
    
    setSize (512, 512);
    // Make sure that before the constructor has finished, you've set the editor's size to whatever you need it to be.
    if (airwindowsLookAndFeel.usingNamedImage) {
        getConstrainer()->setFixedAspectRatio(512.0/512.0); //the aspect ratio stuff leads to cropping the content area off the top
        setResizeLimits(32, 32, 4196, 4196); //this will not honor resize limits correctly in all the DAWs
    }
}

PluginEditor::~PluginEditor(){}

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
    
    auto linewidth = getLocalBounds().getWidth(); if (getLocalBounds().getHeight() > linewidth) linewidth = getLocalBounds().getHeight();  linewidth = (int)cbrt(linewidth/2)/2;
    if ((hostTrackName == juce::String()) || (hostTrackName.length() < 1.0f)) hostTrackName = juce::String("JUCE test plugin"); //if not track name, then name of plugin
    float radius = getLocalBounds().getWidth(); if (radius > (getLocalBounds().getHeight())*0.0618f) radius = (getLocalBounds().getHeight())*0.0618f;
    auto embossScale = sqrt(sqrt(radius*0.618f)*1.618f)*0.618f; //this is customized to the needs of the plugin title text area
    g.setFont ((radius*12.0f) / (float)g.getCurrentFont().getHeight());
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::white, 0.75f)); //highlight
    g.drawFittedText(hostTrackName, juce::Rectangle<int>((int)(getLocalBounds().getWidth()+embossScale),(int)((getLocalBounds().getHeight()*0.0618f)+embossScale)), juce::Justification::centredBottom, 1);
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.75f)); //shadow
    g.drawFittedText(hostTrackName, juce::Rectangle<int>((int)(getLocalBounds().getWidth()-embossScale),(int)((getLocalBounds().getHeight()*0.0618f)-embossScale)), juce::Justification::centredBottom, 1);
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.25f)); //text inside emboss
    g.drawFittedText(hostTrackName, juce::Rectangle<int>((int)getLocalBounds().getWidth(),(int)(getLocalBounds().getHeight()*0.0618f)), juce::Justification::centredBottom, 1);
    //draw the track name or plugin name embossed. This is because there's an unlimited range of colors and textures that could be in play.
    
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::white, 0.618f));
    g.fillRect(0, 0, getLocalBounds().getWidth(), linewidth); g.fillRect(0, 0, linewidth, getLocalBounds().getHeight());
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.382f));
    g.fillRect(linewidth, getLocalBounds().getHeight()-linewidth, getLocalBounds().getWidth(), linewidth); g.fillRect(getLocalBounds().getWidth()-linewidth, linewidth, linewidth, getLocalBounds().getHeight()-linewidth);
    g.setColour (juce::Colours::black); g.drawRect(0, 0, getLocalBounds().getWidth(), getLocalBounds().getHeight());
    //draw global bevel effect, either from the color or from the color of the blurred texture, and a black border
}

void PluginEditor::resized()
{
    auto area = getLocalBounds();
    auto linewidth = area.getWidth();
    if (area.getHeight() > linewidth) linewidth = area.getHeight();
    linewidth = (int)cbrt(linewidth/2)/2;
    area.reduce(linewidth, linewidth);
    //getProportion sets first start X and Y placement, then size X and Y placement

    //juce::Colour tiltColour; //controls tilt of knobs with red channel
    //this mechanism allows for more control area in a given screen space and a novel faux-3D effect
    //that still looks vector and is FULLY resizeable and reshapeable. Control labels are drawn
    //on the actual controls because otherwise they would follow the bounding box.
    //There's three more channels in this color, that could also pass in design data…
    //0.0 is top panel, seen from underneath
    //0.5 is facing view directly, flat like a normal GUI
    //1.0 is bottom panel, seen from above
    
    meter.setBounds(area.getProportion(juce::Rectangle{((float)linewidth*2.0f)/area.getWidth(), 0.05f, 1.0f-(((float)linewidth*4.0f)/area.getWidth()), 0.332f}));
    //including the text line on top, this extends to 0.382 of the space, leaving 0.618 for knobs
    
    cutoffKnob.setColour (juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA(1.0f-(float)gainKnob.getMaxValue(), 0.0f, 0.0f, 1.0f));
    cutoffKnob.setBounds(area.getProportion(juce::Rectangle{0.0f, 0.382f, 1.0f, 0.1f}));
    
    gainKnob.setColour (juce::Slider::backgroundColourId, juce::Colours::grey);
    gainKnob.setColour (juce::Slider::thumbColourId, juce::Colours::red);
    //gainKnob.setColour (juce::Slider::backgroundColourId, juce::Colours::grey);
    gainKnob.setBounds(area.getProportion(juce::Rectangle{0.0f, 0.482f, 1.0f, 0.4f}));
    
    resonanceKnob.setColour (juce::Slider::backgroundColourId, juce::Colour().fromFloatRGBA((float)gainKnob.getMinValue()*0.9f, 0.0f, 0.0f, 1.0f));
    resonanceKnob.setBounds(area.getProportion(juce::Rectangle{0.0f, 0.882f, 1.0f, 0.1f}));
    
    inspectButton.setBounds(area.getProportion(juce::Rectangle{0.01f, 0.01f, 0.03f, 0.03f}));
}

void PluginEditor::sliderValueChanged(juce::Slider *s)
{
    if (s == &gainKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBA;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &cutoffKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBB;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
    if (s == &resonanceKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = PluginProcessor::UIToAudioMessage::NEW_VALUE;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBC;
        msg.newValue = (float)s->getValue();
        processorRef.uiToAudio.push(msg);
    }
}

void PluginEditor::sliderDragStarted(juce::Slider *s) {sliderDragInternal(s, true);}
void PluginEditor::sliderDragEnded(juce::Slider *s) {sliderDragInternal(s, false);}
void PluginEditor::sliderDragInternal(juce::Slider *s, bool bv)
{
    if (s == &gainKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBA;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &cutoffKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBB;
        processorRef.uiToAudio.push(msg);
    }
    if (s == &resonanceKnob)
    {
        PluginProcessor::UIToAudioMessage msg;
        msg.what = bv ? PluginProcessor::UIToAudioMessage::BEGIN_EDIT : PluginProcessor::UIToAudioMessage::END_EDIT;
        msg.which = (PluginProcessor::Parameters)PluginProcessor::KNOBC;
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
                if (msg.which == PluginProcessor::KNOBA) {gainKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBB) {cutoffKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                if (msg.which == PluginProcessor::KNOBC) {resonanceKnob.setValue(msg.newValue, juce::NotificationType::dontSendNotification); break;}
                break; //this can grab the knobs away from the user! Should cause the knob to repaint, too.
                
        case PluginProcessor::AudioToUIMessage::RMS_LEFT: meter.pushA(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::RMS_RIGHT: meter.pushB(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::PEAK_LEFT: meter.pushC(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::PEAK_RIGHT: meter.pushD(msg.newValue); break;
                
        case PluginProcessor::AudioToUIMessage::INCREMENT: //Increment is running at 24 FPS and giving the above calculations
                meter.pushIncrement(msg.newValue); repaintTS = true; //we will repaint GUI after doing the following
                
                gainKnob.setColour(juce::Slider::thumbColourId, juce::Colour::fromFloatRGBA (meter.dataC[meter.dataPosition-1]*airwindowsLookAndFeel.LEDColour.getFloatRed(), meter.dataC[meter.dataPosition-1]*airwindowsLookAndFeel.LEDColour.getFloatGreen(), meter.dataC[meter.dataPosition-1]*airwindowsLookAndFeel.LEDColour.getFloatBlue(), 1.0f));
                //User color LEDS are done like this: choose the same meter.data selection for each, and then the color will always be the user color
                //gainKnob.setColour(juce::Slider::thumbColourId, juce::Colour::fromFloatRGBA (meter.dataA[meter.dataPosition-1], meter.dataB[meter.dataPosition-1], meter.dataC[meter.dataPosition-1], 1.0f));
                //for an RGB or specified color blinken-knob, we don't reference user color, instead we just use the multiple meter.data directly without bringing in LEDColour
                
                cutoffKnob.setColour(juce::Slider::thumbColourId, juce::Colour::fromFloatRGBA (meter.dataD[meter.dataPosition-1]*airwindowsLookAndFeel.LEDColour.getFloatRed(), meter.dataD[meter.dataPosition-1]*airwindowsLookAndFeel.LEDColour.getFloatGreen(), meter.dataD[meter.dataPosition-1]*airwindowsLookAndFeel.LEDColour.getFloatBlue(), 1.0f));
                //User color LEDS are done like this: choose the same meter.data selection for each, and then the color will always be the user color
                //cutoffKnob.setColour(juce::Slider::thumbColourId, juce::Colour::fromFloatRGBA (meter.dataA[meter.dataPosition-1], meter.dataB[meter.dataPosition-1], meter.dataC[meter.dataPosition-1], 1.0f));
                //for an RGB or specified color blinken-knob, we don't reference user color, instead we just use the multiple meter.data directly without bringing in LEDColour

                //resonanceKnob.setColour(juce::Slider::thumbColourId, juce::Colour::fromFloatRGBA (meter.dataA[meter.dataPosition-1]*airwindowsLookAndFeel.LEDColour.getFloatRed(), meter.dataA[meter.dataPosition-1]*airwindowsLookAndFeel.LEDColour.getFloatGreen(), meter.dataA[meter.dataPosition-1]*airwindowsLookAndFeel.LEDColour.getFloatBlue(), 1.0f));
                //User color LEDS are done like this: choose the same meter.data selection for each, and then the color will always be the user color
                resonanceKnob.setColour(juce::Slider::thumbColourId, juce::Colour::fromFloatRGBA (meter.dataA[meter.dataPosition-1], meter.dataB[meter.dataPosition-1], meter.dataC[meter.dataPosition-1], 1.0f));
                //for an RGB or specified color blinken-knob, we don't reference user color, instead we just use the multiple meter.data directly without bringing in LEDColour
                
                //here is where we can make any control's thumb be a continuous blinkenlight with any value in meter. It runs at about 24fps.
                //We can do if statements etc. here, only thing we can NOT do is instantiate new variables. Do it as shown above and it works.
                //Also, this defaults to colors flashing against black, like LEDs: that's a good way to distinguish blinken-knobs from knobs
                break;
        default: break;
        } //end of switch statement for msg.what
    } if (repaintTS) meter.repaint();
}
