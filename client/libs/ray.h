#pragma once

#include "raylib.h"
#include "raygui.h"
#include "rres.h"
#include "rres-raylib.h"
#include "rini.h"
#include "raymath.h"
#include "rlgl.h"

void LoadGuiIcons(const unsigned char* data, int dataSize, const char* fileName);
void LoadGuiStyle(const unsigned char* data, int dataSize);
