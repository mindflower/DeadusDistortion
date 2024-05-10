#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class DistortionVSTAudioProcessorEditor : public juce::AudioProcessorEditor {
public:
    DistortionVSTAudioProcessorEditor (DistortionVSTAudioProcessor&);
    ~DistortionVSTAudioProcessorEditor() = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    std::unique_ptr<juce::Slider> m_driveKnob;
    std::unique_ptr<juce::Slider> m_rangeKnob;
    std::unique_ptr<juce::Slider> m_blendKnob;
    std::unique_ptr<juce::Slider> m_volumeKnob;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_driveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_rangeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_blendAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_volumeAttachment;

    DistortionVSTAudioProcessor& m_audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionVSTAudioProcessorEditor)
};
