#pragma once

#include <juce_core/juce_core.h>

//=====================================================
struct PluginProfile
{
    juce::String pluginName;
    juce::String pluginId;
    juce::String manufacturer;
    juce::String version;

    bool isChunkBased = false;
    bool isVst2       = false;

    int schemaVersion = 1; // For migration
};

//=====================================================
class PluginProfileFactory
{
public:
    static juce::Result loadProfile (const juce::File& file,
                                     PluginProfile& outProfile);

    static juce::Result saveProfile (const juce::File& file,
                                     const PluginProfile& profile);

private:
    // Helpers
    static juce::String getStringProp (const juce::DynamicObject* obj,
                                       const juce::Identifier& key,
                                       const juce::String& defaultValue = {});

    static bool getBoolProp (const juce::DynamicObject* obj,
                             const juce::Identifier& key,
                             bool defaultValue = false);

    static int getIntProp (const juce::DynamicObject* obj,
                           const juce::Identifier& key,
                           int defaultValue = 1);

    static juce::Result migrateIfNeeded (PluginProfile& profile);
};
