#pragma once
#include "ray.h"
#include "shaders.h"
#include "settings.h"
#include "utils.h"
#include "utils_client.h"
#include "gui.h"
#include "input.h"
#include <raylib.h>

struct RoverAssets { // Reload
  Model* rover = nullptr;
  ModelAnimation* driveAnimation = nullptr;
  int animsCount = 0;
  int animFrameCounter = 0;
  Vector3 offset = {0.0f, 0.5f, 0.0f};
  Vector3 rotationAxis = {180.0f, 0.0f, 0.0f};
  Model* scan = nullptr;
  Vector3 scanOffset = {0.0f, 0.5f, 0.0f};
  Vector3 scanRotationAxis = {0.0f, 1.0f, 0.0f};
  Color tint = WHITE;
};

struct ResourceManager { // Reload
  MapCT<const char*, Material, 100>* materialPool;
  RoverAssets roverAssets;

  void reload() {
    UnloadModel(*roverAssets.rover);
    UnloadModelAnimation(*roverAssets.driveAnimation);
    UnloadModel(*roverAssets.scan);
    //Also unload all shared materials
  }
};

struct Cameras { // Permanent
  Camera3D camera;
  Camera3D lightCamera;
};

struct RenderResources { // TODO: Maybe rethink this name & what should be in resource manager vs render resources
  Cameras* cameras;                  // Permanent
  Shaders* shaders;                  // Reload
  ResourceManager* resourceManager;  // Reload
  rresCentralDir* dir;               // Reload
  GUI* gui;                          // Permanent

  void reload() {
    shaders->reload();
    resourceManager->reload();
    shaders = nullptr;
    resourceManager = nullptr;
    rresUnloadCentralDirectory(*dir);
    dir = nullptr;
  }
};

enum class GameMode {
  MENU,
  REALTIME,
  AUTOBATTLE
};

struct GameState {
  uint32_t frameCount = 0;
  float deltaTime = 0;
  GameMode gameMode = GameMode::MENU;
  Settings settings;
  bool exitWindow = false;
  InputHandler inputHandler;

  // Direct pointers to static arena-managed resources
  RenderResources renderResources;

  // Arenas
  Arena frameArena;        // Clears every frame
  Arena matchArena;        // Clears every match
  Arena reloadArena;       // Clears on hot-reload
  Arena permanentArena;    // Doesn't clear on hot-reload

  GameState()
    : frameArena(KB(5))
    , matchArena(MB(5))
    , reloadArena(MB(50))
    , permanentArena(MB(100))
  {
    frameArena.create_map_ct<const char*, void*, ArenaIndexSize>();
    matchArena.create_map_ct<const char*, void*, ArenaIndexSize>();
    reloadArena.create_map_ct<const char*, void*, ArenaIndexSize>();
    permanentArena.create_map_ct<const char*, void*, ArenaIndexSize>();
  }
};

struct Rover { // Stored in generational sparse set in match arena
  Transform transform;
  ArrayCT<float, 4> wheelRotations; // [backLeft, backRight, frontLeft, frontRight]
  bool isScanning = false;
};
