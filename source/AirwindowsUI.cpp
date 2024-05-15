// AirwindowsUI by Chris Johnson
// Initial seed code for the meter created by Paul Walker on 8/23/21.
#include "AirwindowsUI.h"


/*void AirwindowsLookAndFeel::drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                                       float minSliderPos,
                                       float maxSliderPos,
                                       const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    if (slider.isBar())
    {
        g.setColour (slider.findColour (juce::Slider::trackColourId));
        g.fillRect (slider.isHorizontal() ? Rectangle<float> (static_cast<float> (x), (float) y + 0.5f, sliderPos - (float) x, (float) height - 1.0f)
                                          : Rectangle<float> ((float) x + 0.5f, sliderPos, (float) width - 1.0f, (float) y + ((float) height - sliderPos)));

        drawLinearSliderOutline (g, x, y, width, height, style, slider);
    }
    else
    {
        auto isTwoVal   = (style == juce::Slider::SliderStyle::TwoValueVertical   || style == Slider::SliderStyle::TwoValueHorizontal);
        auto isThreeVal = (style == juce::Slider::SliderStyle::ThreeValueVertical || style == Slider::SliderStyle::ThreeValueHorizontal);

        auto trackWidth = jmin (6.0f, slider.isHorizontal() ? (float) height * 0.25f : (float) width * 0.25f);

        Point<float> startPoint (slider.isHorizontal() ? (float) x : (float) x + (float) width * 0.5f,
                                 slider.isHorizontal() ? (float) y + (float) height * 0.5f : (float) (height + y));

        Point<float> endPoint (slider.isHorizontal() ? (float) (width + x) : startPoint.x,
                               slider.isHorizontal() ? startPoint.y : (float) y);

        Path backgroundTrack;
        backgroundTrack.startNewSubPath (startPoint);
        backgroundTrack.lineTo (endPoint);
        g.setColour (slider.findColour (Slider::backgroundColourId));
        g.strokePath (backgroundTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });

        Path valueTrack;
        Point<float> minPoint, maxPoint, thumbPoint;

        if (isTwoVal || isThreeVal)
        {
            minPoint = { slider.isHorizontal() ? minSliderPos : (float) width * 0.5f,
                         slider.isHorizontal() ? (float) height * 0.5f : minSliderPos };

            if (isThreeVal)
                thumbPoint = { slider.isHorizontal() ? sliderPos : (float) width * 0.5f,
                               slider.isHorizontal() ? (float) height * 0.5f : sliderPos };

            maxPoint = { slider.isHorizontal() ? maxSliderPos : (float) width * 0.5f,
                         slider.isHorizontal() ? (float) height * 0.5f : maxSliderPos };
        }
        else
        {
            auto kx = slider.isHorizontal() ? sliderPos : ((float) x + (float) width * 0.5f);
            auto ky = slider.isHorizontal() ? ((float) y + (float) height * 0.5f) : sliderPos;

            minPoint = startPoint;
            maxPoint = { kx, ky };
        }

        auto thumbWidth = getSliderThumbRadius (slider);

        valueTrack.startNewSubPath (minPoint);
        valueTrack.lineTo (isThreeVal ? thumbPoint : maxPoint);
        g.setColour (slider.findColour (Slider::trackColourId));
        g.strokePath (valueTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });

        if (! isTwoVal)
        {
            g.setColour (slider.findColour (Slider::thumbColourId));
            g.fillEllipse (Rectangle<float> (static_cast<float> (thumbWidth), static_cast<float> (thumbWidth)).withCentre (isThreeVal ? thumbPoint : maxPoint));
        }

        if (isTwoVal || isThreeVal)
        {
            auto sr = jmin (trackWidth, (slider.isHorizontal() ? (float) height : (float) width) * 0.4f);
            auto pointerColour = slider.findColour (Slider::thumbColourId);

            if (slider.isHorizontal())
            {
                drawPointer (g, minSliderPos - sr,
                             jmax (0.0f, (float) y + (float) height * 0.5f - trackWidth * 2.0f),
                             trackWidth * 2.0f, pointerColour, 2);

                drawPointer (g, maxSliderPos - trackWidth,
                             jmin ((float) (y + height) - trackWidth * 2.0f, (float) y + (float) height * 0.5f),
                             trackWidth * 2.0f, pointerColour, 4);
            }
            else
            {
                drawPointer (g, jmax (0.0f, (float) x + (float) width * 0.5f - trackWidth * 2.0f),
                             minSliderPos - trackWidth,
                             trackWidth * 2.0f, pointerColour, 1);

                drawPointer (g, jmin ((float) (x + width) - trackWidth * 2.0f, (float) x + (float) width * 0.5f), maxSliderPos - sr,
                             trackWidth * 2.0f, pointerColour, 3);
            }
        }

        if (slider.isBar())
            drawLinearSliderOutline (g, x, y, width, height, style, slider);
    }
}
*/


void AirwindowsLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                                              const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
    auto tilt = (slider.findColour (juce::Slider::backgroundColourId).getFloatRed()-0.5f)*1.236f;
    auto textbounds = juce::Rectangle<int> (x, y, width, height).toFloat();
    auto bounds = textbounds; bounds.removeFromBottom(4.0f+(height*0.12f)+(height*tilt*tilt*0.12f)+(height*tilt*fabs(tilt)*0.12f)); bounds.reduce(1.0f, 1.0f);
    float scaleHeight = 1.0f-(fabs(tilt)); //proportion of vertical height relative to horizontal
    float trimscaleHeight = scaleHeight + (tilt*0.05f);
    float radius = bounds.getWidth()*0.5f; if (radius > (bounds.getHeight()/scaleHeight)*0.5f) radius = (bounds.getHeight()/scaleHeight)*0.5f;
    auto gradientSquare = juce::Rectangle(bounds.getCentreX()-radius, bounds.getCentreY()-(radius*sqrt(scaleHeight)), radius*2.0f, radius*sqrt(trimscaleHeight)*2.0f).toFloat();
    auto square = juce::Rectangle(bounds.getCentreX()-radius, bounds.getCentreY()-(radius*scaleHeight), radius*2.0f, radius*trimscaleHeight*2.0f).toFloat();
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto bevelW = sqrt(radius*0.5f)*1.618f;
    auto lineW = sqrt(bevelW)*0.55f;
    //basic variables we'll be using for our controls
    
    juce::ColourGradient cg = juce::ColourGradient(juce::Colours::white, gradientSquare.getTopLeft(), juce::Colours::black, gradientSquare.getBottomRight(),false);
    cg.addColour(0.2f, juce::Colours::white); cg.addColour(0.618f, juce::Colours::transparentBlack); cg.addColour(0.9f, juce::Colours::black); cg.isRadial = true;
    g.setGradientFill(cg);
    auto inset = square; inset.reduce(bevelW*0.25f, bevelW*0.25f); inset.removeFromTop(-bevelW*tilt*fabs(tilt+0.15f)*0.618f);
    g.drawEllipse (inset, bevelW*0.5f);
    cg = juce::ColourGradient(juce::Colours::transparentWhite, gradientSquare.getTopLeft(), juce::Colours::black, gradientSquare.getBottomRight(),false);
    cg.addColour(0.0955f, juce::Colours::white); cg.addColour(0.382f, slider.findColour (juce::ResizableWindow::backgroundColourId)); cg.addColour(0.618f, slider.findColour (juce::ResizableWindow::backgroundColourId)); cg.isRadial = true;
    g.setGradientFill(cg);
    inset.reduce(bevelW*0.25f, bevelW*0.25f); inset.removeFromTop(-bevelW*tilt*fabs(tilt+0.15f)*0.618f);
    g.drawEllipse (inset, bevelW*0.382f);
    cg = juce::ColourGradient(juce::Colours::transparentWhite, gradientSquare.getTopLeft(), juce::Colours::transparentBlack, gradientSquare.getBottomRight(),false);
    cg.addColour(0.04775f, juce::Colours::transparentWhite); cg.addColour(0.382f, slider.findColour (juce::ResizableWindow::backgroundColourId)); cg.addColour(0.618f, slider.findColour (juce::ResizableWindow::backgroundColourId)); cg.isRadial = true;
    g.setGradientFill(cg);
    inset.reduce(bevelW*0.382f, bevelW*0.382f); inset.removeFromTop(-bevelW*tilt*fabs(tilt+0.15f)*0.618f);
    g.drawEllipse (inset, bevelW*0.618f); g.setColour (juce::Colours::black); g.drawEllipse (square, lineW);
    //This is the outside circle of the knob, with the shading/highlighting that renders the 3D effect. Tilting of the knob is included
    
    g.setFont ((((lineW+bevelW)*30.0f) / (float)g.getCurrentFont().getHeight()) + (tilt*0.25f));
    auto padHeight = (sqrt(bevelW)*-2.0f) + ((bounds.getHeight())-(bounds.getWidth()*scaleHeight)) + (tilt*bevelW) + (fabs(tilt)*-bevelW); if (padHeight < 0.0) padHeight = 0.0;
    textbounds.removeFromBottom(padHeight*0.618f);
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::white, 0.75f)); //highlight
    g.drawFittedText(slider.getName(), juce::Rectangle<int>((int)(textbounds.getWidth()+lineW),(int)(textbounds.getHeight()+lineW)), juce::Justification::centredBottom, 1);
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.75f)); //shadow
    g.drawFittedText(slider.getName(), juce::Rectangle<int>((int)(textbounds.getWidth()-lineW),(int)(textbounds.getHeight()-lineW)), juce::Justification::centredBottom, 1);
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.25f)); //text inside emboss
    g.drawFittedText(slider.getName(), juce::Rectangle<int>((int)textbounds.getWidth(),(int)textbounds.getHeight()), juce::Justification::centredBottom, 1);
    //This is the drawing of the text just under the knob, no matter what tilt setting is included and what the real bounding box is doing. That's why it's so tricky :)

    auto arcRadius = (radius-(bevelW*0.5f))*0.7f;
    auto thumbWidth = pow(bevelW,1.618f)*0.7f;
    juce::Point<float> thumbPoint (bounds.getCentreX()+(arcRadius*std::cos(toAngle-juce::MathConstants<float>::halfPi)),bounds.getCentreY()+(-tilt*bevelW)+(arcRadius*trimscaleHeight*std::sin(toAngle-juce::MathConstants<float>::halfPi)));
    float thumbScale = 1.0f+(std::sin(toAngle-juce::MathConstants<float>::halfPi)*tilt*fabs(tilt)*0.5f);
    square = juce::Rectangle<float> (thumbWidth*thumbScale, thumbWidth*trimscaleHeight*thumbScale).withCentre (thumbPoint);
    gradientSquare = juce::Rectangle<float> (thumbWidth*trimscaleHeight*thumbScale, thumbWidth*thumbScale).withCentre (thumbPoint);
    g.setColour (slider.findColour (juce::Slider::thumbColourId)); g.fillEllipse (square);
    cg = juce::ColourGradient(juce::Colours::white, gradientSquare.getBottomRight(), juce::Colours::black, gradientSquare.getTopLeft(),false);
    cg.addColour(0.191f, juce::Colours::white); cg.addColour(0.382f, slider.findColour (juce::Slider::thumbColourId)); cg.addColour(0.618f, slider.findColour (juce::Slider::thumbColourId)); cg.isRadial = true;
    g.setGradientFill(cg);
    inset = square; inset.reduce(bevelW*0.382f, bevelW*0.382f); inset.removeFromBottom(bevelW*tilt*fabs(tilt+0.15f)*0.125f);
    g.drawEllipse (inset, bevelW*0.5f);
    cg = juce::ColourGradient(juce::Colours::white, gradientSquare.getBottomRight(), juce::Colours::black, gradientSquare.getTopLeft(),false);
    cg.addColour(0.0955f, juce::Colours::transparentWhite); cg.addColour(0.382f, slider.findColour (juce::Slider::thumbColourId)); cg.addColour(0.618f, slider.findColour (juce::Slider::thumbColourId)); cg.isRadial = true;
    g.setGradientFill(cg);
    inset.reduce(bevelW*0.125f, bevelW*0.125f); inset.removeFromBottom(bevelW*tilt*fabs(tilt+0.15f)*0.125f);
    g.drawEllipse (inset, bevelW*0.5f); g.setColour (juce::Colours::black); g.drawEllipse (square, lineW);
    //This is the thumb of the knob, also rendered on a tilt if needed, and allowing a custom color to the thumb
    
}

