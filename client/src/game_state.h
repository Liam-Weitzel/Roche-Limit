#pragma once
#include "utils.h"
#include "utils_client.h"

struct MainMenu {
  char realtimeButtonText[32];
  char arenaButtonText[32];
  char sandboxButtonText[32];
  char customGameButtonText[32];
  char settingsButtonText[32];
  char exitButtonText[32];
  char songsListViewText[32];
  char partyGroupBoxText[32];
  char profileGroupBoxText[32];
  char partyUsernameButton4Text[32];
  char partyUsernameButton3Text[32];
  char partyUsernameButton2Text[32];
  char partyUsernameButton1Text[32];
  char profileUsernameButtonText[32];
  char profileIconButtonText[32];
  char partyIconButton1Text[32];
  char partyIconButton2Text[32];
  char partyIconButton3Text[32];
  char partyIconButton4Text[32];
  char partyKickButton1Text[32];
  char partyKickButton2Text[32];
  char partyKickButton3Text[32];
  char partyKickButton4Text[32];
  char partyLeaveButtonText[32];
  char partyInviteButtonText[32];
  char musicSelectorGroupBoxText[32];
  char albumSpinnerText[32];
  char chatScrollWindowText[32];

  Rectangle layoutRecs[28];
  Vector2 anchor01;
  Vector2 anchor02;
  Vector2 anchor03;
  Vector2 anchor04;
  Rectangle chatScrollWindowScrollView = { 0, 0, 0, 0 };
  Vector2 chatScrollWindowScrollOffset = { 0, 0 };
  Vector2 chatScrollWindowBoundsOffset = { 0, 0 };
  int songsListViewScrollIndex = 0;
  int songsListViewActive = 0;
  bool albumSpinnerEditMode = false;
  int albumSpinnerValue = 0;

  bool dirty = true;

  MainMenu() {
    strncpy(realtimeButtonText, "Realtime (1885)", sizeof(realtimeButtonText)-1);
    strncpy(arenaButtonText, "Arena (1756)", sizeof(arenaButtonText)-1);
    strncpy(sandboxButtonText, "Sandbox", sizeof(sandboxButtonText)-1);
    strncpy(customGameButtonText, "Custom game", sizeof(customGameButtonText)-1);
    strncpy(settingsButtonText, "Settings", sizeof(settingsButtonText)-1);
    strncpy(exitButtonText, "Exit", sizeof(exitButtonText)-1);
    strncpy(songsListViewText, "ONE;TWO;THREE", sizeof(songsListViewText)-1);
    strncpy(partyGroupBoxText, "Party", sizeof(partyGroupBoxText)-1);
    strncpy(profileGroupBoxText, "Profile", sizeof(profileGroupBoxText)-1);
    strncpy(partyUsernameButton4Text, "USERNAME", sizeof(partyUsernameButton4Text)-1);
    strncpy(partyUsernameButton3Text, "USERNAME", sizeof(partyUsernameButton3Text)-1);
    strncpy(partyUsernameButton2Text, "USERNAME", sizeof(partyUsernameButton2Text)-1);
    strncpy(partyUsernameButton1Text, "USERNAME", sizeof(partyUsernameButton1Text)-1);
    strncpy(profileUsernameButtonText, "USERNAME", sizeof(profileUsernameButtonText)-1);
    strncpy(profileIconButtonText, "", sizeof(profileIconButtonText)-1);
    strncpy(partyIconButton1Text, "", sizeof(partyIconButton1Text)-1);
    strncpy(partyIconButton2Text, "", sizeof(partyIconButton2Text)-1);
    strncpy(partyIconButton3Text, "", sizeof(partyIconButton3Text)-1);
    strncpy(partyIconButton4Text, "", sizeof(partyIconButton4Text)-1);
    strncpy(partyKickButton1Text, "Kick", sizeof(partyKickButton1Text)-1);
    strncpy(partyKickButton2Text, "Kick", sizeof(partyKickButton2Text)-1);
    strncpy(partyKickButton3Text, "Kick", sizeof(partyKickButton3Text)-1);
    strncpy(partyKickButton4Text, "Kick", sizeof(partyKickButton4Text)-1);
    strncpy(partyLeaveButtonText, "Leave", sizeof(partyLeaveButtonText)-1);
    strncpy(partyInviteButtonText, "Invite", sizeof(partyInviteButtonText)-1);
    strncpy(musicSelectorGroupBoxText, "Music", sizeof(musicSelectorGroupBoxText)-1);
    strncpy(albumSpinnerText, "", sizeof(albumSpinnerText)-1);
    strncpy(chatScrollWindowText, "", sizeof(chatScrollWindowText)-1);
  }
};

