#pragma once
#include "ui_elements.h"
#include "ray.h"
#include <cstring>

struct GameState;  // Forward declare
struct Shaders; // Forward declare

struct ShaderSettingsMenu : DraggableWindow {
  char shaderSettingsBoxText[32];
  char selfShadowIntensitySliderText[32];
  char poissonDiskScaleSliderText[32];
  char shadowBiasFactorSliderText[32];
  char shadowBiasMinSliderText[32];
  char poissonSamplesSpinnerText[32];
  char shadowDarknessSliderText[32];
  char ambientDivisionSliderText[32];
  char specularPowerSliderText[32];
  char gammaSliderText[32];
  char saveButtonText[32];

  Rectangle recs[11];
  Rectangle layout[11] = {
    {-136, -136, 272, 272},  // WindowBox
    {8, -80, 120, 16},       // SelfShadowIntensity
    {8, -32, 120, 16},       // PoissonDiskScale
    {8, -8, 120, 16},        // ShadowBiasFactor
    {8, 16, 120, 16},        // ShadowBiasMin
    {8, 40, 120, 16},        // PoissonSamples
    {8, 64, 120, 16},        // ShadowDarkness
    {8, 88, 120, 16},        // AmbientDivision
    {8, -56, 120, 16},       // SpecularPower
    {8, 112, 120, 16},       // Gamma
    {8, -104, 120, 16}        // Save
  };

  Shaders* shaders;

  ShaderSettingsMenu() {
    layoutRecs = recs;
    baseLayout = layout;
    numRecs = 11;
    active = true;

    strncpy(shaderSettingsBoxText, "Shadow Shader Settings", sizeof(shaderSettingsBoxText)-1);
    strncpy(selfShadowIntensitySliderText, "SelfShadowIntensity", sizeof(selfShadowIntensitySliderText)-1);
    strncpy(poissonDiskScaleSliderText, "PoissonDiskScale", sizeof(poissonDiskScaleSliderText)-1);
    strncpy(shadowBiasFactorSliderText, "ShadowBiasFactor", sizeof(shadowBiasFactorSliderText)-1);
    strncpy(shadowBiasMinSliderText, "ShadowBiasMin", sizeof(shadowBiasMinSliderText)-1);
    strncpy(poissonSamplesSpinnerText, "PoissonSamples", sizeof(poissonSamplesSpinnerText)-1);
    strncpy(shadowDarknessSliderText, "ShadowDarkness", sizeof(shadowDarknessSliderText)-1);
    strncpy(ambientDivisionSliderText, "AmbientDivision", sizeof(ambientDivisionSliderText)-1);
    strncpy(specularPowerSliderText, "SpecularPower", sizeof(specularPowerSliderText)-1);
    strncpy(gammaSliderText, "Gamma", sizeof(gammaSliderText)-1);
    strncpy(saveButtonText, "Save", sizeof(saveButtonText)-1);
  }

  void Init(GameState& state);
  void Draw();
  void SaveButton();
};
