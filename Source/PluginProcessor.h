/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

using namespace dsp;


//==============================================================================
/**
*/
class Binaural_spatAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Binaural_spatAudioProcessor();
    ~Binaural_spatAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect () const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void updateParameters();
    
    int channel=0;
    int is_loaded=0;
    int azimut,elevation;
    Convolution conv_right;
    Convolution conv_left;
    File folder;
    AudioParameterFloat* el_Param;
    AudioParameterFloat* az_Param;
    AudioParameterChoice* channel_Param;
    
    float elevation_values[50] ={-45., -39.375, -33.75, -28.125, -22.5, -16.875, -11.25 , -5.625, 0., 5.625, 11.25, 16.875, 22.5, 28.125, 33.75, 39.375, 45., 50.625, 56.25, 61.875, 67.5, 73.125, 78.75, 84.375, 90., 95.625, 101.25, 106.875, 112.5, 118.125, 123.75, 129.375, 135.,140.625, 146.25, 151.875, 157.5, 163.125, 168.75, 174.375, 180., 185.625, 191.25, 196.875, 202.5, 208.125, 213.75, 219.375,225., 230.625};
    
    float azimut_values[25] = {-80.,-65.,-55.,-45., -40., -35., -30., -25., -20., -15., -10.,  -5.,   0.,   5.,  10.,  15.,
        20.,  25.,  30.,  35.,  40.,  45., 55., 65., 80};
    

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Binaural_spatAudioProcessor)
};