struct SettingsMenu : DraggableWindow {
    char settingsWindowBoxText[32];
    char musicSliderBarText[32];
    char sfxSliderBarText[32];
    char uiScaleSliderText[32];
    char displayDropdownBoxText[64];
    char displayLabelText[32];
    char LineText[32];
    char fpsValueBoxText[32];
    char uiStyleSpinnerText[32];
    char Line2Text[32];
    char cancelButtonText[32];
    char applyButtonText[32];

    bool uiStyleSpinnerEditMode = false;
    int uiStyleSpinnerValue = 0;
    float musicSliderBarValue = 50.0f;
    float sfxSliderBarValue = 50.0f;
    bool fpsValueBoxEditMode = false;
    int fpsValueBoxValue = 120;
    float uiScaleSliderValue = 50.0f;
    bool displayDropdownBoxEditMode = false;
    int displayDropdownBoxActive = 0;

    Rectangle recs[12];
    Rectangle layout[12] = {
        {-96, -124, 192, 248},  // GroupBox: settingsGroupBox
        {-24, 28, 80, 24},      // Spinner: uiStyleSpinner
        {-24, -92, 112, 16},    // SliderBar: musicSliderBar
        {-24, -68, 112, 16},    // SliderBar: sfxSliderBar
        {-24, -4, 72, 24},      // ValueBox: fpsValueBox
        {-96, -52, 192, 16},    // Line: Line
        {-24, 60, 112, 16},     // Slider: uiScaleSlider
        {-24, -36, 96, 24},     // DropdownBox: screenDropdownBox
        {-72, -36, 56, 24},     // Label: screenLabel
        {-96, 76, 192, 16},     // Line: Line2
        {-88, 92, 80, 24},      // Button: cancelButton
        {8, 92, 80, 24}         // Button: applyButton
    };

    SettingsMenu() {
        layoutRecs = recs;
        baseLayout = layout;
        numRecs = 12;
        active = false;

        strncpy(settingsWindowBoxText, "Settings", sizeof(settingsWindowBoxText)-1);
        strncpy(musicSliderBarText, "Music", sizeof(musicSliderBarText)-1);
        strncpy(sfxSliderBarText, "SFX", sizeof(sfxSliderBarText)-1);
        strncpy(uiScaleSliderText, "UI Scale", sizeof(uiScaleSliderText)-1);
        strncpy(displayDropdownBoxText, "Windowed; Borderless; Fullscreen", sizeof(displayDropdownBoxText)-1);
        strncpy(displayLabelText, "Display", sizeof(displayLabelText)-1);
        strncpy(LineText, "", sizeof(LineText)-1);
        strncpy(fpsValueBoxText, "FPS ", sizeof(fpsValueBoxText)-1);
        strncpy(uiStyleSpinnerText, "UI Style ", sizeof(uiStyleSpinnerText)-1);
        strncpy(Line2Text, "", sizeof(Line2Text)-1);
        strncpy(cancelButtonText, "Cancel", sizeof(cancelButtonText)-1);
        strncpy(applyButtonText, "Apply", sizeof(applyButtonText)-1);
    }
};

struct ShaderSettingsMenu : DraggableWindow {
    char shaderSettingsBoxText[32];
    char selfShadowIntensitySliderText[32];
    char poissonDiskScaleSliderText[32];
    char shadowBiasFactorSliderText[32];
    char shadowBiasMinSliderText[32];
    char poissonSamplesSliderText[32];
    char shadowDarknessSliderText[32];
    char ambientDivisionSliderText[32];
    char specularPowerSliderText[32];
    char gammaSliderText[32];
    char saveButtonText[32];

    float selfShadowIntensitySliderValue = 1.0f;
    float specularPowerSliderValue = 16.0f;
    float poissonDiskScaleSliderValue = 1500.0f;
    float shadowBiasFactorSliderValue = 0.00008f;
    float shadowBiasMinSliderValue = 0.000008f;
    float poissonSamplesSliderValue = 6.0f;
    float shadowDarknessSliderValue = 0.16f;
    float ambientDivisionSliderValue = 20.0f;
    float gammaSliderValue = 2.2f;

    Rectangle recs[11];
    Rectangle layout[11] = {
        {-136, -136, 272, 272},  // WindowBox
        {8, -80, 120, 16},       // SelfShadowIntensity
        {8, -32, 120, 16},       // PoissonDiskScale
        {8, -8, 120, 16},        // ShadowBiasFactor
        {8, 16, 120, 16},        // ShadowBiasMin
        {8, 40, 120, 16},        // PoissonSamples
        {8, 64, 120, 16},        // ShadowDarkness
        {8, 88, 120, 16},        // AmbientDivision
        {8, -56, 120, 16},       // SpecularPower
        {8, 112, 120, 16},       // Gamma
        {8, -104, 120, 16}        // Save
    };

