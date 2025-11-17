#include "PluginProfile.h"

bool PluginProfile::loadFromJson(const juce::var& json)
{
    if (!json.isObject())
        return false;
    
    auto* obj = json.getDynamicObject();
    if (obj == nullptr)
        return false;
    
    // Load basic info
    pluginName = obj->getProperty("pluginName").toString();
    pluginId = obj->getProperty("pluginId").toString();
    manufacturer = obj->getProperty("manufacturer").toString();
    version = obj->getProperty("version").toString();
    
    // Load format info
    isChunkBased = obj->getProperty("isChunkBased");
    isVst2 = obj->getProperty("isVst2");
    
    // Load layer config
    defaultLayerType = obj->getProperty("defaultLayerType").toString();
    if (defaultLayerType.isEmpty())
        defaultLayerType = "Sampler";
    
    // Load required samples
    auto samplesVar = obj->getProperty("requiredSamples");
    if (samplesVar.isArray())
    {
        auto* samplesArray = samplesVar.getArray();
        for (const auto& sample : *samplesArray)
            requiredSamples.add(sample.toString());
    }
    
    // Load metadata
    profileAuthor = obj->getProperty("profileAuthor").toString();
    notes = obj->getProperty("notes").toString();
    
    // Load parameter mappings
    auto mappingsVar = obj->getProperty("parameterMappings");
    if (mappingsVar.isArray())
    {
        auto* mappingsArray = mappingsVar.getArray();
        for (const auto& mappingVar : *mappingsArray)
        {
            if (!mappingVar.isObject())
                continue;
            
            auto* mappingObj = mappingVar.getDynamicObject();
            if (mappingObj == nullptr)
                continue;
            
            ParameterMapping mapping;
            mapping.parameterIndex = mappingObj->getProperty("index");
            mapping.parameterName = mappingObj->getProperty("name").toString();
            mapping.unifyControl = mappingObj->getProperty("unifyControl").toString();
            mapping.defaultValue = mappingObj->getProperty("default");
            mapping.minValue = mappingObj->getProperty("min");
            mapping.maxValue = mappingObj->getProperty("max");
            mapping.displayUnit = mappingObj->getProperty("unit").toString();
            
            if (mapping.isValid())
                parameterMappings.push_back(mapping);
        }
    }
    
    return isValid();
}

juce::var PluginProfile::saveToJson() const
{
    auto* obj = new juce::DynamicObject();
    
    // Save basic info
    obj->setProperty("pluginName", pluginName);
    obj->setProperty("pluginId", pluginId);
    obj->setProperty("manufacturer", manufacturer);
    obj->setProperty("version", version);
    
    // Save format info
    obj->setProperty("isChunkBased", isChunkBased);
    obj->setProperty("isVst2", isVst2);
    
    // Save layer config
    obj->setProperty("defaultLayerType", defaultLayerType);
    
    // Save required samples
    juce::Array<juce::var> samplesArray;
    for (const auto& sample : requiredSamples)
        samplesArray.add(sample);
    obj->setProperty("requiredSamples", samplesArray);
    
    // Save metadata
    obj->setProperty("profileAuthor", profileAuthor);
    obj->setProperty("notes", notes);
    
    // Save parameter mappings
    juce::Array<juce::var> mappingsArray;
    for (const auto& mapping : parameterMappings)
    {
        auto* mappingObj = new juce::DynamicObject();
        mappingObj->setProperty("index", mapping.parameterIndex);
        mappingObj->setProperty("name", mapping.parameterName);
        mappingObj->setProperty("unifyControl", mapping.unifyControl);
        mappingObj->setProperty("default", mapping.defaultValue);
        mappingObj->setProperty("min", mapping.minValue);
        mappingObj->setProperty("max", mapping.maxValue);
        mappingObj->setProperty("unit", mapping.displayUnit);
        
        mappingsArray.add(juce::var(mappingObj));
    }
    obj->setProperty("parameterMappings", mappingsArray);
    
    return juce::var(obj);
}

bool PluginProfile::loadFromFile(const juce::File& file)
{
    if (!file.existsAsFile())
        return false;
    
    juce::String jsonText = file.loadFileAsString();
    if (jsonText.isEmpty())
        return false;
    
    juce::var json;
    auto result = juce::JSON::parse(jsonText, json);
    
    if (result.failed())
        return false;
    
    return loadFromJson(json);
}

bool PluginProfile::saveToFile(const juce::File& file) const
{
    auto json = saveToJson();
    juce::String jsonText = juce::JSON::toString(json, true);
    
    return file.replaceWithText(jsonText);
}
