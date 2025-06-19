#include "ui_elements.h"

void UIElementGroup::Update(float uiScale) {
  if (!active) return;
  
  if (IsWindowResized()) dirty = true;
  if (!dirty) return;

  UpdateLayout(uiScale);
  dirty = false;
}

void UIElementGroup::UpdateLayout(float uiScale) {
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

void DraggableWindow::ClampPosition() {
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

void DraggableWindow::HandleDragging() {
  if (!active || numRecs == 0) return;

  Vector2 mousePos = GetMousePosition();

  // Only check collision when starting to drag
  if (!isDragging && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && shouldProcessInput) {
    // Calculate close button rectangle
    Rectangle closeButtonRec = { 
      layoutRecs[0].x + layoutRecs[0].width - GuiGetStyle(STATUSBAR, BORDER_WIDTH) - 20,
      layoutRecs[0].y + dragHeight/2.0f - 18.0f/2.0f, 
      18, 
      18 
    };

    // Create drag area
    Rectangle dragArea = {
      layoutRecs[0].x,
      layoutRecs[0].y,
      layoutRecs[0].width,
      dragHeight
    };

    // Check if mouse is in drag area but NOT in close button
    if (CheckCollisionPointRec(mousePos, dragArea) && 
        !CheckCollisionPointRec(mousePos, closeButtonRec)) {
      isDragging = true;
      dragOffset = {
        anchor.x - mousePos.x,
        anchor.y - mousePos.y
      };
    }
  }

  // If already dragging, just update position
  if (isDragging) {
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      anchor.x = mousePos.x + dragOffset.x;
      anchor.y = mousePos.y + dragOffset.y;
      dirty = true;
    } else {
      isDragging = false;
    }
  }
}

void DraggableWindow::Update(float uiScale) {
  if (!active) return;

  HandleDragging();

  if (IsWindowResized()) dirty = true;

  if (!dirty) return;
  ClampPosition();
  UpdateLayout(uiScale);
  dirty = false;
}
