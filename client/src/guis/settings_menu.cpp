#include "settings_menu.h"
#include "game_state.h"
#include "ray.h"
#include <cmath>

void UpdateSettings(GameState& state) {
  // Update UI Style
  if(state.renderResources.gui->loaded_style != state.settings.uiStyle) {
    if (strcmp(state.renderResources.gui->styles[state.settings.uiStyle], "default") == 0) GuiLoadStyleDefault();
    else {
        int idStyle = rresGetResourceId(*state.renderResources.dir, state.renderResources.gui->styles[state.settings.uiStyle]);
        rresResourceChunk chunkStyle = rresLoadResourceChunk("resources.rres", idStyle);
        if(UnpackResourceChunk(&chunkStyle) == RRES_SUCCESS) {
            LoadGuiStyle((const unsigned char*) chunkStyle.data.raw, chunkStyle.info.baseSize);
        }
        rresUnloadResourceChunk(chunkStyle);
    }
    state.renderResources.gui->loaded_style = state.settings.uiStyle;
  }

  // Update target fps
  SetTargetFPS(state.settings.fpsLimit);
}

void DrawSettingsMenu(GameState& state) {
  GUI& gui = *state.renderResources.gui;
  SettingsMenu& sm = gui.settingsMenu;
  Settings& s = state.settings;

  if(!sm.active) return;

  if (sm.displayDropdownBoxEditMode) GuiLock();

  sm.active = !GuiWindowBox(sm.layoutRecs[0], sm.settingsWindowBoxText);
  if (GuiSpinner(sm.layoutRecs[1], sm.uiStyleSpinnerText, &sm.uiStyleSpinnerValue, 0, 10, sm.uiStyleSpinnerEditMode)) sm.uiStyleSpinnerEditMode = !sm.uiStyleSpinnerEditMode;
  GuiSliderBar(sm.layoutRecs[2], sm.musicSliderBarText, NULL, &sm.musicSliderBarValue, 0, 100);
  GuiSliderBar(sm.layoutRecs[3], sm.sfxSliderBarText, NULL, &sm.sfxSliderBarValue, 0, 100);
  if (GuiValueBox(sm.layoutRecs[4], sm.fpsValueBoxText, &sm.fpsValueBoxValue, 10, 1000, sm.fpsValueBoxEditMode)) sm.fpsValueBoxEditMode = !sm.fpsValueBoxEditMode;
  GuiLine(sm.layoutRecs[5], sm.LineText);
  GuiSlider(sm.layoutRecs[6], sm.uiScaleSliderText, NULL, &sm.uiScaleSliderValue, 0, 100);
  GuiLabel(sm.layoutRecs[8], sm.displayLabelText);
  if (GuiDropdownBox(sm.layoutRecs[7], sm.displayDropdownBoxText, &sm.displayDropdownBoxActive, sm.displayDropdownBoxEditMode)) sm.displayDropdownBoxEditMode = !sm.displayDropdownBoxEditMode;
  GuiLine(sm.layoutRecs[9], sm.Line2Text);
  if (GuiButton(sm.layoutRecs[10], sm.cancelButtonText)) CancelButton(gui, s); 
  if (GuiButton(sm.layoutRecs[11], sm.applyButtonText)) ApplyButton(state); 

  GuiUnlock();
}

void CancelButton(GUI& gui, Settings& s) {
  rini_config config = rini_load_config("settings.ini");

  s.musicVolume = rini_get_config_value(config, "MUSIC_VOLUME");
  s.sfxVolume = rini_get_config_value(config, "SFX_VOLUME");
  s.displayMode = rini_get_config_value(config, "DISPLAY_MODE");
  s.fpsLimit = rini_get_config_value(config, "FPS_LIMIT");
  s.uiStyle = rini_get_config_value(config, "UI_STYLE");
  s.uiScale = rini_get_config_value(config, "UI_SCALE");

  rini_unload_config(&config);
  gui.settingsMenu.active = false;
  gui.settingsMenu.dirty = true;
  gui.mainMenu.dirty = true;
}

void ApplyButton(GameState& state) {
  GUI& gui = *state.renderResources.gui;
  Settings& s = state.settings;

  s.fpsLimit = gui.settingsMenu.fpsValueBoxValue;
  s.uiStyle = gui.settingsMenu.uiStyleSpinnerValue;
  s.uiScale = gui.settingsMenu.uiScaleSliderValue;
  s.displayMode = gui.settingsMenu.displayDropdownBoxActive;
  s.sfxVolume = gui.settingsMenu.sfxSliderBarValue;
  s.musicVolume = gui.settingsMenu.musicSliderBarValue;

  rini_config config = rini_load_config(NULL);

  rini_set_config_comment_line(&config, NULL);
  rini_set_config_comment_line(&config, "Settings");
  rini_set_config_comment_line(&config, NULL);
  rini_set_config_comment_line(&config, "NOTE: This file is loaded at application startup,");
  rini_set_config_comment_line(&config, "if file is not found, default values are applied");
  rini_set_config_comment_line(&config, NULL);

  rini_set_config_value(&config, "MUSIC_VOLUME", s.musicVolume, "Music volume");
  rini_set_config_value(&config, "SFX_VOLUME", s.sfxVolume, "Sound effects volumes");
  rini_set_config_value(&config, "DISPLAY_MODE", s.displayMode, "Borderless; Windowed; Fullscreen");
  rini_set_config_value(&config, "FPS_LIMIT", s.fpsLimit, "The fps that the game should not exceed");
  rini_set_config_value(&config, "UI_STYLE", s.uiStyle, "UI visual style selected");
  rini_set_config_value(&config, "UI_SCALE", s.uiScale, "UI scale multiplier");

  rini_save_config(config, "settings.ini");

  rini_unload_config(&config);

  UpdateSettings(state);

  gui.settingsMenu.dirty = true;
  gui.shaderSettingsMenu.dirty = true;
  gui.mainMenu.dirty = true;
}
