#include "settings_menu.h"
#include "game_state.h"
#include "ray.h"
#include <cmath>

void UpdateSettings(GameState& state) {
  switch (state.settings.displayMode) {
    case 0: // windowed
      ClearWindowState(FLAG_FULLSCREEN_MODE | FLAG_BORDERLESS_WINDOWED_MODE | FLAG_WINDOW_MAXIMIZED);
      SetWindowState(FLAG_WINDOW_RESIZABLE);
      SetWindowSize(1280, 720);
      break;

    case 1: // borderless
      ClearWindowState(FLAG_FULLSCREEN_MODE | FLAG_WINDOW_RESIZABLE);
      SetWindowState(FLAG_BORDERLESS_WINDOWED_MODE | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
      break;

    case 2: // fullscreen
      ClearWindowState(FLAG_BORDERLESS_WINDOWED_MODE | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
      SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
      SetWindowState(FLAG_FULLSCREEN_MODE);
      break;
  }

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

void InitSettings(GameState& state) {
  Vector2 anchor = {
    static_cast<float>(GetScreenWidth()) / 2.0f,
    static_cast<float>(GetScreenHeight()) / 2.0f
  };
  state.renderResources.gui->settingsMenu.anchor01 = anchor;
  state.renderResources.gui->settingsMenu.sfxSliderBarValue = state.settings.sfxVolume;
  state.renderResources.gui->settingsMenu.musicSliderBarValue = state.settings.musicVolume;
  state.renderResources.gui->settingsMenu.displayDropdownBoxActive = state.settings.displayMode;
  state.renderResources.gui->settingsMenu.fpsValueBoxValue = state.settings.fpsLimit;
  state.renderResources.gui->settingsMenu.uiStyleSpinnerValue = state.settings.uiStyle;
  state.renderResources.gui->settingsMenu.uiScaleSliderValue = state.settings.uiScale;
  state.renderResources.gui->settingsMenu.HDPICheckboxValue = state.settings.hdpi;
  state.renderResources.gui->settingsMenu.MSAACheckboxValue = state.settings.msaa;
  state.renderResources.gui->settingsMenu.VSYNCCheckboxValue = state.settings.vsync;
  state.renderResources.gui->settingsMenu.AlwaysRunCheckboxValue = state.settings.alwaysRun;
  state.renderResources.gui->settingsMenu.InterlacedCheckboxValue = state.settings.interlaced;
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
  GuiCheckBox(sm.layoutRecs[12], sm.MSAACheckboxText, &sm.MSAACheckboxValue);
  GuiCheckBox(sm.layoutRecs[13], sm.HDPICheckboxText, &sm.HDPICheckboxValue);
  GuiCheckBox(sm.layoutRecs[14], sm.VSYNCCheckboxText, &sm.VSYNCCheckboxValue);
  GuiCheckBox(sm.layoutRecs[15], sm.AlwaysRunCheckboxText, &sm.AlwaysRunCheckboxValue);
  GuiCheckBox(sm.layoutRecs[16], sm.InterlacedCheckboxText, &sm.InterlacedCheckboxValue);
  GuiCheckBox(sm.layoutRecs[17], sm.PlaceHolder1CheckboxText, &sm.PlaceHolder1CheckboxValue);
  GuiCheckBox(sm.layoutRecs[18], sm.PlaceHolder2CheckboxText, &sm.PlaceHolder2CheckboxValue);
  GuiCheckBox(sm.layoutRecs[19], sm.PlaceHolder3CheckboxText, &sm.PlaceHolder3CheckboxValue);
  GuiCheckBox(sm.layoutRecs[20], sm.PlaceHolder4CheckboxText, &sm.PlaceHolder4CheckboxValue);
  GuiCheckBox(sm.layoutRecs[21], sm.PlaceHolder5CheckboxText, &sm.PlaceHolder5CheckboxValue);

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
  s.msaa = gui.settingsMenu.MSAACheckboxValue;
  s.hdpi = gui.settingsMenu.HDPICheckboxValue;
  s.vsync = gui.settingsMenu.VSYNCCheckboxValue;
  s.alwaysRun = gui.settingsMenu.AlwaysRunCheckboxValue;
  s.interlaced = gui.settingsMenu.InterlacedCheckboxValue;

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
  rini_set_config_value(&config, "MSAA", s.msaa, "Pass FLAG_MSAA_4X_HINT as window startup config flag");
  rini_set_config_value(&config, "HDPI", s.hdpi, "Pass FLAG_WINDOW_HIGHDPI as window startup config flag");
  rini_set_config_value(&config, "VSYNC", s.vsync, "Pass FLAG_VSYNC_HINT as window startup config flag");
  rini_set_config_value(&config, "ALWAYS_RUN", s.alwaysRun, "Pass FLAG_WINDOW_ALWAYS_RUN as window startup config flag");
  rini_set_config_value(&config, "INTERLACED", s.interlaced, "Pass FLAG_INTERLACED_HINT as startup config flag");

  rini_save_config(config, "settings.ini");

  rini_unload_config(&config);

  UpdateSettings(state);

  state.renderResources.gui->settingsMenu.dirty = true;
  state.renderResources.gui->shaderSettingsMenu.dirty = true;
  state.renderResources.gui->mainMenu.dirty = true;
}
