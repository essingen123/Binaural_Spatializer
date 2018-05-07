/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
Binaural_spatAudioProcessorEditor::Binaural_spatAudioProcessorEditor (Binaural_spatAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    int i;
    
    addAndMakeVisible(az_Slider= new ParameterSlider(*processor.az_Param));
    addAndMakeVisible(el_Slider= new ParameterSlider(*processor.el_Param));
    addAndMakeVisible(&Select_channel);
    addAndMakeVisible(&Select_folder);
    addAndMakeVisible(&Label_folder);
    
    az_Slider->setSliderStyle(Slider::Rotary);
    az_Slider->setTextBoxStyle (Slider::TextBoxBelow , false, 100, 20);
    el_Slider->setSliderStyle(Slider::Rotary);
    el_Slider->setTextBoxStyle (Slider::TextBoxBelow , false, 100, 20);
    Select_folder.setButtonText("Select ...");
    Select_folder.addListener (this);
    Label_folder.setText("No bank selected",dontSendNotification);
    
    Label_folder.setReadOnly(true);
    Label_folder.setCaretVisible(false);

    Select_channel.addListener(this);
    
    i = 1;
    for (auto choice : processor.channel_Param->choices)
        Select_channel.addItem (choice, i++);
    
    Select_channel.setSelectedId (processor.channel_Param->getIndex() + 1);
    
    setSize (400, 300);
    
}

Binaural_spatAudioProcessorEditor::~Binaural_spatAudioProcessorEditor()
{
}

//==============================================================================
void Binaural_spatAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.setColour (Colours::white);
    g.setFont (20.0f);
    g.drawFittedText ("Binaural Spatializer", 10,15,380,10, Justification::centred, 1);
    g.setFont (15.0f);
    g.drawFittedText ("Input Channel:", 20,60,100,20, Justification::left, 1);
    g.drawFittedText ("HRTF Folder:", 20,100,100,20, Justification::left, 1);
    g.drawFittedText ("Azimuth", 20,140,160,20, Justification::centred, 1);
    g.drawFittedText ("Elevation", 220,140,160,20, Justification::centred, 1);
    g.setFont (8.0f);
    g.drawFittedText ("Made by O. Torillec, J. Legrand, and V. Choqueuse", 10,280,380,20, Justification::right, 1);
}

void Binaural_spatAudioProcessorEditor::resized()
{
    Select_channel.setBounds (140, 60, 240,20);
    Select_folder.setBounds(140, 100, 60, 20);
    Label_folder.setBounds(220, 100, 160, 20);
    az_Slider->setBounds (10,160, 190, 100);
    el_Slider->setBounds (210,160, 190,100);
    
}

void Binaural_spatAudioProcessorEditor::buttonClicked(Button* button)
{
    
    if (button == &Select_folder)
    {
        FileChooser chooser ("Select ...",File::nonexistent,"*.wav");
        if (chooser.browseForDirectory())
        {
            processor.folder=chooser.getResult();
            String info_string="bank "+processor.folder.getFileName()+" loaded";
            Label_folder.setText(info_string,dontSendNotification);
        }
    }
}

void Binaural_spatAudioProcessorEditor::comboBoxChanged(ComboBox* comboBox)
{
    
    if (comboBox == &Select_channel)
    {
        processor.channel=comboBox->getSelectedItemIndex();
    }
}



