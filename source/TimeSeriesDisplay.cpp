//
// Created by Paul Walker on 8/23/21.
//

#include "TimeSeriesDisplay.h"

void TimeSeriesDisplay::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::white); //blank screen before doing anything, unless our draw covers the whole display anyway
    //this is probably quick and optimized
    
    g.setColour(juce::Colours::lightgrey);
    g.fillRect(0.0,  60.0, (float)getWidth(),1.0); // -6dB markings
    g.fillRect(0.0, 100.0, (float)getWidth(),1.0); //-12dB markings
    g.fillRect(0.0, 130.0, (float)getWidth(),1.0); //-18dB markings
    g.fillRect(0.0, 150.0, (float)getWidth(),1.0); //-24dB markings
    g.fillRect(0.0, 165.0, (float)getWidth(),1.0); //-30dB markings
    g.fillRect(0.0, 175.0, (float)getWidth(),1.0); //-36dB markings

    float dx = getWidth() * (1.0f / n_points); //width of individual slice on this display object
    for (u_long i = 0; i < n_points; ++i) //count through all the points in the array
    {
        
       // g.setColour(juce::Colours::red);
       // g.fillRect(i * dx,  30.0, (float)getWidth(), 1.0); // -6dB markings
       
        g.setColour(juce::Colours::grey);
        g.fillRect(count * dx, 0.0f, 1.0f, (float)getHeight()); //the moving line

        float hPosition = getHeight() + 1.0f; //reference for how tall this is
        
        if (dataPeakL[i] > dataPeakR[i]) {
            float peakR = dataPeakR[i] * getHeight(); //height of this data point
            float peakL = dataPeakL[i] * getHeight();
            
            //float setD = sin(fabs(dataPeakL[i]-lastLPeak)*5.0f);
            //float setL = sin(dataSlewL[i]/(dataRMSL[i]+0.0001f));

            //g.setColour(juce::Colour::fromHSV(setL, 1.0, 1.0, setD));
            //g.fillRect(i * dx, hPosition - peakL, dx, fabs(peakL-(lastLPeak*getHeight())));

            g.setColour(juce::Colours::black);
            g.fillRect(i * dx, hPosition - peakL, dx, 2.0);
            lastLPeak = dataPeakL[i];
        } else {
            float peakL = dataPeakL[i] * getHeight(); //height of this data point
            float peakR = dataPeakR[i] * getHeight();
            
            //float setD = sin(fabs(dataPeakR[i]-lastRPeak)*5.0f);
            //float setL = sin(dataSlewR[i]/(dataRMSR[i]+0.0001f));

            //g.setColour(juce::Colour::fromHSV(setL, 1.0, 1.0, setD));
            //g.fillRect(i * dx, hPosition - peakR, dx, fabs(peakR-(lastRPeak*getHeight())));

            g.setColour(juce::Colours::black);
            g.fillRect(i * dx, hPosition - peakR, dx, 2.0);
            lastRPeak = dataPeakR[i];
        } //done with peak chunk and peak energy color spectrums

        if (dataRMSL[i] > dataRMSR[i]) {
            //draw grey to R point, then green to L
            float h = dataRMSR[i] * getHeight(); //height of this data point
            float setR = 1.0f-dataRMSR[i];
            float setG = 1.0f-dataRMSR[i];
            float setB = 1.0f-dataRMSR[i];
            g.setColour(juce::Colour::fromFloatRGBA (setR, setG, setB, 1.0f));
            g.fillRect(i * dx, hPosition - h, dx, h);
            float greyPoint = h;
            h = dataRMSL[i] * getHeight(); //height of this data point
            g.setColour(juce::Colour::fromFloatRGBA (0.0, 1.0, 0.0, 1.0f)); //L is pure green
            g.fillRect(i * dx, hPosition - h, dx, h-greyPoint);
            g.setColour(juce::Colours::black); //outline in black
            g.fillRect(i * dx, hPosition - h, dx, fabs(h-lastLOutline)+1.5f);
            lastLOutline = h;
        } else {
            //draw grey to L point, then red to R
            float h = dataRMSL[i] * getHeight(); //height of this data point
            float setR = 1.0f-dataRMSL[i];
            float setG = 1.0f-dataRMSL[i];
            float setB = 1.0f-dataRMSL[i];
            g.setColour(juce::Colour::fromFloatRGBA (setR, setG, setB, 1.0f));
            g.fillRect(i * dx, hPosition - h, dx, h);
            float greyPoint = h;
            h = dataRMSR[i] * getHeight(); //height of this data point
            g.setColour(juce::Colour::fromFloatRGBA (1.0, 0.0, 0.0, 1.0f)); //R is pure red
            g.fillRect(i * dx, hPosition - h, dx, h-greyPoint);
            g.setColour(juce::Colours::black); //outline in black
            g.fillRect(i * dx, hPosition - h, dx,  fabs(h-lastROutline)+1.0f);
            lastROutline = h;
        } //done with RMS chunk and sound density
    }
}
