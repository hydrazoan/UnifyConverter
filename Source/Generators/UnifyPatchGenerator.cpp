#include "UnifyPatchGenerator.h"

bool UnifyPatchGenerator::savePatch(const PresetData& preset, const juce::File& file) noexcept
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

    juce::DynamicObject::Ptr root = new juce::DynamicObject();
    root->setProperty("name",         preset.presetName);
    root->setProperty("pluginName",   preset.pluginName);
    root->setProperty("pluginId",     preset.pluginId);
    root->setProperty("author",       preset.author);
    root->setProperty("category",     preset.category);
    root->setProperty("description",  preset.description);
    root->setProperty("isChunkBased", preset.isChunkBased);
    root->setProperty("isTextBased",  preset.isTextBased);

    if (preset.isChunkBased && preset.chunkData.getSize() > 0)
    {
        juce::MemoryOutputStream mos;
        juce::Base64::convertToBase64(mos, preset.chunkData.getData(), preset.chunkData.getSize());
        root->setProperty("chunkBase64", mos.toString());
    }
    else if (preset.isTextBased && preset.textData.isNotEmpty())
    {
        root->setProperty("textData", preset.textData);
    }
    else if (preset.parameters.size() > 0)
    {
        juce::Array<juce::var> params;
        params.ensureStorageAllocated(preset.parameters.size());
        for (auto v : preset.parameters) params.add(juce::var(v));
        root->setProperty("parameters", juce::var(params));
    }

    juce::var v(root.get());
    auto json = juce::JSON::toString(v, true);

    out.setPosition(0);
    out.truncate();
    out.writeText(json, false, false, "\n");
    out.flush();

    return true;
}