    ShaderSettingsMenu() {
        layoutRecs = recs;
        baseLayout = layout;
        numRecs = 11;
        active = true;

        strncpy(shaderSettingsBoxText, "Shadow Shader Settings", sizeof(shaderSettingsBoxText)-1);
        strncpy(selfShadowIntensitySliderText, "SelfShadowIntensity", sizeof(selfShadowIntensitySliderText)-1);
        strncpy(poissonDiskScaleSliderText, "PoissonDiskScale", sizeof(poissonDiskScaleSliderText)-1);
        strncpy(shadowBiasFactorSliderText, "ShadowBiasFactor", sizeof(shadowBiasFactorSliderText)-1);
        strncpy(shadowBiasMinSliderText, "ShadowBiasMin", sizeof(shadowBiasMinSliderText)-1);
        strncpy(poissonSamplesSliderText, "PoissonSamples", sizeof(poissonSamplesSliderText)-1);
        strncpy(shadowDarknessSliderText, "ShadowDarkness", sizeof(shadowDarknessSliderText)-1);
        strncpy(ambientDivisionSliderText, "AmbientDivision", sizeof(ambientDivisionSliderText)-1);
        strncpy(specularPowerSliderText, "SpecularPower", sizeof(specularPowerSliderText)-1);
        strncpy(gammaSliderText, "Gamma", sizeof(gammaSliderText)-1);
        strncpy(saveButtonText, "Save", sizeof(saveButtonText)-1);
    }
};

struct GUI { // Permanent
  MainMenu mainMenu;
  SettingsMenu settingsMenu;
  ShaderSettingsMenu shaderSettingsMenu;
  char styles[12][32];
  int loaded_style = 0;

  GUI() {
    const char* tmp_styles[12] = {
      "default",
      "ash.rgs",
      "bluish.rgs",
      "candy.rgs",
      "cherry.rgs",
      "cyber.rgs",
      "dark.rgs",
      "enefete.rgs",
      "jungle.rgs",
      "lavanda.rgs",
      "sunny.rgs",
      "terminal.rgs"
    };

    for (int i = 0; i < 12; ++i) {
      strncpy(styles[i], tmp_styles[i], sizeof(styles[i]) - 1);
      styles[i][sizeof(styles[i]) - 1] = '\0';
    }
  }
};

struct RoverAssets { // Reload
  Model* body = nullptr;
  Vector3 bodyOffset = {0.0f, 0.5f, 0.0f};
  Vector3 bodyRotationAxis = {0.0f, 1.0f, 0.0f};
  Model* wheel = nullptr;
  Vector3 wheelOffsets[4] = {
    {0.0f, 0.5f, 0.0f},    // backLeft
    {-2.1f, 0.5f, 0.0f},   // backRight
    {0.0f, 0.5f, -1.25f},  // frontLeft
    {-2.1f, 0.5f, -1.25f}  // frontRight
  };
  Vector3 wheelRotationAxis = {0.0f, 1.0f, 0.0f};
  Model* scan = nullptr;
  Vector3 scanOffset = {0.0f, 0.5f, 0.0f};
  Vector3 scanRotationAxis = {0.0f, 1.0f, 0.0f};
  Color tint = WHITE;
};

struct ResourceManager { // Reload
  rresCentralDir dir;
  RoverAssets roverAssets;

  void reload() {
    // Don't unload models as they are freed when arena is cleared
    rresUnloadCentralDirectory(dir);
  }
};

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

  void reload() {
    UnloadShader(shadowShader);
    UnloadRenderTexture(shadowMap);

    if (shadowMap.id > 0) rlUnloadFramebuffer(shadowMap.id);
  }
};

struct Cameras { // Permanent
  Camera3D camera;
  Camera3D lightCamera;
};

struct RenderResources {
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
    dir = nullptr;
  }
};

enum class GameMode {
  MENU,
  REALTIME,
  AUTOBATTLE
};

struct Settings { // Permanent
  float musicVolume = 50.0f;
  float sfxVolume = 50.0f;
  int displayMode = 0;
  int fpsLimit = 120;
  int uiStyle = 0;
  float uiScale = 50.0f;
};

struct GameState {
  uint32_t frameCount = 0;
  float deltaTime = 0;
  GameMode gameMode = GameMode::MENU;
  Settings settings{};

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
