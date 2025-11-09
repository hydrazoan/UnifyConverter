#pragma once
#include <JuceHeader.h>
#include "../Core/PresetData.h"

class Vst2Parser
{
public:
    Vst2Parser() = default;
    ~Vst2Parser() = default;

    // Single files
    PresetData              parseFxpPreset(const juce::File& file);
    std::vector<PresetData> parseFxbBank(const juce::File& file);

    // Unified
    std::vector<PresetData> parseFile(const juce::File& file);

    juce::String getLastError() const noexcept { return lastError; }

private:
    struct FxbHeader
    {
        char    chunkMagic[4];  // "CcnK"
        int32_t byteSize;
        char    fxMagic[4];     // "FBCh" bank chunk OR "FxBk" param-bank
        int32_t version;
        char    fxID[4];
        int32_t fxVersion;
        int32_t numPrograms;
        char    future[128];
    };

    struct FxpHeader
    {
        char    chunkMagic[4];  // "CcnK"
        int32_t byteSize;
        char    fxMagic[4];     // "FxCk" chunk-preset or "FPCh" param-preset
        int32_t version;
        char    fxID[4];
        int32_t fxVersion;
        int32_t numParams;
        char    prgName[28];
    };

    bool    readFxbHeader(juce::MemoryInputStream& stream, FxbHeader& header);
    bool    readFxpHeader(juce::MemoryInputStream& stream, FxpHeader& header);
    int32_t readBigEndianInt32(juce::MemoryInputStream& stream);

    juce::String lastError;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Vst2Parser)
};