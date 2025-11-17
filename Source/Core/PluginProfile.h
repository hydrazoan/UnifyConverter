#pragma once

#include <JuceHeader.h>

/**
 * @struct ParameterMapping
 * @brief Maps a plugin parameter to a Unify control
 */
struct ParameterMapping
{
    int parameterIndex = -1;         // Source parameter index in plugin
    juce::String parameterName;      // Parameter name in plugin
    juce::String unifyControl;       // Target control name in Unify
    float defaultValue = 0.0f;       // Default value (0.0 - 1.0)
    float minValue = 0.0f;           // Minimum value
    float maxValue = 1.0f;           // Maximum value
    juce::String displayUnit;        // Display unit (Hz, dB, %, etc.)
    
    bool isValid() const
    {
        return parameterIndex >= 0 && unifyControl.isNotEmpty();
    }
};

/**
 * @class PluginProfile
 * @brief Defines how to convert a specific plugin's presets to Unify format
 * 
 * Contains metadata about the plugin and parameter mappings for conversion.
 */
class PluginProfile
{
public:
    PluginProfile() = default;
    ~PluginProfile() = default;
    
    // Plugin identification
    juce::String pluginName;         // Human-readable name
    juce::String pluginId;           // VST unique ID
    juce::String manufacturer;       // Plugin manufacturer
    juce::String version;            // Plugin version this profile is for
    
    // Format information
    bool isChunkBased = true;        // true if plugin uses chunk format
    bool isVst2 = true;              // true for VST2, false for VST3
    
    // Parameter mappings (for parameter-based presets)
    std::vector<ParameterMapping> parameterMappings;
    
    // Unify layer configuration
    juce::String defaultLayerType = "Sampler";  // Default layer type in Unify
    juce::StringArray requiredSamples;           // Sample files needed
    
    // Metadata
    juce::String profileAuthor;      // Who created this profile
    juce::Time creationDate;         // When profile was created
    juce::String notes;              // Additional notes/limitations
    
    /**
     * Add a parameter mapping
     */
    void addParameterMapping(int index, const juce::String& name, 
                            const juce::String& unifyControl,
                            float defaultVal = 0.0f)
    {
        ParameterMapping mapping;
        mapping.parameterIndex = index;
        mapping.parameterName = name;
        mapping.unifyControl = unifyControl;
        mapping.defaultValue = defaultVal;
        parameterMappings.push_back(mapping);
    }
    
    /**
     * Find mapping by parameter index
     */
    const ParameterMapping* findMappingByIndex(int index) const
    {
        for (const auto& mapping : parameterMappings)
        {
            if (mapping.parameterIndex == index)
                return &mapping;
        }
        return nullptr;
    }
    
    /**
     * Find mapping by parameter name
     */
    const ParameterMapping* findMappingByName(const juce::String& name) const
    {
        for (const auto& mapping : parameterMappings)
        {
            if (mapping.parameterName == name)
                return &mapping;
        }
        return nullptr;
    }
    
    /**
     * Check if profile is valid
     */
    bool isValid() const
    {
        return pluginName.isNotEmpty() && pluginId.isNotEmpty();
    }
    
    /**
     * Load profile from JSON
     */
    bool loadFromJson(const juce::var& json);
    
    /**
     * Save profile to JSON
     */
    juce::var saveToJson() const;
    
    /**
     * Load profile from file
     */
    bool loadFromFile(const juce::File& file);
    
    /**
     * Save profile to file
     */
    bool saveToFile(const juce::File& file) const;
    
    /**
     * Get a summary string
     */
    juce::String getSummary() const
    {
        juce::String summary;
        summary << "Plugin Profile: " << pluginName << "\n";
        summary << "Manufacturer: " << manufacturer << "\n";
        summary << "ID: " << pluginId << "\n";
        summary << "Type: " << (isChunkBased ? "Chunk-based" : "Parameter-based") << "\n";
        summary << "Format: " << (isVst2 ? "VST2" : "VST3") << "\n";
        summary << "Mappings: " << parameterMappings.size() << "\n";
        return summary;
    }
    
private:
    JUCE_LEAK_DETECTOR(PluginProfile)
};
