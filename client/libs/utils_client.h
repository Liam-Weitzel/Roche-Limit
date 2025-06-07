#pragma once

#include "utils.h"
#include "ray.h"
#include <cmath>
#include <cstdint>
#include <cstring>

#define MAX_MATERIAL_MAPS 12
#define RL_MAX_SHADER_LOCATIONS 32

// NOTE: Load model from chunk for use with rres
Model& LoadModelFromChunk(const rresResourceChunk &chunk, Arena &arena);
// NOTE: Clean the shader code of padded bytes for use with rres
char* cleanShaderCode(const rresResourceChunk& chunk);

// NOTE: Comparisons
bool CompareVector3(const Vector3 &a, const Vector3 &b, float epsilon = 0.0001f);
bool CompareVector4(const Vector4 &a, const Vector4 &b, float epsilon = 0.0001f);
bool CompareBones(const BoneInfo *a, const BoneInfo *b, size_t size);
bool CompareMatrices(const Matrix *a, const Matrix *b, size_t count);
bool CompareColor(const Color &a, const Color &b);
bool CompareTexture(const Texture &a, const Texture &b);

// NOTE: Commonly used types
static constexpr uint32_t ArenaIndexSize = 100;
using ArenaIndex = MapCT<const char*, void*, ArenaIndexSize>;

// NOTE: Rini helpers
inline void rini_set_config_value_float(rini_config* config, const char* key, float value, const char* comment) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%f", value);
    rini_set_config_value_text(config, key, buffer, comment);
}

// NOTE: Common UI stuff
struct UIScale {
    float baseWidth = 1920.0f;
    float baseHeight = 1080.0f;
    float scaleX;
    float scaleY;
    float uniformScale;
    // User Scale Multiplier is in settings
};

UIScale CalculateUIScale(float uiScale);
float ScaleSize(float baseSize, float scaleFactor);

template<typename T>
void ClampMenuPosition(T& menu, float screenWidth, float screenHeight) {
    // Calculate maximum allowed positions
    float maxX = screenWidth - menu.layoutRecs[0].width/2;
    float minX = menu.layoutRecs[0].width/2;
    float maxY = screenHeight - menu.layoutRecs[0].height/2;
    float minY = menu.layoutRecs[0].height/2;
    
    // Clamp the anchor position
    menu.anchor01.x = Clamp(menu.anchor01.x, minX, maxX);
    menu.anchor01.y = Clamp(menu.anchor01.y, minY, maxY);
}

struct DraggableWindow {
    Vector2 anchor01 = {0, 0};
    bool isDragging = false;
    Vector2 dragOffset = {0, 0};
    bool dirty = true;
    bool active = false;
    Rectangle* layoutRecs;
    Rectangle* baseLayout;
    int numRecs;
    float dragHeight = 25.0f;
};

template<typename T>
void UpdateDraggableWindow(T& window, float uiScale) {
  if(!window.active) return;

  Vector2 mousePos = GetMousePosition();
  Rectangle windowRect = {
    window.layoutRecs[0].x,
    window.layoutRecs[0].y,
    window.layoutRecs[0].width,
    window.dragHeight
  };

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, windowRect)) {
    window.isDragging = true;
    window.dragOffset = {
      window.anchor01.x - mousePos.x,
      window.anchor01.y - mousePos.y
    };
  }

  if (window.isDragging && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
    window.anchor01.x = mousePos.x + window.dragOffset.x;
    window.anchor01.y = mousePos.y + window.dragOffset.y;
    window.dirty = true;
  }

  if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
    window.isDragging = false;
  }

  if(IsWindowResized()) window.dirty = true;
  if(window.dirty) {
    window.dirty = false;
    float width = GetScreenWidth();
    float height = GetScreenHeight();

    UIScale scale = CalculateUIScale(uiScale);
    GuiSetStyle(DEFAULT, TEXT_SIZE, (int)ScaleSize(12.0f, scale.uniformScale));
    ClampMenuPosition(window, width, height);

    for(int i = 0; i < window.numRecs; i++) {
      window.layoutRecs[i] = {
        window.anchor01.x + ScaleSize(window.baseLayout[i].x, scale.uniformScale),
        window.anchor01.y + ScaleSize(window.baseLayout[i].y, scale.uniformScale),
        ScaleSize(window.baseLayout[i].width, scale.uniformScale),
        ScaleSize(window.baseLayout[i].height, scale.uniformScale)
      };
    }
  }
}
