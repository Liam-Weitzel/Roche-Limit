#include "exit_confirmation_window.h"
#include "game_state.h"

void ExitConfirmationWindow::Draw() {
  if (!active) return;
  if (!shouldProcessInput) GuiLock();

  active = !GuiWindowBox(layoutRecs[0], ExitConfirmationWindowText);
  if (GuiButton(layoutRecs[1], YesButtonText) || IsKeyPressed(KEY_Y) || IsKeyPressed(KEY_ENTER)) Yes(); 
  if (GuiButton(layoutRecs[2], NoButtonText) || IsKeyPressed(KEY_N)) No(); 
  GuiLabelButton(layoutRecs[3], AreYouSureLabelText);

  GuiUnlock();
}

void ExitConfirmationWindow::Init(GameState& state) {
  anchor = {
    static_cast<float>(GetScreenWidth()) / 2.0f,
    static_cast<float>(GetScreenHeight()) / 2.0f
  };
  exitWindow = &state.exitWindow;
}

void ExitConfirmationWindow::Yes() {
  *exitWindow = true;
}

void ExitConfirmationWindow::No() {
  active = false;
}
