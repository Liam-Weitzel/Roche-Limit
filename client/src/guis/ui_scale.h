#pragma once
#include "ray.h"

struct UIScale {
  float baseWidth = 1920.0f;
  float baseHeight = 1080.0f;
  float scaleX;
  float scaleY;
  float uniformScale;
  // User Scale Multiplier is in settings
};

inline UIScale CalculateUIScale(float uiScalePercent) {
  UIScale scale;
  float width = GetScreenWidth();
  float height = GetScreenHeight();

  scale.scaleX = width / scale.baseWidth;
  scale.scaleY = height / scale.baseHeight;

  // Use the smaller scale to maintain proportions
  scale.uniformScale = (scale.scaleX < scale.scaleY) ? scale.scaleX : scale.scaleY;

  // Apply user's preference to the final scale
  // Normalize from [0, 100] to [0.25, 4.0]
  float normalizedUIScale = 0.25f + (uiScalePercent / 100.0f) * (2.0f - 0.5f);
  scale.uniformScale *= normalizedUIScale;

  return scale;
}

inline float ScaleSize(float baseSize, float scaleFactor) {
  return roundf(baseSize * scaleFactor);
}
