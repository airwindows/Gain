//
// Created by Paul Walker on 8/23/21.
//

#ifndef AIRWINDOW_JUCE_TOY_TIMESERIESDISPLAY_H
#define AIRWINDOW_JUCE_TOY_TIMESERIESDISPLAY_H

#include "PluginProcessor.h"

struct TimeSeriesDisplay : public juce::Component
{
    void paint(juce::Graphics &g) override;
    
    static constexpr int n_points = 1200;
    std::array<float, n_points> dataRMSL;
    std::array<float, n_points> dataRMSR;
    std::array<float, n_points> dataPeakL;
    std::array<float, n_points> dataPeakR;
    std::array<float, n_points> dataSlewL;
    std::array<float, n_points> dataSlewR; //standard array of floats of size n_points
    float lastLOutline = 0.0;
    float lastROutline = 0.0;
    float lastLPeak = 0.0;
    float lastRPeak = 0.0;
    u_long count = 0;
    
    void pushRMSL(float rmsL)
    {
        dataRMSL[count] = rmsL;
    }
    void pushRMSR(float rmsR)
    {
        dataRMSR[count] = rmsR;
    }
    void pushPeakL(float peakL)
    {
        dataPeakL[count] = peakL;
    }
    void pushPeakR(float peakR)
    {
        dataPeakR[count] = peakR;
    }
    void pushSlewL(float slewL)
    {
        dataSlewL[count] = slewL;
    }
    void pushSlewR(float slewR)
    {
        dataSlewR[count] = slewR;
    }
    void pushIncrement(float limit)
    {
        count++;
        if (count >= (u_long)limit) count = 0;
    }
};

#endif // AIRWINDOW_JUCE_TOY_TIMESERIESDISPLAY_H
