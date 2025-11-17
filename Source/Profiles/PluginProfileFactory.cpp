#include "PluginProfileFactory.h"

//==============================================================================
juce::String PluginProfileFactory::getStringProp (const juce::DynamicObject* obj,
                                                  const juce::Identifier& key,
                                                  const juce::String& defaultValue)
{
    if (! obj->hasProperty (key))
        return defaultValue;

    return obj->getProperty (key).toString();
}

//==============================================================================
bool PluginProfileFactory::getBoolProp (const juce::DynamicObject* obj,
                                        const juce::Identifier& key,
                                        bool defaultValue)
{
    if (! obj->hasProperty (key))
        return defaultValue;

    return static_cast<bool> (obj->getProperty (key));
}

//==============================================================================
int PluginProfileFactory::getIntProp (const juce::DynamicObject* obj,
                                      const juce::Identifier& key,
                                      int defaultValue)
{
    if (! obj->hasProperty (key))
        return defaultValue;

    return static_cast<int> (obj->getProperty (key));
}

//==============================================================================
// MIGRATION HOOK — runs automatically if schema version changes
juce::Result PluginProfileFactory::migrateIfNeeded (PluginProfile& p)
{
    constexpr int currentSchema = 1;

    if (p.schemaVersion == currentSchema)
        return juce::Result::ok();

    // --- Future schema migration example ---
    // if (p.schemaVersion == 0)
    //     applyChangesForSchema0to1(p);

    p.schemaVersion = currentSchema;
    return juce::Result::ok();
}

//==============================================================================
// LOAD
juce::Result PluginProfileFactory::loadProfile (const juce::File& file,
                                                PluginProfile& outProfile)
{
    if (! file.existsAsFile())
        return juce::Result::fail ("File does not exist: " + file.getFullPathName());

    // Load JSON text
    const juce::String text = file.loadFileAsString();

    juce::Result parseStatus;
    juce::var parsed = juce::JSON::parse(text, parseStatus);  // <-- FIXED HERE

    if (parseStatus.failed())
        return juce::Result::fail ("Failed to parse JSON: " + parseStatus.getErrorMessage());

    if (! parsed.isObject())
        return juce::Result::fail ("JSON root is not an object");

    auto* obj = parsed.getDynamicObject();
    if (obj == nullptr)
        return juce::Result::fail ("JSON root lacks a DynamicObject");

    // ----- Validate required fields -----
    static const juce::Identifier f_pluginName   ("pluginName");
    static const juce::Identifier f_pluginId     ("pluginId");
    static const juce::Identifier f_manufacturer ("manufacturer");
    static const juce::Identifier f_version      ("version");
    static const juce::Identifier f_isChunkBased ("isChunkBased");
    static const juce::Identifier f_isVst2       ("isVst2");
    static const juce::Identifier f_schemaVer    ("schemaVersion");

    auto require = [&](juce::Identifier key, const char* typeName)
    {
        if (! obj->hasProperty(key))
            return juce::Result::fail ("Missing required field: " + key.toString()
                                      + " (expected " + juce::String(typeName) + ")");
        return juce::Result::ok();
    };

    if (auto r = require(f_pluginName, "string"); r.failed()) return r;
    if (auto r = require(f_pluginId,   "string"); r.failed()) return r;
    if (auto r = require(f_manufacturer, "string"); r.failed()) return r;
    if (auto r = require(f_version, "string"); r.failed()) return r;
    if (auto r = require(f_isChunkBased, "bool"); r.failed()) return r;
    if (auto r = require(f_isVst2, "bool"); r.failed()) return r;

    // ----- Populate -----
    PluginProfile p;
    p.pluginName    = getStringProp(obj, f_pluginName);
    p.pluginId      = getStringProp(obj, f_pluginId);
    p.manufacturer  = getStringProp(obj, f_manufacturer);
    p.version       = getStringProp(obj, f_version);
    p.isChunkBased  = getBoolProp  (obj, f_isChunkBased);
    p.isVst2        = getBoolProp  (obj, f_isVst2);

    // Optional with default
    p.schemaVersion = getIntProp   (obj, f_schemaVer, 1);

    // ----- Migration if needed -----
    if (auto r = migrateIfNeeded(p); r.failed())
        return r;

    outProfile = p;
    return juce::Result::ok();
}


//==============================================================================
// SAVE
juce::Result PluginProfileFactory::saveProfile (const juce::File& file,
                                                const PluginProfile& profile)
{
    juce::DynamicObject::Ptr obj = new juce::DynamicObject();

    obj->setProperty ("pluginName",   profile.pluginName);
    obj->setProperty ("pluginId",     profile.pluginId);
    obj->setProperty ("manufacturer", profile.manufacturer);
    obj->setProperty ("version",      profile.version);
    obj->setProperty ("isChunkBased", profile.isChunkBased);
    obj->setProperty ("isVst2",       profile.isVst2);
    obj->setProperty ("schemaVersion",profile.schemaVersion);

    juce::var v (obj.get());
    auto json = juce::JSON::toString (v, true);

    if (! file.replaceWithText (json))
        return juce::Result::fail("Failed to write file: " + file.getFullPathName());

    return juce::Result::ok();
}
