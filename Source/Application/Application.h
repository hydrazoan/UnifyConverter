#pragma once
#include <JuceHeader.h>
#include "../UI/MainComponent.h"

class PresetConverterApplication : public juce::JUCEApplication
{
public:
    PresetConverterApplication() = default;

    const juce::String getApplicationName() override       { return "Preset Converter"; }
    const juce::String getApplicationVersion() override    { return "1.0.0"; }
    bool moreThanOneInstanceAllowed() override             { return true; }

    void initialise (const juce::String&) override;
    void shutdown() override;

    void systemRequestedQuit() override { quit(); }
    void anotherInstanceStarted (const juce::String&) override {}

private:
    std::unique_ptr<juce::DocumentWindow> mainWindow;
};