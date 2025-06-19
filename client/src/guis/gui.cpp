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
  bool anyOpen = false;
  for(DraggableWindow* i : draggableWindows) if(i->active) anyOpen = true;

  if(anyOpen) {
    bool draggingAnyWindow = false;
    for (DraggableWindow* i : draggableWindows) {
      if(i->isDragging) {
        draggingAnyWindow = true;
        break;
      }
    }

    // Check for clicks and determine which windows should process input
    Vector2 mousePos = GetMousePosition();
    bool clickThisFrame = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    // Reset all windows' input processing
    for (DraggableWindow* i : draggableWindows) {
      i->shouldProcessInput = false;
    }
    mainMenu.shouldProcessInput = false;

    // Sort windows by z-index before drawing
    // Simple bubble sort since we'll have few windows
    // TODO: Refactor to use heap instead
    for (uint32_t i = 0; i < draggableWindows.size() - 1; i++) {
      for (uint32_t j = 0; j < draggableWindows.size() - i - 1; j++) {
        if (draggableWindows[j]->zIndex > draggableWindows[j + 1]->zIndex) {
          DraggableWindow* temp = draggableWindows[j];
          draggableWindows[j] = draggableWindows[j + 1];
          draggableWindows[j + 1] = temp;
        }
      }
    }

    // First check if mouse is over any active draggable window
    bool mouseOverWindow = false;
    for (int i = draggableWindows.count - 1; i >= 0; i--) {
      if (!draggableWindows[i]->active) continue;

      bool mouseOverThisWindow = CheckCollisionPointRec(mousePos, draggableWindows[i]->layoutRecs[0]);

      if (mouseOverThisWindow) {
        if (clickThisFrame) BringToFront(draggableWindows[i]);
        if (!draggingAnyWindow) draggableWindows[i]->shouldProcessInput = true;
        mouseOverWindow = true;
        break;
      }

      // Main menu can only process input if no window is under the mouse
      mainMenu.shouldProcessInput = !mouseOverWindow && !draggingAnyWindow;
    }
  } else {
    mainMenu.shouldProcessInput = true;
  }

  // Update all windows
  mainMenu.Update();
  for (DraggableWindow* i : draggableWindows) {
    i->Update(*uiScale);
  }
}

void GUI::Draw() {
  mainMenu.Draw();

  // Draw draggable windows in z-order
  for (uint32_t i = 0; i < draggableWindows.count; i++) {
    draggableWindows[i]->Draw();
  }
}

void GUI::Open(DraggableWindow* window) {
  window->anchor = {
    static_cast<float>(GetScreenWidth()) / 2.0f,
    static_cast<float>(GetScreenHeight()) / 2.0f
  };
  window->active = true;
  BringToFront(window);
}

void GUI::Close(DraggableWindow* window) {
  window->active = false;
}

void GUI::Toggle(DraggableWindow* window) {
  if(!window->active) Open(window);
  else Close(window);
}

void GUI::CloseTopWindowOrOpenSettings() {
  // We know that draggableWindows is already sorted by zIndex because GUI::Update does this every frame
  DraggableWindow* topMost = nullptr;
  for (int i = draggableWindows.count - 1; i >= 0; i--) {
    if(draggableWindows[i]->active) {
      topMost = draggableWindows[i];
      break;
    }
  }
  if(topMost != nullptr) Close(topMost);
  else Open(&settingsMenu);
}
