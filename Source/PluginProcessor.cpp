#include "PluginProcessor.h"
#include "PluginEditor.h"

DistortionVSTAudioProcessor::DistortionVSTAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    m_state = std::make_unique<juce::AudioProcessorValueTreeState>(*this, nullptr);

    using Parameter = juce::AudioProcessorValueTreeState::Parameter;
    m_state->createAndAddParameter(std::make_unique<Parameter>("drive", "Drive", juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 1.0f));
    m_state->createAndAddParameter(std::make_unique<Parameter>("range", "Range", juce::NormalisableRange<float>(0.0f, 3000.0f, 0.001f), 1.0f));
    m_state->createAndAddParameter(std::make_unique<Parameter>("blend", "Blend", juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 1.0f));
    m_state->createAndAddParameter(std::make_unique<Parameter>("volume", "Volume", juce::NormalisableRange<float>(0.0f, 3.0f, 0.001f), 1.0f));

    m_state->state = juce::ValueTree("drive");
    m_state->state = juce::ValueTree("range");
    m_state->state = juce::ValueTree("blend");
    m_state->state = juce::ValueTree("volume");
}

const juce::String DistortionVSTAudioProcessor::getName() const {
    return JucePlugin_Name;
}

bool DistortionVSTAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool DistortionVSTAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool DistortionVSTAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double DistortionVSTAudioProcessor::getTailLengthSeconds() const {
    return 0.0;
}

int DistortionVSTAudioProcessor::getNumPrograms() {
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DistortionVSTAudioProcessor::getCurrentProgram() {
    return 0;
}

void DistortionVSTAudioProcessor::setCurrentProgram (int index) {
    juce::ignoreUnused(index);
}

const juce::String DistortionVSTAudioProcessor::getProgramName (int index) {
    juce::ignoreUnused(index);
    return {};
}

void DistortionVSTAudioProcessor::changeProgramName (int index, const juce::String& newName) {
    juce::ignoreUnused(index);
    juce::ignoreUnused(newName);
}

void DistortionVSTAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) {
    juce::ignoreUnused(sampleRate);
    juce::ignoreUnused(samplesPerBlock);
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void DistortionVSTAudioProcessor::releaseResources() {
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DistortionVSTAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const {
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void DistortionVSTAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;
    const auto totalNumInputChannels  = getTotalNumInputChannels();
    const auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    const float drive = *m_state->getRawParameterValue("drive");
    const float range = *m_state->getRawParameterValue("range");
    const float blend = *m_state->getRawParameterValue("blend");
    const float volume = *m_state->getRawParameterValue("volume");

    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        auto* channelData = buffer.getWritePointer (channel);
        for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
            // Calculate distortion
            const auto cleanChannelData = *channelData;
            *channelData *= drive * range;
            *channelData = (((2.f / juce::float_Pi) * atan(*channelData) * blend) +
                (cleanChannelData * (1.f - blend))) * volume;
            channelData++;
        }
    }
}

bool DistortionVSTAudioProcessor::hasEditor() const {
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DistortionVSTAudioProcessor::createEditor() {
    return new DistortionVSTAudioProcessorEditor (*this);
}

void DistortionVSTAudioProcessor::getStateInformation (juce::MemoryBlock& destData) {
    juce::MemoryOutputStream stream(destData, false);
    m_state->state.writeToStream(stream);
}

void DistortionVSTAudioProcessor::setStateInformation (const void* data, int sizeInBytes) {
    juce::ValueTree tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid()) {
        m_state->state = tree;
    }
}

juce::AudioProcessorValueTreeState& DistortionVSTAudioProcessor::getState() {
    return *m_state;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new DistortionVSTAudioProcessor();
}
