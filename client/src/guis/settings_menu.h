#pragma once
#include "ui_elements.h"
#include <cstring>
#include <rres.h>
#include "ray.h"

struct GameState;  // Forward declare
struct Settings; // Forward declare
struct InputManager; // Forward declare
struct GUI;

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
  char MSAACheckboxText[32];
  char HDPICheckboxText[32];
  char VSYNCCheckboxText[32];
  char AlwaysRunCheckboxText[32];
  char InterlacedCheckboxText[32];
  char PlaceHolder1CheckboxText[32];
  char PlaceHolder2CheckboxText[32];
  char PlaceHolder3CheckboxText[32];
  char PlaceHolder4CheckboxText[32];
  char PlaceHolder5CheckboxText[32];

  bool uiStyleSpinnerEditMode = false;
  int uiStyleSpinnerValue = 0;
  float musicSliderBarValue = 50.0f;
  float sfxSliderBarValue = 50.0f;
  bool fpsValueBoxEditMode = false;
  int fpsValueBoxValue = 120;
  float uiScaleSliderValue = 50.0f;
  bool displayDropdownBoxEditMode = false;
  int displayDropdownBoxActive = 0;
  bool MSAACheckboxValue = true;
  bool HDPICheckboxValue = true;
  bool VSYNCCheckboxValue = true;
  bool AlwaysRunCheckboxValue = true;
  bool InterlacedCheckboxValue = false;
  bool PlaceHolder1CheckboxValue = false;
  bool PlaceHolder2CheckboxValue = false;
  bool PlaceHolder3CheckboxValue = false;
  bool PlaceHolder4CheckboxValue = false;
  bool PlaceHolder5CheckboxValue = false;

  Rectangle recs[22];
  Rectangle layout[22] = {
    {-96, -210, 192, 420},      // GroupBox: settingsGroupBox
    {-24, -58, 80, 24},         // Spinner: uiStyleSpinner
    {-24, -178, 112, 16},       // SliderBar: musicSliderBar
    {-24, -154, 112, 16},       // SliderBar: sfxSliderBar
    {-24, -90, 72, 24},         // ValueBox: fpsValueBox
    {-96, -138, 192, 16},       // Line: Line
    {-24, -26, 112, 16},        // Slider: uiScaleSlider
    {-24, -122, 96, 24},        // DropdownBox: screenDropdownBox
    {-72, -122, 56, 24},        // Label: screenLabel
    {-96, 166, 192, 16},        // Line: Line2
    {-88, 182, 80, 24},         // Button: cancelButton
    {8, 182, 80, 24},           // Button: applyButton
    {0, 6, 24, 24},             // Checkbox: MSAA
    {0, 38, 24, 24},            // Checkbox: HDPI
    {0, 70, 24, 24},            // Checkbox: VSYNC
    {0, 102, 24, 24},           // Checkbox: AlwaysRun
    {0, 134, 24, 24},           // Checkbox: Interlaced
    {-88, 6, 24, 24},           // Checkbox: Placeholder
    {-88, 38, 24, 24},          // Checkbox: Placeholder
    {-88, 70, 24, 24},          // Checkbox: Placeholder
    {-88, 102, 24, 24},         // Checkbox: Placeholder
    {-88, 134, 24, 24}          // Checkbox: Placeholder
  };

  Settings* s;
  InputManager* im;
  GUI* gui;
  rresCentralDir* dir;

  SettingsMenu() {
    layoutRecs = recs;
    baseLayout = layout;
    numRecs = 22;
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
    strncpy(MSAACheckboxText, "MSAA", sizeof(MSAACheckboxText)-1);
    strncpy(HDPICheckboxText, "HDPI", sizeof(HDPICheckboxText)-1);
    strncpy(VSYNCCheckboxText, "VSYNC", sizeof(VSYNCCheckboxText)-1);
    strncpy(AlwaysRunCheckboxText, "Always run", sizeof(AlwaysRunCheckboxText)-1);
    strncpy(InterlacedCheckboxText, "Interlaced", sizeof(InterlacedCheckboxText)-1);
    strncpy(PlaceHolder1CheckboxText, "TBD", sizeof(PlaceHolder1CheckboxText)-1);
    strncpy(PlaceHolder2CheckboxText, "TBD", sizeof(PlaceHolder2CheckboxText)-1);
    strncpy(PlaceHolder3CheckboxText, "TBD", sizeof(PlaceHolder3CheckboxText)-1);
    strncpy(PlaceHolder4CheckboxText, "TBD", sizeof(PlaceHolder4CheckboxText)-1);
    strncpy(PlaceHolder5CheckboxText, "TBD", sizeof(PlaceHolder5CheckboxText)-1);
  }

  void Init(GameState& state);
  void Draw();
  void CancelButton();
  void ApplyButton();
};
