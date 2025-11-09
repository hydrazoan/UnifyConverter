#pragma once
#include <JuceHeader.h>

#include "../Core/PresetData.h"
#include "../Parsers/Vst2Parser.h"
#include "../Parsers/Vst3Parser.h"
#include "../Parsers/UHeParsers.h"
#include "../Generators/UnifyPatchGenerator.h"
#include "../Generators/GuruFileGenerator.h"
#include "../Profiles/PluginProfileFactory.h"

class MainComponent : public juce::Component,
                      public juce::FileDragAndDropTarget
{
public:
    MainComponent();
    ~MainComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    // DnD
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;
    void fileDragEnter(const juce::StringArray& files, int x, int y) override;
    void fileDragExit(const juce::StringArray& files) override;

private:
    void setupComponents();
    void browseForFiles();
    void selectOutputDirectory();
    void convertFile(const juce::File& inputFile);
    void convertPresets(const std::vector<PresetData>& presets,
                        const PluginProfile& profile,
                        const juce::File& outputDir);
    void logMessage(const juce::String& message);
    void updateProgress(double newProgress);
    const PluginProfile* getSelectedProfile() const;

    juce::Label       titleLabel, instructionsLabel;
    juce::ComboBox    pluginSelector; // (Automatic detection) + profiles
    juce::TextButton  browseButton, settingsButton;
    juce::ProgressBar progressBar;
    juce::TextEditor  logTextEditor;

    double progress = 0.0;
    bool   isDragging = false;

    juce::File outputDirectory;

    // Converters
    Vst2Parser           vst2;
    Vst3Parser           vst3;
    UHeParser            uhe;
    UnifyPatchGenerator  unifyGenerator;
    GuruFileGenerator    guruGenerator;
    PluginProfileFactory profileFactory;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};