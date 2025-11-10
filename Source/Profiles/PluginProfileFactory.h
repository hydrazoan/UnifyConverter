#pragma once
#include <JuceHeader.h>
#include "PluginProfile.h"

class PluginProfileFactory
{
public:
    PluginProfileFactory();
    ~PluginProfileFactory();

    /** Load all JSON profiles from a directory */
    int loadProfilesFromDirectory(const juce::File& dir);

    /** Load a single profile JSON file */
    std::optional<PluginProfile> loadProfile(const juce::File& file);

    /** Lookup by pluginName or alias */
    const PluginProfile* getProfileByName(const juce::String& name) const;

    /** Create a fallback profile when unknown */
    PluginProfile createDefaultProfile(const juce::String& pluginId,
                                       const juce::String& name);

private:
    juce::Array<PluginProfile> loadedProfiles;
};