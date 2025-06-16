#include "shader_settings_menu.h"
#include "game_state.h"
#include "shaders.h"

void ShaderSettingsMenu::Init(GameState& state) {
  shaders = state.renderResources.shaders;
  anchor = {
    static_cast<float>(GetScreenWidth()) / 2.0f,
    static_cast<float>(GetScreenHeight()) / 2.0f
  };
}

void ShaderSettingsMenu::Draw() {
  if(!active) return;

  active = !GuiWindowBox(layoutRecs[0], shaderSettingsBoxText);
  GuiSliderBar(layoutRecs[1], selfShadowIntensitySliderText, NULL, &shaders->selfShadowIntensity, 0, 2.0f);
  GuiSliderBar(layoutRecs[8], specularPowerSliderText, NULL, &shaders->specularPower, 0, 32.0f);
  GuiSliderBar(layoutRecs[2], poissonDiskScaleSliderText, NULL, &shaders->poissonDiskScale, 0, 3000.0f);
  GuiSliderBar(layoutRecs[3], shadowBiasFactorSliderText, NULL, &shaders->shadowBiasFactor, 0, 0.00016f);
  GuiSliderBar(layoutRecs[4], shadowBiasMinSliderText, NULL, &shaders->shadowBiasMin, 0, 0.000016f);
  GuiSpinner(layoutRecs[5], poissonSamplesSpinnerText, &shaders->poissonSamples, 0, 12, false);
  GuiSliderBar(layoutRecs[6], shadowDarknessSliderText, NULL, &shaders->shadowDarkness, 0, 0.32f);
  GuiSliderBar(layoutRecs[7], ambientDivisionSliderText, NULL, &shaders->ambientDivision, 0, 40.0f);
  GuiSliderBar(layoutRecs[9], gammaSliderText, NULL, &shaders->gamma, 0, 4.4f);
  if(GuiButton(layoutRecs[10], saveButtonText)) SaveButton();
}

void ShaderSettingsMenu::SaveButton() {
  rini_config config = rini_load_config(NULL);
  shaders->save(config);
  rini_save_config(config, "shaders.ini");
  rini_unload_config(&config);
}
