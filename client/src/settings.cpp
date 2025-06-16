#include "settings.h"
#include "gui.h"
#include "ray.h"
#include "game_state.h"

void Settings::save(rini_config& config) {
  rini_set_config_comment_line(&config, NULL);
  rini_set_config_comment_line(&config, "Settings");
  rini_set_config_comment_line(&config, NULL);
  rini_set_config_comment_line(&config, "NOTE: This file is loaded at application startup,");
  rini_set_config_comment_line(&config, "if file is not found, default values are applied");
  rini_set_config_comment_line(&config, NULL);

  rini_set_config_value(&config, "MUSIC_VOLUME", musicVolume, "Music volume");
  rini_set_config_value(&config, "SFX_VOLUME", sfxVolume, "Sound effects volumes");
  rini_set_config_value(&config, "DISPLAY_MODE", displayMode, "Borderless; Windowed; Fullscreen");
  rini_set_config_value(&config, "FPS_LIMIT", fpsLimit, "The fps that the game should not exceed");
  rini_set_config_value(&config, "UI_STYLE", uiStyle, "UI visual style selected");
  rini_set_config_value(&config, "UI_SCALE", uiScale, "UI scale multiplier");
  rini_set_config_value(&config, "MSAA", msaa, "Pass FLAG_MSAA_4X_HINT as window startup config flag");
  rini_set_config_value(&config, "HDPI", hdpi, "Pass FLAG_WINDOW_HIGHDPI as window startup config flag");
  rini_set_config_value(&config, "VSYNC", vsync, "Pass FLAG_VSYNC_HINT as window startup config flag");
  rini_set_config_value(&config, "ALWAYS_RUN", alwaysRun, "Pass FLAG_WINDOW_ALWAYS_RUN as window startup config flag");
  rini_set_config_value(&config, "INTERLACED", interlaced, "Pass FLAG_INTERLACED_HINT as startup config flag");
}

void Settings::load(rini_config& config) {
  fpsLimit = rini_get_config_value(config, "FPS_LIMIT");
  uiStyle = rini_get_config_value(config, "UI_STYLE");
  uiScale = (float)rini_get_config_value(config, "UI_SCALE");
  displayMode = rini_get_config_value(config, "DISPLAY_MODE");
  sfxVolume = (float)rini_get_config_value(config, "SFX_VOLUME");
  musicVolume = (float)rini_get_config_value(config, "MUSIC_VOLUME");
  msaa = rini_get_config_value(config, "MSAA");
  hdpi = rini_get_config_value(config, "HDPI");
  vsync = rini_get_config_value(config, "VSYNC");
  alwaysRun = rini_get_config_value(config, "ALWAYS_RUN");
  interlaced = rini_get_config_value(config, "INTERLACED");
}

unsigned int Settings::initFlagMask() {
  unsigned int flags = 0;
  if (msaa) flags |= FLAG_MSAA_4X_HINT;
  if (hdpi) flags |= FLAG_WINDOW_HIGHDPI;
  if (vsync) flags |= FLAG_VSYNC_HINT;
  if (alwaysRun) flags |= FLAG_WINDOW_ALWAYS_RUN;
  if (interlaced) flags |= FLAG_INTERLACED_HINT;
  return flags;
}

void Settings::Update() {
  switch (displayMode) {
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
  if(gui->loaded_style != uiStyle) {
    if (strcmp(gui->styles[uiStyle], "default") == 0) GuiLoadStyleDefault();
    else {
      int idStyle = rresGetResourceId(*dir, gui->styles[uiStyle]);
      rresResourceChunk chunkStyle = rresLoadResourceChunk("resources.rres", idStyle);
      if(UnpackResourceChunk(&chunkStyle) == RRES_SUCCESS) {
        LoadGuiStyle((const unsigned char*) chunkStyle.data.raw, chunkStyle.info.baseSize);
      }
      rresUnloadResourceChunk(chunkStyle);
    }
    gui->loaded_style = uiStyle;
  }

  // Update target fps
  SetTargetFPS(fpsLimit);
}

void Settings::Init(GameState& state) {
  gui = state.renderResources.gui;
  dir = state.renderResources.dir;
}
