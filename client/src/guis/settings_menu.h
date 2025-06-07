#pragma once

#include "game_state.h"

void DrawSettingsMenu(GameState& state);
void CancelButton(GUI& gui, Settings& s);                // Button: cancelButton logic
void ApplyButton(GameState& state);                      // Button: applyButton logic
