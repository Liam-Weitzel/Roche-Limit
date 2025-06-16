#pragma once
#include "ray.h"
#include <cstring>

struct GUI;  // Forward declare
struct GameState; // Forward declare

struct Settings { // Permanent
  float musicVolume = 50.0f;
  float sfxVolume = 50.0f;
  int displayMode = 0;
  int fpsLimit = 120;
  int uiStyle = 0;
  float uiScale = 50.0f;
  bool msaa = true;
  bool hdpi = true;
  bool vsync = true;
  bool alwaysRun = true;
  bool interlaced = false;

  GUI* gui; // Pointer to state.renderResources.gui
  rresCentralDir* dir; // Pointer to state.renderResources.dir

  void save(rini_config& config);
  void load(rini_config& config);
  unsigned int initFlagMask();
  void Update();
  void Init(GameState& state);
};
