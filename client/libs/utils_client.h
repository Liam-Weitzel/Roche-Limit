#pragma once

#include "utils.h"
#include "ray.h"
#include <cmath>
#include <cstdint>
#include <cstring>
#include <raylib.h>

#define MAX_MATERIAL_MAPS 12
#define RL_MAX_SHADER_LOCATIONS 32

// NOTE: Load model from chunk for use with rres
Model& LoadModelFromChunk(const rresResourceChunk &chunk, Arena &arena);
ModelAnimation& LoadModelAnimationFromChunk(const rresResourceChunk &chunk, Arena& arena);
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

