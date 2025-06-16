#pragma once
#include "ray.h"
#include "ui_scale.h"

struct UIElementGroup {
  Vector2 anchor = {0, 0};
  bool active = false;
  bool dirty = true;
  Rectangle* layoutRecs = nullptr;
  Rectangle* baseLayout = nullptr;
  int numRecs = 0;

  virtual void Update(float uiScale) {
    if (!active) return;
    
    if (IsWindowResized()) dirty = true;
    if (!dirty) return;

    UpdateLayout(uiScale);
    dirty = false;
  }

  virtual void UpdateLayout(float uiScale) {
    UIScale scale = CalculateUIScale(uiScale);
    GuiSetStyle(DEFAULT, TEXT_SIZE, (int)ScaleSize(12.0f, scale.uniformScale));

    for (int i = 0; i < numRecs; i++) {
      layoutRecs[i] = {
        anchor.x + ScaleSize(baseLayout[i].x, scale.uniformScale),
        anchor.y + ScaleSize(baseLayout[i].y, scale.uniformScale),
        ScaleSize(baseLayout[i].width, scale.uniformScale),
        ScaleSize(baseLayout[i].height, scale.uniformScale)
      };
    }
  }

  virtual ~UIElementGroup() = default;
};

struct DraggableWindow : public UIElementGroup {
  bool isDragging = false;
  Vector2 dragOffset = {0, 0};
  float dragHeight = 25.0f;
  int zIndex = 0;

  void ClampPosition() {
    if (numRecs == 0) return;
    
    float width = GetScreenWidth();
    float height = GetScreenHeight();
    float maxX = width - layoutRecs[0].width/2;
    float minX = layoutRecs[0].width/2;
    float maxY = height - layoutRecs[0].height/2;
    float minY = layoutRecs[0].height/2;
    
    anchor.x = Clamp(anchor.x, minX, maxX);
    anchor.y = Clamp(anchor.y, minY, maxY);
  }

  void HandleDragging() {
    if (!active || numRecs == 0) return;

    Vector2 mousePos = GetMousePosition();
    Rectangle dragArea = {
      layoutRecs[0].x,
      layoutRecs[0].y,
      layoutRecs[0].width,
      dragHeight
    };

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && 
      CheckCollisionPointRec(mousePos, dragArea)) {
      isDragging = true;
      dragOffset = {
        anchor.x - mousePos.x,
        anchor.y - mousePos.y
      };
    }

    if (isDragging && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      anchor.x = mousePos.x + dragOffset.x;
      anchor.y = mousePos.y + dragOffset.y;
      dirty = true;
    }

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
      isDragging = false;
    }
  }

  void Update(float uiScale) override {
    if (!active) return;

    HandleDragging();

    if (IsWindowResized()) dirty = true;
    if (!dirty) return;

    ClampPosition();
    UpdateLayout(uiScale);
    dirty = false;
  }
};

struct StaticElementGroup : public UIElementGroup {
  // Inherits base implementation from UIElementGroup
};
