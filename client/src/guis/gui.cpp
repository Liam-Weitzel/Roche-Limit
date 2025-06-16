#include "gui.h"
#include "game_state.h"

void GUI::BringToFront(DraggableWindow* window) {
  int maxZ = 0;
  for (uint32_t i = 0; i < draggableWindows.size(); i++) {
    if (draggableWindows[i]->zIndex > maxZ) {
      maxZ = draggableWindows[i]->zIndex;
    }
  }
  window->zIndex = maxZ + 1;
}

void GUI::Init(GameState& state) {
  uiScale = &state.settings.uiScale;
  mainMenu.Init(state);
  settingsMenu.Init(state);
  shaderSettingsMenu.Init(state);
  exitConfirmationWindow.Init(state);
}

void GUI::Update() {
  mainMenu.Update();
  for (DraggableWindow* i : draggableWindows) i->Update(*uiScale);
}

void GUI::Draw() {
  mainMenu.Draw();

  // Sort windows by z-index before drawing
  // Simple bubble sort since we'll have few windows
  for (uint32_t i = 0; i < draggableWindows.size() - 1; i++) {
    for (uint32_t j = 0; j < draggableWindows.size() - i - 1; j++) {
      if (draggableWindows[j]->zIndex > draggableWindows[j + 1]->zIndex) {
        DraggableWindow* temp = draggableWindows[j];
        draggableWindows[j] = draggableWindows[j + 1];
        draggableWindows[j + 1] = temp;
      }
    }
  }

  // Draw windows in z-order
  for (uint32_t i = 0; i < draggableWindows.size(); i++) {
    if (draggableWindows[i]->active) {
      if (draggableWindows[i] == &settingsMenu) {
        settingsMenu.Draw();
      }
      else if (draggableWindows[i] == &shaderSettingsMenu) {
        shaderSettingsMenu.Draw();
      }
      else if (draggableWindows[i] == &exitConfirmationWindow) {
        exitConfirmationWindow.Draw();
      }
    }
  }
}
