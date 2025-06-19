#pragma once
#include "ray.h"
#include "ui_elements.h"
#include <cstring>

struct GameState;  // Forward declare

struct ExitConfirmationWindow : DraggableWindow {
  char shaderSettingsBoxText[32];
  char ExitConfirmationWindowText[32];
  char YesButtonText[32];
  char NoButtonText[32];
  char AreYouSureLabelText[32];

  bool* exitWindow; // pointer to state.exitWindow

  Rectangle recs[4];
  Rectangle layout[4] = {
    {-64, -50, 128, 96},
    {-56, 06, 56, 32},
    {0, 06, 56, 32},
    {-40, -26, 88, 32}
  };

  ExitConfirmationWindow() {
    layoutRecs = recs;
    baseLayout = layout;
    numRecs = 4;
    active = false;

    strncpy(ExitConfirmationWindowText, "Exit Game", sizeof(ExitConfirmationWindowText)-1);
    strncpy(YesButtonText, "YES", sizeof(YesButtonText)-1);
    strncpy(NoButtonText, "NO", sizeof(NoButtonText)-1);
    strncpy(AreYouSureLabelText, "Are you sure?", sizeof(AreYouSureLabelText)-1);
  }

  void Draw();
  void Init(GameState& state);
  void Yes();
  void No();
};
