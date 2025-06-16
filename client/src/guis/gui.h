#pragma once
#include "exit_confirmation_window.h"
#include "main_menu.h"
#include "settings_menu.h"
#include "shader_settings_menu.h"
#include "utils.h"
#include <cstring>

struct GameState;  // Forward declare

#define MAX_NUMBER_OF_WINDOWS 30

struct GUI { // Permanent
  MainMenu mainMenu;
  SettingsMenu settingsMenu;
  ShaderSettingsMenu shaderSettingsMenu;
  ExitConfirmationWindow exitConfirmationWindow;
  ArrayCT<DraggableWindow*, MAX_NUMBER_OF_WINDOWS> draggableWindows;

  char styles[12][32];
  int loaded_style = 0;

  float* uiScale; // Pointer to state.settings.uiScale;

  GUI() {
    const char* tmp_styles[12] = {
      "default",
      "ash.rgs",
      "bluish.rgs",
      "candy.rgs",
      "cherry.rgs",
      "cyber.rgs",
      "dark.rgs",
      "enefete.rgs",
      "jungle.rgs",
      "lavanda.rgs",
      "sunny.rgs",
      "terminal.rgs"
    };

    for (int i = 0; i < 12; ++i) {
      strncpy(styles[i], tmp_styles[i], sizeof(styles[i]) - 1);
      styles[i][sizeof(styles[i]) - 1] = '\0';
    }

    // Initialize draggable windows array
    draggableWindows.add(&settingsMenu);
    draggableWindows.add(&shaderSettingsMenu);
    draggableWindows.add(&exitConfirmationWindow);

    // Set initial z-indices
    for (uint32_t i = 0; i < draggableWindows.size(); i++) {
      draggableWindows[i]->zIndex = i;
    }
  }

  void Init(GameState& state);
  void BringToFront(DraggableWindow* window);
  void Update();
  void Draw();
};
