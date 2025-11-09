#pragma once
#include <JuceHeader.h>
#include "../Core/PresetData.h"

class Vst3Parser
{
public:
    Vst3Parser() = default;
    ~Vst3Parser() = default;

    std::vector<PresetData> parseFile(const juce::File& file) noexcept;
    juce::String getLastError() const noexcept { return lastError; }

private:
    juce::String lastError;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Vst3Parser)
};