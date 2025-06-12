#include "shader_settings_menu.h"
#include "game_state.h"
#include "ray.h"
#include <cmath>

void UpdateShaderSettings(GameState& state) {
  Shaders& shaders = *state.renderResources.shaders;
  ShaderSettingsMenu& ssm = state.renderResources.gui->shaderSettingsMenu;
  if(!ssm.active) return;

  shaders.selfShadowIntensity = ssm.selfShadowIntensitySliderValue;
  shaders.specularPower = ssm.specularPowerSliderValue;
  shaders.poissonDiskScale = ssm.poissonDiskScaleSliderValue;
  shaders.shadowBiasFactor = ssm.shadowBiasFactorSliderValue;
  shaders.shadowBiasMin = ssm.shadowBiasMinSliderValue;
  shaders.poissonSamples = ssm.poissonSamplesSliderValue;
  shaders.shadowDarkness = ssm.shadowDarknessSliderValue;
  shaders.ambientDivision = ssm.ambientDivisionSliderValue;
  shaders.gamma = ssm.gammaSliderValue;
  SetShaderValue(shaders.shadowShader, shaders.selfShadowIntensityLoc, &shaders.selfShadowIntensity, SHADER_UNIFORM_FLOAT);
  SetShaderValue(shaders.shadowShader, shaders.specularPowerLoc, &shaders.specularPower, SHADER_UNIFORM_FLOAT);
  SetShaderValue(shaders.shadowShader, shaders.poissonDiskScaleLoc, &shaders.poissonDiskScale, SHADER_UNIFORM_FLOAT);
  SetShaderValue(shaders.shadowShader, shaders.shadowBiasFactorLoc, &shaders.shadowBiasFactor, SHADER_UNIFORM_FLOAT);
  SetShaderValue(shaders.shadowShader, shaders.shadowBiasMinLoc, &shaders.shadowBiasMin, SHADER_UNIFORM_FLOAT);
  SetShaderValue(shaders.shadowShader, shaders.poissonSamplesLoc, &shaders.poissonSamples, SHADER_UNIFORM_INT);
  SetShaderValue(shaders.shadowShader, shaders.shadowDarknessLoc, &shaders.shadowDarkness, SHADER_UNIFORM_FLOAT);
  SetShaderValue(shaders.shadowShader, shaders.ambientDivisionLoc, &shaders.ambientDivision, SHADER_UNIFORM_FLOAT);
  SetShaderValue(shaders.shadowShader, shaders.gammaLoc, &shaders.gamma, SHADER_UNIFORM_FLOAT);
}

void InitShaderSettings(GameState& state) {
  Vector2 anchor = {
    static_cast<float>(GetScreenWidth()) / 2.0f,
    static_cast<float>(GetScreenHeight()) / 2.0f
  };
  state.renderResources.gui->shaderSettingsMenu.anchor01 = anchor;
  // state.renderResources.gui->shaderSettingsMenu.gammaSliderValue = state.renderResources.shaders->gamma;
  // state.renderResources.gui->shaderSettingsMenu.specularPowerSliderValue = state.renderResources.shaders->specularPower;
  // state.renderResources.gui->shaderSettingsMenu.poissonSamplesSliderValue = state.renderResources.shaders->poissonSamples;
  // state.renderResources.gui->shaderSettingsMenu.shadowDarknessSliderValue = state.renderResources.shaders->shadowDarkness;
  // state.renderResources.gui->shaderSettingsMenu.ambientDivisionSliderValue = state.renderResources.shaders->ambientDivision;
  // state.renderResources.gui->shaderSettingsMenu.shadowBiasMinSliderValue = state.renderResources.shaders->shadowBiasMin;
  // state.renderResources.gui->shaderSettingsMenu.shadowBiasFactorSliderValue = state.renderResources.shaders->shadowBiasFactor;
  // state.renderResources.gui->shaderSettingsMenu.selfShadowIntensitySliderValue = state.renderResources.shaders->selfShadowIntensity;
}

void DrawShaderSettingsMenu(GameState& state) {
  ShaderSettingsMenu& ssm = state.renderResources.gui->shaderSettingsMenu;
  if(!ssm.active) return;

  ssm.active = !GuiWindowBox(ssm.layoutRecs[0], ssm.shaderSettingsBoxText);
  GuiSliderBar(ssm.layoutRecs[1], ssm.selfShadowIntensitySliderText, NULL, &ssm.selfShadowIntensitySliderValue, 0, 2.0f);
  GuiSliderBar(ssm.layoutRecs[8], ssm.specularPowerSliderText, NULL, &ssm.specularPowerSliderValue, 0, 32.0f);
  GuiSliderBar(ssm.layoutRecs[2], ssm.poissonDiskScaleSliderText, NULL, &ssm.poissonDiskScaleSliderValue, 0, 3000.0f);
  GuiSliderBar(ssm.layoutRecs[3], ssm.shadowBiasFactorSliderText, NULL, &ssm.shadowBiasFactorSliderValue, 0, 0.00016f);
  GuiSliderBar(ssm.layoutRecs[4], ssm.shadowBiasMinSliderText, NULL, &ssm.shadowBiasMinSliderValue, 0, 0.000016f);
  GuiSliderBar(ssm.layoutRecs[5], ssm.poissonSamplesSliderText, NULL, &ssm.poissonSamplesSliderValue, 0, 12.0f);
  GuiSliderBar(ssm.layoutRecs[6], ssm.shadowDarknessSliderText, NULL, &ssm.shadowDarknessSliderValue, 0, 0.32f);
  GuiSliderBar(ssm.layoutRecs[7], ssm.ambientDivisionSliderText, NULL, &ssm.ambientDivisionSliderValue, 0, 40.0f);
  GuiSliderBar(ssm.layoutRecs[9], ssm.gammaSliderText, NULL, &ssm.gammaSliderValue, 0, 4.4f);
  if(GuiButton(ssm.layoutRecs[10], ssm.saveButtonText)) SaveButton(state);
}

void SaveButton(GameState& state) {
  ShaderSettingsMenu& ssm = state.renderResources.gui->shaderSettingsMenu;
  rini_config config = rini_load_config(NULL);

  rini_set_config_comment_line(&config, NULL);
  rini_set_config_comment_line(&config, "Shader values printout for debugging");
  rini_set_config_comment_line(&config, NULL);

  rini_set_config_value_float(&config, "SELF_SHADOW_INTENSITY", ssm.selfShadowIntensitySliderValue, "");
  rini_set_config_value_float(&config, "SPECULAR_POWER", ssm.specularPowerSliderValue, "");
  rini_set_config_value_float(&config, "POISSON_DISK_SCALE", ssm.poissonDiskScaleSliderValue, "");
  rini_set_config_value_float(&config, "SHADOW_BIAS_FACTOR", ssm.shadowBiasFactorSliderValue, "");
  rini_set_config_value_float(&config, "SHADOW_BIAS_MIN", ssm.shadowBiasMinSliderValue, "");
  rini_set_config_value_float(&config, "POISSON_SAMPLES", ssm.poissonSamplesSliderValue, "");
  rini_set_config_value_float(&config, "SHADOW_DARKNESS", ssm.shadowDarknessSliderValue, "");
  rini_set_config_value_float(&config, "AMBIENT_DIVISION", ssm.ambientDivisionSliderValue, "");
  rini_set_config_value_float(&config, "GAMMA", ssm.gammaSliderValue, "");

  rini_save_config(config, "shaders.ini");

  rini_unload_config(&config);
}
