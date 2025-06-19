#include "input.h"

#include "gui.h"
#include "game_state.h"

void InputHandler::init(GameState& state) {
  gui = state.renderResources.gui;
}

void InputHandler::update() {
  if (WindowShouldClose()) gui->Open(&gui->exitConfirmationWindow);
  Command cmd = input.getCommand();
  switch (cmd) {
    case Command::CloseTopWindowOrOpenSettings:
      gui->CloseTopWindowOrOpenSettings();
      break;

    case Command::ToggleShaderSettings:
      gui->Toggle(&gui->shaderSettingsMenu);
      break;

    case Command::None:
      break;
  }
}
