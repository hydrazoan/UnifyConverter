#pragma once
#include <JuceHeader.h>
#include "../Core/PresetData.h"

class GuruFileGenerator
{
public:
    GuruFileGenerator() = default;
    ~GuruFileGenerator() = default;

    bool saveGuruFile(const PresetData& preset, const juce::File& file) noexcept;
    juce::String getLastError() const noexcept { return lastError; }

private:
    juce::String lastError;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GuruFileGenerator)
};