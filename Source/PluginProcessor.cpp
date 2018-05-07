/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
Binaural_spatAudioProcessor::Binaural_spatAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter(el_Param=new AudioParameterFloat ("El","Elevation",{ -45.f, 231.f, 0.f, 1.0f },0.f," deg"));
    addParameter(az_Param=new AudioParameterFloat ("Az", "Azimut", { -80.f, 80.f, 0.f, 1.0f },0.f," deg"));
    addParameter(channel_Param=new AudioParameterChoice ("CHA", "Channel", { "Mix", "Left","Right"},1));
}

Binaural_spatAudioProcessor::~Binaural_spatAudioProcessor()
{
}

//==============================================================================
const String Binaural_spatAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Binaural_spatAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Binaural_spatAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Binaural_spatAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Binaural_spatAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Binaural_spatAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Binaural_spatAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Binaural_spatAudioProcessor::setCurrentProgram (int index)
{
}

const String Binaural_spatAudioProcessor::getProgramName (int index)
{
    return {};
}

void Binaural_spatAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Binaural_spatAudioProcessor::updateParameters()
{
   
    int indice;
    int az_index,el_index;
    float az_min,el_min, value;
    float az_new=az_Param->get();
    float el_new=el_Param->get();
    
    // find nearest elevation
    el_index=1;
    el_min=std::pow(elevation_values[0]-el_new,2);
    for (indice=1;indice<50;indice++)
    {
        value=std::pow(elevation_values[indice]-el_new,2);
        if (value < el_min)
        {
            el_min=value;
            el_index=1+indice;
        }
    }
    
    // find nearest azimut
    az_index=1;
    az_min=std::pow(azimut_values[0]-az_new,2);
    for (indice=1;indice<25;indice++)
    {
        value=std::pow(azimut_values[indice]-az_new,2);
        if (value < az_min)
        {
            az_min=value;
            az_index=1+indice;
        }
    }
    
    if ((azimut != az_index)||(elevation != el_index))
    {
        azimut=az_index;
        elevation=el_index;
        String hrir_left="HRIRdroite_az"+String(azimut)+"_el"+String(elevation)+".wav";
        String hrir_right="HRIRgauche_az"+String(azimut)+"_el"+String(elevation)+".wav";
        
        size_t maxSize = 8192;
        
        if (folder.getChildFile(String(hrir_left)).exists() && folder.getChildFile(String(hrir_right)).exists())
        {
            conv_left.loadImpulseResponse(folder.getChildFile (hrir_left),false,true, maxSize);
            conv_right.loadImpulseResponse(folder.getChildFile (hrir_right),false,true, maxSize);
        }
    
    }
    
}

//==============================================================================
void Binaural_spatAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    ProcessSpec spec { sampleRate, static_cast<uint32> (samplesPerBlock), 1 };
    conv_left.prepare(spec);
    conv_right.prepare(spec);
    
    updateParameters();
    reset();
    
}

void Binaural_spatAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Binaural_spatAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Binaural_spatAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    AudioBlock<float> channel_block;

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    
    updateParameters();
    
    AudioBlock<float> block(buffer);

    switch(channel)
    {
        case 0:{
                channel_block = block.getSingleChannelBlock(0).add(block.getSingleChannelBlock(1));
                }break;
        case 1:{
                channel_block = block.getSingleChannelBlock(0);
                }break;
        case 2:{
                channel_block = block.getSingleChannelBlock(1);
                } break;
    }

    auto channelL = block.getSingleChannelBlock(0).copy(channel_block);
    auto channelR = block.getSingleChannelBlock(1).copy(channel_block);
    
    // perform convolution
    conv_left.process(ProcessContextReplacing<float> (channelL) );
    conv_right.process(ProcessContextReplacing<float> (channelR));
    

}

//==============================================================================
bool Binaural_spatAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Binaural_spatAudioProcessor::createEditor()
{
    return new Binaural_spatAudioProcessorEditor (*this);
}

//==============================================================================
void Binaural_spatAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Binaural_spatAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Binaural_spatAudioProcessor();
}
