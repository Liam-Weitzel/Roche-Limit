#pragma once

#include "game_state.h"

void UpdateSettings(GameState& state);
void InitSettings(GameState& state);
void DrawSettingsMenu(GameState& state);
void CancelButton(GUI& gui, Settings& s);                // Button: cancelButton logic
void ApplyButton(GameState& state);                      // Button: applyButton logic
