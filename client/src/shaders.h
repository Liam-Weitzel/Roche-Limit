#pragma once
#include "ray.h"
#include "utils_client.h"

struct Shaders { // Reload
  Shader shadowShader;
  RenderTexture2D shadowMap;
  Vector3 lightDir;
  int lightDirLoc;
  int lightVPLoc;
  int shadowMapLoc;

  // Shadow map settings
  int shadowMapResolutionLoc;
  int selfShadowIntensityLoc;
  int specularPowerLoc;
  int poissonDiskScaleLoc;
  int shadowBiasFactorLoc;
  int shadowBiasMinLoc;
  int poissonSamplesLoc;
  int shadowDarknessLoc;
  int ambientDivisionLoc;
  int gammaLoc;

  // Values
  int shadowMapResolution = 1024;
  float selfShadowIntensity = 1.0f;
  float specularPower = 16.0f;
  float poissonDiskScale = 1500.0f;
  float shadowBiasFactor = 0.00008f;
  float shadowBiasMin = 0.000008f;
  int poissonSamples = 6;
  float shadowDarkness = 0.16f;
  float ambientDivision = 20.0f;
  float gamma = 2.2f;

  void save(rini_config& config) {
    rini_set_config_comment_line(&config, NULL);
    rini_set_config_comment_line(&config, "Shader values printout for debugging");
    rini_set_config_comment_line(&config, NULL);

    rini_set_config_value_float(&config, "SELF_SHADOW_INTENSITY", selfShadowIntensity, "");
    rini_set_config_value_float(&config, "SPECULAR_POWER", specularPower, "");
    rini_set_config_value_float(&config, "POISSON_DISK_SCALE", poissonDiskScale, "");
    rini_set_config_value_float(&config, "SHADOW_BIAS_FACTOR", shadowBiasFactor, "");
    rini_set_config_value_float(&config, "SHADOW_BIAS_MIN", shadowBiasMin, "");
    rini_set_config_value_float(&config, "POISSON_SAMPLES", poissonSamples, "");
    rini_set_config_value_float(&config, "SHADOW_DARKNESS", shadowDarkness, "");
    rini_set_config_value_float(&config, "AMBIENT_DIVISION", ambientDivision, "");
    rini_set_config_value_float(&config, "GAMMA", gamma, "");
  }

  void upload() {
    SetShaderValue(shadowShader, selfShadowIntensityLoc, &selfShadowIntensity, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shadowShader, specularPowerLoc, &specularPower, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shadowShader, poissonDiskScaleLoc, &poissonDiskScale, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shadowShader, shadowBiasFactorLoc, &shadowBiasFactor, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shadowShader, shadowBiasMinLoc, &shadowBiasMin, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shadowShader, poissonSamplesLoc, &poissonSamples, SHADER_UNIFORM_INT);
    SetShaderValue(shadowShader, shadowDarknessLoc, &shadowDarkness, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shadowShader, ambientDivisionLoc, &ambientDivision, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shadowShader, gammaLoc, &gamma, SHADER_UNIFORM_FLOAT);
  }
  
  void reload() {
    UnloadShader(shadowShader);
    UnloadRenderTexture(shadowMap);
    if (shadowMap.id > 0) rlUnloadFramebuffer(shadowMap.id);
  }
};
