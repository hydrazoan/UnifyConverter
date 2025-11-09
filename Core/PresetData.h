#pragma once
#include <JuceHeader.h>

struct PresetData
{
    // Basic info
    juce::String presetName;
    juce::String pluginName;
    juce::String pluginId;

    // Optional metadata
    juce::String author;
    juce::String category;
    juce::String description;

    // Data kinds
    bool isChunkBased = false;
    bool isTextBased  = false;

    juce::MemoryBlock chunkData;
    juce::String      textData;

    // Optional params
    juce::Array<float>  parameters;
    juce::StringArray   parameterNames;

    // Validity
    bool         isValid = false;
    juce::String errorMessage;

    void setValid(bool v, const juce::String& err = {}) noexcept
    {
        isValid = v; errorMessage = v ? juce::String() : err;
    }

    bool ok() const noexcept
    {
        return isValid && (isChunkBased || isTextBased || parameters.size() > 0);
    }

    JUCE_LEAK_DETECTOR(PresetData)
};