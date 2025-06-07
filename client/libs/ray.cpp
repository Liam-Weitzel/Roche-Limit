#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#include "raygui.h"

#define RRES_IMPLEMENTATION
#include "rres.h"

#define RRES_RAYLIB_IMPLEMENTATION
#include "rres-raylib.h"

#define RINI_IMPLEMENTATION
#include "rini.h"

void LoadGuiIcons(const unsigned char* data, int dataSize, const char* fileName) {
  GuiLoadIconsFromMemory(data, dataSize, fileName);
}

void LoadGuiStyle(const unsigned char* data, int dataSize) {
  GuiLoadStyleFromMemory(data, dataSize);
}
