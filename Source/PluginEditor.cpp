#include "PluginProcessor.h"
#include "PluginEditor.h"

DistortionVSTAudioProcessorEditor::DistortionVSTAudioProcessorEditor (DistortionVSTAudioProcessor& p)
    : AudioProcessorEditor (&p)
    , m_audioProcessor (p) {

    m_driveKnob = std::make_unique<juce::Slider>("Drive");
    addAndMakeVisible(*m_driveKnob);
    m_driveKnob->setSliderStyle(juce::Slider::Rotary);
    m_driveKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100);

    m_rangeKnob = std::make_unique<juce::Slider>("Range");
    addAndMakeVisible(*m_rangeKnob);
    m_rangeKnob->setSliderStyle(juce::Slider::Rotary);
    m_rangeKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100);

    m_blendKnob = std::make_unique<juce::Slider>("Blend");
    addAndMakeVisible(*m_blendKnob);
    m_blendKnob->setSliderStyle(juce::Slider::Rotary);
    m_blendKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100);

    m_volumeKnob = std::make_unique<juce::Slider>("Volume");
    addAndMakeVisible(*m_volumeKnob);
    m_volumeKnob->setSliderStyle(juce::Slider::Rotary);
    m_volumeKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100);

    m_driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.getState(), "drive", *m_driveKnob);
    m_rangeAttachment = std::make_unique < juce::AudioProcessorValueTreeState::SliderAttachment>(p.getState(), "range", *m_rangeKnob);
    m_blendAttachment = std::make_unique < juce::AudioProcessorValueTreeState::SliderAttachment>(p.getState(), "blend", *m_blendKnob);
    m_volumeAttachment = std::make_unique < juce::AudioProcessorValueTreeState::SliderAttachment>(p.getState(), "volume", *m_volumeKnob);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 200);
}

void DistortionVSTAudioProcessorEditor::paint (juce::Graphics& g) {
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);

    g.drawText("Drive", ((getWidth() / 5) * 1) - (100 / 2), (getHeight() / 2) + 5, 100, 100, juce::Justification::centred, false);
    g.drawText("Range", ((getWidth() / 5) * 2) - (100 / 2), (getHeight() / 2) + 5, 100, 100, juce::Justification::centred, false);
    g.drawText("Blend", ((getWidth() / 5) * 3) - (100 / 2), (getHeight() / 2) + 5, 100, 100, juce::Justification::centred, false);
    g.drawText("Volume", ((getWidth() / 5) * 4) - (100 / 2), (getHeight() / 2) + 5, 100, 100, juce::Justification::centred, false);

}

void DistortionVSTAudioProcessorEditor::resized() {
    m_driveKnob->setBounds(((getWidth() / 5) * 1) - (100 / 2), (getHeight() / 2) - (100 / 2), 100, 100);
    m_rangeKnob->setBounds(((getWidth() / 5) * 2) - (100 / 2), (getHeight() / 2) - (100 / 2), 100, 100);
    m_blendKnob->setBounds(((getWidth() / 5) * 3) - (100 / 2), (getHeight() / 2) - (100 / 2), 100, 100);
    m_volumeKnob->setBounds(((getWidth() / 5) * 4) - (100 / 2), (getHeight() / 2) - (100 / 2), 100, 100);
}
