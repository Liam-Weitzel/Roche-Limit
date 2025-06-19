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
  bool shouldProcessInput = false;

  virtual void Update(float uiScale);
  virtual void UpdateLayout(float uiScale);
  virtual ~UIElementGroup() = default;
};

struct DraggableWindow : public UIElementGroup {
  bool isDragging = false;
  Vector2 dragOffset = {0, 0};
  float dragHeight = 25.0f;
  int zIndex = 0;

  void ClampPosition();
  void HandleDragging();
  void Update(float uiScale) override;
  virtual void Draw() = 0;
};

struct StaticElementGroup : public UIElementGroup {
  // Inherits base implementation from UIElementGroup
};
