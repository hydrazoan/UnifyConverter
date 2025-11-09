#pragma once
#include <JuceHeader.h>
#include "../Core/PresetData.h"
#include "../Profiles/PluginProfile.h"

// Experimental demuxer for Zampler FBCh bank chunks.
// Strategy:
//  - If profile provides "demuxMarkers" (ASCII delimiters), split on those.
//  - Else, try heuristic scan for multiple textual "ProgramName"/"Preset" keys.
//  - Never throw; if split isn't confident, return empty and caller will fallback.

class ZamplerDemuxer
{
public:
    ZamplerDemuxer() = default;
    ~ZamplerDemuxer() = default;

    // Returns N>0 when split found; otherwise returns empty vector (caller falls back)
    std::vector<PresetData> demux(const PluginProfile& profile,
                                  const PresetData& bankPreset) noexcept;

private:
    // Profile-driven split (delimiter strings)
    std::vector<PresetData> splitByMarkers(const PluginProfile& profile,
                                           const PresetData& bankPreset) noexcept;

    // Heuristic: try to split by common textual keys ("ProgramName=", "<program", etc.)
    std::vector<PresetData> splitHeuristically(const PresetData& bankPreset) noexcept;

    static juce::String extractLikelyNameFromBlock(const void* data, size_t size) noexcept;
};