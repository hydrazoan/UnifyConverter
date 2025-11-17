#include "PluginProfileFactory.h"

// ===== Helpers =================================================

static juce::String getStringProp(const juce::DynamicObject* obj,
                                  const juce::Identifier& key)
{
    if (!obj->hasProperty(key)) return {};
    return obj->getProperty(key).toString();
}

static bool getBoolProp(const juce::DynamicObject* obj,
                        const juce::Identifier& key,
                        bool defaultValue=false)
{
    if (!obj->hasProperty(key)) return defaultValue;
    return obj->getProperty(key);
}

static void getStringArrayProp(const juce::DynamicObject* obj,
                               const juce::Identifier& key,
                               juce::StringArray& out)
{
    out.clear();

    if (!obj->hasProperty(key))
        return;

    auto value = obj->getProperty(key);

    if (value.isArray())
    {
        auto* arr = value.getArray();
        for (auto& v : *arr)
            out.add(v.toString());
    }
    else
    {
        out.add(value.toString());
    }
}

static void getVarArrayProp(const juce::DynamicObject* obj,
                            const juce::Identifier& key,
                            juce::Array<juce::var>& out)
{
    out.clear();

    if (!obj->hasProperty(key))
        return;

    auto value = obj->getProperty(key);

    if (value.isArray())
    {
        auto* arr = value.getArray();
        for (auto& v : *arr)
            out.add(v);
    }
    else
    {
        out.add(value);
    }
}


// ===== Factory =================================================

PluginProfileFactory::PluginProfileFactory() = default;
PluginProfileFactory::~PluginProfileFactory() = default;


int PluginProfileFactory::loadProfilesFromDirectory(const juce::File& dir)
{
    loadedProfiles.clear();

    if (!dir.exists() || !dir.isDirectory())
        return 0;

    juce::Array<juce::File> jsonFiles;
    dir.findChildFiles(jsonFiles, juce::File::findFiles, true, "*.json");

    for (auto& file : jsonFiles)
    {
        auto p = loadProfile(file);
        if (p.has_value())
            loadedProfiles.add(p.value());
    }

    return loadedProfiles.size();
}



std::optional<PluginProfile> PluginProfileFactory::loadProfile(const juce::File& file)
{
    auto result = juce::JSON::parse(file);

    if (!result.wasOk())
        return {};

    auto parsed = result.getResult();
    if (!parsed.isObject())
        return {};

    auto* obj = parsed.getDynamicObject();
    if (!obj)
        return {};

    PluginProfile p;

    p.pluginName       = getStringProp(obj, "pluginName");
    p.pluginId         = getStringProp(obj, "pluginId");
    p.manufacturer     = getStringProp(obj, "manufacturer");
    p.version          = getStringProp(obj, "version");

    p.isChunkBased     = getBoolProp(obj, "isChunkBased");
    p.isVst2           = getBoolProp(obj, "isVst2");
    p.isVst3           = getBoolProp(obj, "isVst3");

    p.defaultLayerType = getStringProp(obj, "defaultLayerType");
    p.profileAuthor    = getStringProp(obj, "profileAuthor");
    p.notes            = getStringProp(obj, "notes");

    getStringArrayProp(obj, "aliases",         p.aliases);
    getStringArrayProp(obj, "requiredSamples", p.requiredSamples);
    getVarArrayProp(obj, "parameterMappings",  p.parameterMappings);

    if (!p.isValid())
        return {};

    return p;
}



const PluginProfile* PluginProfileFactory::getProfileByName(const juce::String& name) const
{
    // direct name match
    for (auto& p : loadedProfiles)
        if (p.pluginName.equalsIgnoreCase(name))
            return &p;

    // alias match
    for (auto& p : loadedProfiles)
        if (p.aliases.contains(name, true))
            return &p;

    return nullptr;
}



PluginProfile PluginProfileFactory::createDefaultProfile(const juce::String& pluginId,
                                                         const juce::String& name)
{
    PluginProfile p;
    p.pluginId   = pluginId;
    p.pluginName = name;
    p.isVst2     = true;
    p.isChunkBased = true;
    return p;
}
