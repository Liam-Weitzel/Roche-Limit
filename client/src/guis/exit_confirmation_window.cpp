#include "exit_confirmation_window.h"
#include "ray.h"

void DrawExitConfirmationWindow(GameState& state) {
  ExitConfirmationWindow& ecw = state.renderResources.gui->exitConfirmationWindow;
  if (ecw.active) {
    ecw.active = !GuiWindowBox(ecw.layoutRecs[0], ecw.ExitConfirmationWindowText);
    if (GuiButton(ecw.layoutRecs[1], ecw.YesButtonText) || IsKeyPressed(KEY_Y) || IsKeyPressed(KEY_ENTER)) Yes(state); 
    if (GuiButton(ecw.layoutRecs[2], ecw.NoButtonText) || IsKeyPressed(KEY_N)) No(state); 
    GuiLabelButton(ecw.layoutRecs[3], ecw.AreYouSureLabelText);
  }
}

void InitExitConfirmationWindow(GameState& state) {
  Vector2 anchor = {
    static_cast<float>(GetScreenWidth()) / 2.0f,
    static_cast<float>(GetScreenHeight()) / 2.0f
  };
  state.renderResources.gui->exitConfirmationWindow.anchor01 = anchor;
}

void Yes(GameState& state) {
  state.exitWindow = true;
}

void No(GameState& state) {
  state.renderResources.gui->exitConfirmationWindow.active = false;
}
