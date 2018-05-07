/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


struct ParameterSlider    : public Slider,public Timer
{
    ParameterSlider (AudioProcessorParameter& p)
    : Slider (p.getName (256)), param (p)
    {
        setRange (0.0, 1.0, 0.0);
        startTimerHz (30);
        updateSliderPos();
    }
    
    void valueChanged() override
    {
        if (isMouseButtonDown())
            param.setValueNotifyingHost ((float) Slider::getValue());
        else
            param.setValue ((float) Slider::getValue());
    }
    
    void timerCallback() override       { updateSliderPos(); }
    void startedDragging() override     { param.beginChangeGesture(); }
    void stoppedDragging() override     { param.endChangeGesture();   }
    double getValueFromText (const String& text) override   { return param.getValueForText (text); }
    String getTextFromValue (double value) override         { return param.getText ((float) value, 1024) + " " + param.getLabel(); }
    
    void updateSliderPos()
    {
        const float newValue = param.getValue();
        
        if (newValue != (float) Slider::getValue() && ! isMouseButtonDown())
            Slider::setValue (newValue);
    }
    
    AudioProcessorParameter& param;
};

//==============================================================================
/**
*/
class Binaural_spatAudioProcessorEditor  : public AudioProcessorEditor,
                                           public Button::Listener,
                                           public ComboBox::Listener
{
public:
    Binaural_spatAudioProcessorEditor (Binaural_spatAudioProcessor&);
    ~Binaural_spatAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void buttonClicked (Button* button) override;
    void comboBoxChanged(ComboBox* comboBox) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Binaural_spatAudioProcessor& processor;
    
    ScopedPointer<ParameterSlider> az_Slider, el_Slider;
    ComboBox Select_channel;
    TextButton Select_folder;
    TextEditor Label_folder;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Binaural_spatAudioProcessorEditor)
};
