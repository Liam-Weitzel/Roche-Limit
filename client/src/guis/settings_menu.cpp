#include "settings_menu.h"
#include "input.h"
#include "ray.h"
#include "game_state.h"
#include "settings.h"

void SettingsMenu::Init(GameState& state) {
  im = &state.inputHandler.input;
  s = &state.settings;
  gui = state.renderResources.gui;
  dir = state.renderResources.dir;

  anchor = {
    static_cast<float>(GetScreenWidth()) / 2.0f,
    static_cast<float>(GetScreenHeight()) / 2.0f
  };
  sfxSliderBarValue = s->sfxVolume;
  musicSliderBarValue = s->musicVolume;
  displayDropdownBoxActive = s->displayMode;
  fpsValueBoxValue = s->fpsLimit;
  uiStyleSpinnerValue = s->uiStyle;
  uiScaleSliderValue = s->uiScale;
  HDPICheckboxValue = s->hdpi;
  MSAACheckboxValue = s->msaa;
  VSYNCCheckboxValue = s->vsync;
  AlwaysRunCheckboxValue = s->alwaysRun;
  InterlacedCheckboxValue = s->interlaced;
}

void SettingsMenu::Draw() {
  if(!active) return;

  if (displayDropdownBoxEditMode) GuiLock();

  active = !GuiWindowBox(layoutRecs[0], settingsWindowBoxText);
  if (GuiSpinner(layoutRecs[1], uiStyleSpinnerText, &uiStyleSpinnerValue, 0, 10, uiStyleSpinnerEditMode)) uiStyleSpinnerEditMode = !uiStyleSpinnerEditMode;
  GuiSliderBar(layoutRecs[2], musicSliderBarText, NULL, &musicSliderBarValue, 0, 100);
  GuiSliderBar(layoutRecs[3], sfxSliderBarText, NULL, &sfxSliderBarValue, 0, 100);
  if (GuiValueBox(layoutRecs[4], fpsValueBoxText, &fpsValueBoxValue, 10, 1000, fpsValueBoxEditMode)) fpsValueBoxEditMode = !fpsValueBoxEditMode;
  GuiLine(layoutRecs[5], LineText);
  GuiSlider(layoutRecs[6], uiScaleSliderText, NULL, &uiScaleSliderValue, 0, 100);
  GuiLabel(layoutRecs[8], displayLabelText);
  if (GuiDropdownBox(layoutRecs[7], displayDropdownBoxText, &displayDropdownBoxActive, displayDropdownBoxEditMode)) displayDropdownBoxEditMode = !displayDropdownBoxEditMode;
  GuiLine(layoutRecs[9], Line2Text);
  if (GuiButton(layoutRecs[10], cancelButtonText)) CancelButton(); 
  if (GuiButton(layoutRecs[11], applyButtonText)) ApplyButton(); 
  GuiCheckBox(layoutRecs[12], MSAACheckboxText, &MSAACheckboxValue);
  GuiCheckBox(layoutRecs[13], HDPICheckboxText, &HDPICheckboxValue);
  GuiCheckBox(layoutRecs[14], VSYNCCheckboxText, &VSYNCCheckboxValue);
  GuiCheckBox(layoutRecs[15], AlwaysRunCheckboxText, &AlwaysRunCheckboxValue);
  GuiCheckBox(layoutRecs[16], InterlacedCheckboxText, &InterlacedCheckboxValue);
  GuiCheckBox(layoutRecs[17], PlaceHolder1CheckboxText, &PlaceHolder1CheckboxValue);
  GuiCheckBox(layoutRecs[18], PlaceHolder2CheckboxText, &PlaceHolder2CheckboxValue);
  GuiCheckBox(layoutRecs[19], PlaceHolder3CheckboxText, &PlaceHolder3CheckboxValue);
  GuiCheckBox(layoutRecs[20], PlaceHolder4CheckboxText, &PlaceHolder4CheckboxValue);
  GuiCheckBox(layoutRecs[21], PlaceHolder5CheckboxText, &PlaceHolder5CheckboxValue);

  GuiUnlock();
}

void SettingsMenu::CancelButton() {
  rini_config config = rini_load_config("settings.ini");

  s->musicVolume = rini_get_config_value(config, "MUSIC_VOLUME");
  s->sfxVolume = rini_get_config_value(config, "SFX_VOLUME");
  s->displayMode = rini_get_config_value(config, "DISPLAY_MODE");
  s->fpsLimit = rini_get_config_value(config, "FPS_LIMIT");
  s->uiStyle = rini_get_config_value(config, "UI_STYLE");
  s->uiScale = rini_get_config_value(config, "UI_SCALE");

  rini_unload_config(&config);
  active = false;
  dirty = true;
}

void SettingsMenu::ApplyButton() {
  s->fpsLimit = fpsValueBoxValue;
  s->uiStyle = uiStyleSpinnerValue;
  s->uiScale = uiScaleSliderValue;
  s->displayMode = displayDropdownBoxActive;
  s->sfxVolume = sfxSliderBarValue;
  s->musicVolume = musicSliderBarValue;
  s->msaa = MSAACheckboxValue;
  s->hdpi = HDPICheckboxValue;
  s->vsync = VSYNCCheckboxValue;
  s->alwaysRun = AlwaysRunCheckboxValue;
  s->interlaced = InterlacedCheckboxValue;

  rini_config config = rini_load_config(NULL);
  s->save(config);
  im->save(config);
  rini_save_config(config, "settings.ini");
  rini_unload_config(&config);

  s->Update();

  dirty = true;
}
