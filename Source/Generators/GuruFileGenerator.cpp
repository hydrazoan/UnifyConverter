#include "GuruFileGenerator.h"

bool GuruFileGenerator::saveGuruFile(const PresetData& preset, const juce::File& file) noexcept
{
    lastError = {};

    if (!file.getParentDirectory().createDirectory())
    {
        lastError = "Failed to create directory: " + file.getParentDirectory().getFullPathName();
        return false;
    }

    juce::FileOutputStream out(file);
    if (!out.openedOk())
    {
        lastError = "Failed to open for write: " + file.getFullPathName();
        return false;
    }

    juce::DynamicObject::Ptr meta = new juce::DynamicObject();
    meta->setProperty("presetName",  preset.presetName);
    meta->setProperty("pluginName",  preset.pluginName);
    meta->setProperty("pluginId",    preset.pluginId);
    meta->setProperty("author",      preset.author);
    meta->setProperty("category",    preset.category);
    meta->setProperty("description", preset.description);

    juce::var v(meta.get());
    auto json = juce::JSON::toString(v, true);

    out.setPosition(0);
    out.truncate();
    out.writeText(json, false, false, "\n");
    out.flush();

    return true;
}