void AirwindowsMeter::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::white); //blank screen before doing anything, unless our draw covers the whole display anyway
    //this is probably quick and optimized
    
    auto linewidth = getWidth();
    if (getHeight() > linewidth) linewidth = getHeight();
    linewidth = (int)cbrt(linewidth/2)/2;
    
    float dx = (float)getWidth()/720.0f; //width of individual pixel on this display object
    float dy = (float)getHeight()/200.0f; //height of individual pixel on this display object
    float dotWidth = fmax(1280.0f/(float)getParentWidth(),1.0f); //scale single pixel to roughly the right size
    float dotHeight = fmax(720.0f/(float)getParentHeight(),1.0f); //scale single pixel to roughly the right size

    g.setColour(juce::Colours::lightgrey);
    g.fillRect(0.0,  60.0f*dy, (float)getWidth(),1.0); // -6dB markings
    g.fillRect(0.0, 100.0f*dy, (float)getWidth(),1.0); //-12dB markings
    g.fillRect(0.0, 130.0f*dy, (float)getWidth(),1.0); //-18dB markings
    g.fillRect(0.0, 150.0f*dy, (float)getWidth(),1.0); //-24dB markings
    g.fillRect(0.0, 165.0f*dy, (float)getWidth(),1.0); //-30dB markings
    g.fillRect(0.0, 175.0f*dy, (float)getWidth(),1.0); //-36dB markings

    for (int count = 0; count < dataPoints; ++count) //count through all the points in the array
    {
        g.setColour(juce::Colours::grey);
        g.fillRect((float)dataPosition*dx, 0.0f, 1.0f, (float)getHeight()); //the moving line
        g.setColour(juce::Colours::black);

        float peakL = dataC[count] * 200.0f;
        float peakR = dataD[count] * 200.0f;
        float slewL = sqrt(dataE[count])*275.0f;
        if (slewL > 197.0f) slewL = 197.0;
        float slewR = sqrt(dataF[count])*275.0f;
        if (slewR > 197.0f) slewR = 197.0;
        float zeroL = (sqrt(dataG[count])*6.0f)-6.0f;
        if (zeroL > 198.0f) zeroL = 198.0f;
        float zeroR = (sqrt(dataH[count])*6.0f)-6.0f;
        if (zeroR > 198.0f) zeroR = 198.0f;
        
        if (dataA[count] > dataB[count]) {
            //draw grey to R point, then green to L
            float h = dataB[count] * 200.0f; //height of this data point
            float setR = 1.0f-dataB[count];
            float setG = 1.0f-dataB[count];
            float setB = 1.0f-dataB[count];
            g.setColour(juce::Colour::fromFloatRGBA (setR, setG, setB, 1.0f));
            g.fillRect((float)count*dx, (200.0f - h)*dy, dx, h*dy);
            float greyPoint = h;
            h = dataA[count] * 200.0f; //height of this data point
            g.setColour(juce::Colour::fromFloatRGBA (0.0, 1.0, 0.0, 1.0f)); //L is pure green
            g.fillRect((float)count*dx, (200.0f - h)*dy, dx, (h-greyPoint)*dy);
            g.setColour(juce::Colours::black); //outline in black
            float extent = fabs(h-lastLOutline)+1.0f;
            if (extent > h) extent = h;
            g.fillRect((float)count*dx, (200.0f - h)*dy, dx, extent*dy);
            g.setColour(juce::Colour::fromFloatRGBA (0.9f, 0.9f, 0.9f, 1.0f));
            g.fillRect((float)count*dx, (400.0f - h)*dy, dx, h*dy);
            lastLOutline = h;
        } else {
            //draw grey to L point, then red to R
            float h = dataA[count] * 200.0f; //height of this data point
            float setR = 1.0f-dataA[count];
            float setG = 1.0f-dataA[count];
            float setB = 1.0f-dataA[count];
            g.setColour(juce::Colour::fromFloatRGBA (setR, setG, setB, 1.0f));
            g.fillRect((float)count*dx, (200.0f - h)*dy, dx, h*dy);
            float greyPoint = h;
            h = dataB[count] * 200.0f; //height of this data point
            g.setColour(juce::Colour::fromFloatRGBA (1.0, 0.0, 0.0, 1.0f)); //R is pure red
            g.fillRect((float)count*dx, (200.0f - h)*dy, dx, (h-greyPoint)*dy);
            g.setColour(juce::Colours::black); //outline in black
            float extent = fabs(h-lastROutline)+1.0f;
            if (extent > h) extent = h;
            g.fillRect((float)count*dx, (200.0f - h)*dy, dx, extent*dy);
            g.setColour(juce::Colour::fromFloatRGBA (0.9f, 0.9f, 0.9f, 1.0f));
            g.fillRect((float)count*dx, (400.0f - h)*dy, dx, h*dy);
            lastROutline = h;
        } //done with RMS chunk and sound density
        
        if (peakL > 197.0f) {
            peakL = 197.0;
            g.setColour(juce::Colour(255, 0, 0));
            g.fillRect((float)count*dx, (200.0f - peakL)*dy, dotWidth*dx, dotHeight*dy);

        } else if (peakL != 0.0f) {
            g.setColour(juce::Colours::black);
            g.fillRect((float)count*dx, (200.0f - peakL)*dy, dotWidth*dx, dotHeight*dy);
        }
        
        if (peakR > 197.0f) {
            peakR = 197.0;
            g.setColour(juce::Colour(255, 0, 0));
            g.fillRect((float)count*dx, (200.0f - peakR)*dy, dotWidth*dx, dotHeight*dy);

        } else if (peakR != 0.0f) {
            g.setColour(juce::Colours::black);
            g.fillRect((float)count*dx, (200.0f - peakR)*dy, dotWidth*dx, dotHeight*dy);
            //done with peak
        }
     }
    
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.382f));
    g.fillRect(0, 0, getWidth(), linewidth);
    g.fillRect(0, 0, linewidth, getHeight());
    
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::white, 0.618f));
    g.fillRect(linewidth, getHeight()-linewidth, getWidth(), linewidth);
    g.fillRect(getWidth()-linewidth, linewidth, linewidth, getHeight()-linewidth);
}
