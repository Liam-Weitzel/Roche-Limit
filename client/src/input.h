#pragma once
#include "ray.h"
#include <cstdio>

struct GUI; // Forward declare
struct GameState; // Forward declare

struct KeyDebounce {
  bool canPress = true;

  bool handle(bool pressed) {
    if (pressed && canPress) {
      canPress = false;
      return true;
    }

    if (!canPress && !pressed) {
      canPress = true;
    }

    return false;
  }
};

enum class Command {
  None,
  CloseTopWindowOrOpenSettings,
  ToggleShaderSettings,
};

struct KeyBinding {
  int key;  // RAYLIB key code
  bool ctrl;
  bool shift;
  bool alt;
  
  KeyBinding(int k = 0, bool c = false, bool s = false, bool a = false) : key(k), ctrl(c), shift(s), alt(a) {}

  bool isPressed() const {
    return IsKeyDown(key) && 
           (!ctrl || IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) &&
           (!shift || IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) &&
           (!alt || IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT));
  }
};

struct InputManager {
  static const int MAX_BINDINGS = 32;
  KeyBinding bindings[MAX_BINDINGS];
  Command commands[MAX_BINDINGS];
  KeyDebounce debounces[MAX_BINDINGS];
  int bindingCount = 0;

  InputManager() {
    // Default bindings
    mapCommand(Command::CloseTopWindowOrOpenSettings, KeyBinding(KEY_ESCAPE));
    mapCommand(Command::ToggleShaderSettings, KeyBinding(KEY_F1));
  }

  void mapCommand(Command cmd, const KeyBinding& binding) {
    if (bindingCount >= MAX_BINDINGS) return;
    bindings[bindingCount] = binding;
    commands[bindingCount] = cmd;
    bindingCount++;
  }

  Command getCommand() {
    for (int i = 0; i < bindingCount; i++) {
      if (debounces[i].handle(bindings[i].isPressed())) {
        return commands[i];
      }
    }
    return Command::None;
  }

  const char* getCommandString(Command cmd) {
    switch(cmd) {
      case Command::CloseTopWindowOrOpenSettings: return "CLOSE_TOP_WINDOW_OR_TOGGLE_SETTINGS";
      case Command::ToggleShaderSettings: return "TOGGLE_SHADER_SETTINGS";
      case Command::None: return "NONE";
      default: return "UNKNOWN";
    }
  }

  // Save bindings to config
  void save(rini_config& config) {
    for (int i = 0; i < bindingCount; i++) {
      char value[64];
      snprintf(value, sizeof(value), "%d,%d,%d,%d", 
               bindings[i].key, 
               bindings[i].ctrl,
               bindings[i].shift,
               bindings[i].alt);
      rini_set_config_value_text(&config, getCommandString(commands[i]), value, getCommandString(commands[i]));
    }
  }

  // Load bindings from config
  void load(rini_config& config) {
    for (int i = 0; i < bindingCount; i++) {
      const char* value = rini_get_config_value_text(config, getCommandString(commands[i]));
      if (value) {
        int k, c, s, a;
        if (sscanf(value, "%d,%d,%d,%d", &k, &c, &s, &a) == 4) {
          bindings[i] = KeyBinding(k, c, s, a);  // Update in place
        }
      }
    }
  }
};

struct InputHandler {
  InputManager input;
  GUI* gui;

  void init(GameState& state);
  void update();
};
