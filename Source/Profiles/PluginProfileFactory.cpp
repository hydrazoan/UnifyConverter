#include "PluginProfileFactory.h"

//========== Helpers =================================================

static juce::String getStringProp(const juce::DynamicObject* obj, const juce::Identifier& key)
{
    if (!obj->hasProperty(key)) return {};
    return obj->getProperty(key).toString();
}

static bool getBoolProp(const juce::DynamicObject* obj, const juce::Identifier& key, bool defaultValue=false)
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

    auto v = obj->getProperty(key);

    if (v.isArray())
    {
        auto* arr = v.getArray();
        for (auto& item : *arr)
            out.add(item.toString());
    }
    else
    {
        out.add(v.toString());
    }
}

static void getVarArrayProp(const juce::DynamicObject* obj,
                            const juce::Identifier& key,
                            juce::Array<juce::var>& out)
{
    out.clear();

    if (!obj->hasProperty(key))
        return;

    auto v = obj->getProperty(key);
    if (v.isArray())
    {
        auto* arr = v.getArray();
        for (auto& item : *arr)
            out.add(item);
    }
    else
    {
        out.add(v);
    }
}


//========== Main Factory ===========================================

PluginProfileFactory::PluginProfileFactory() = default;
PluginProfileFactory::~PluginProfileFactory() = default;

int PluginProfileFactory::loadProfilesFromDirectory(const juce::File& dir)
{
    loadedProfiles.clear();

    if (!dir.exists() || !dir.isDirectory())
        return 0;

    juce::Array<juce::File> files;
    dir.findChildFiles(files, juce::File::findFiles, false, "*.json");

    for (auto& f : files)
    {
        auto profileOpt = loadProfile(f);
        if (profileOpt.has_value())
            loadedProfiles.add(profileOpt.value());
    }

    return loadedProfiles.size();
}

std::optional<PluginProfile> PluginProfileFactory::loadProfile(const juce::File& file)
{
    juce::var parsed;
    if (auto result = juce::JSON::parse(file); result.wasOk())
        parsed = result.getResult();
    else
        return {};

    if (!parsed.isObject())
        return {};

    auto* obj = parsed.getDynamicObject();
    if (!obj)
        return {};

    PluginProfile profile;

    profile.pluginName       = getStringProp(obj, "pluginName");
    profile.pluginId         = getStringProp(obj, "pluginId");
    profile.manufacturer     = getStringProp(obj, "manufacturer");
    profile.version          = getStringProp(obj, "version");
    profile.isChunkBased     = getBoolProp(obj, "isChunkBased", false);
    profile.isVst2           = getBoolProp(obj, "isVst2", false);
    profile.isVst3           = getBoolProp(obj, "isVst3", false);
    profile.defaultLayerType = getStringProp(obj, "defaultLayerType");
    profile.profileAuthor    = getStringProp(obj, "profileAuthor");
    profile.notes            = getStringProp(obj, "notes");

    getStringArrayProp(obj, "aliases",        profile.aliases);
    getStringArrayProp(obj, "requiredSamples", profile.requiredSamples);
    getVarArrayProp(obj,    "parameterMappings", profile.parameterMappings);

    if (!profile.isValid())
        return {};

    return profile;
}

const PluginProfile* PluginProfileFactory::getProfileByName(const juce::String& name) const
{
    for (auto& p : loadedProfiles)
        if (p.pluginName.compareIgnoreCase(name) == 0)
            return &p;

    // try alias fallback
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