#pragma once
#include "ui_elements.h"
#include <cstring>

struct GameState;  // Forward declare
struct Settings; //Forward declare

struct PartyGroup : public StaticElementGroup {
  char partyGroupBoxText[32];
  char profileGroupBoxText[32];
  char partyUsernameTexts[4][32];
  char profileUsernameButtonText[32];
  char profileIconButtonText[32];
  char partyIconText[32];
  char partyKickText[32];
  char partyLeaveButtonText[32];
  char partyInviteButtonText[32];

  Rectangle layout[18] = {
    {-192, 80, 192, 208},   // GroupBox: partyGroupBox
    {-192, 8, 192, 64},     // GroupBox: profileGroupBox
    
    {-184, 240, 112, 32},   // Button: partyUsernameButton4
    {-184, 192, 112, 32},   // Button: partyUsernameButton3
    {-184, 144, 112, 32},   // Button: partyUsernameButton2
    {-184, 96, 112, 32},    // Button: partyUsernameButton1
    {-184, 24, 112, 32},    // Button: profileUsernameButton
    
    {-64, 16, 56, 48},      // Button: profileIconButton
    {-64, 88, 56, 48},      // Button: partyIconButton1
    {-64, 136, 56, 48},     // Button: partyIconButton2
    {-64, 184, 56, 48},     // Button: partyIconButton3
    {-64, 232, 56, 48},     // Button: partyIconButton4
    
    {-232, 104, 32, 16},    // Button: partyKickButton1
    {-232, 152, 32, 16},    // Button: partyKickButton2
    {-232, 200, 32, 16},    // Button: partyKickButton3
    {-232, 248, 32, 16},    // Button: partyKickButton4
    
    {-184, 296, 80, 24},    // Button: partyLeaveButton
    {-96, 296, 80, 24}      // Button: partyInviteButton
  };
  Rectangle recs[18];
  
  PartyGroup() {
    layoutRecs = recs;
    baseLayout = layout;
    numRecs = 18;
    active = true;

    // Group boxes
    strncpy(partyGroupBoxText, "Party", sizeof(partyGroupBoxText)-1);
    strncpy(profileGroupBoxText, "Profile", sizeof(profileGroupBoxText)-1);

    // Profile
    strncpy(profileUsernameButtonText, "USERNAME", sizeof(profileUsernameButtonText)-1);
    strncpy(profileIconButtonText, "", sizeof(profileIconButtonText)-1);

    // Party members
    for(int i = 0; i < 4; i++) {
        strncpy(partyUsernameTexts[i], "USERNAME", sizeof(partyUsernameTexts[i])-1);
    }
    strncpy(partyIconText, "", sizeof(partyIconText)-1);
    strncpy(partyKickText, "Kick", sizeof(partyKickText)-1);

    // Party controls
    strncpy(partyLeaveButtonText, "Leave", sizeof(partyLeaveButtonText)-1);
    strncpy(partyInviteButtonText, "Invite", sizeof(partyInviteButtonText)-1);
  }

  void Draw();
  void PartyUsernameButton4();
  void PartyUsernameButton3();
  void PartyUsernameButton2();
  void PartyUsernameButton1();
  void ProfileUsernameButton();
  void ProfileIconButton();
  void PartyIconButton1();
  void PartyIconButton2();
  void PartyIconButton3();
  void PartyIconButton4();
  void PartyKickButton1();
  void PartyKickButton2();
  void PartyKickButton3();
  void PartyKickButton4();
  void PartyLeaveButton();
  void PartyInviteButton();
};

struct MusicGroup : public StaticElementGroup {
  char musicSelectorGroupBoxText[32];
  char songsListViewText[32];
  char albumSpinnerText[32];

  Rectangle layout[3] = {
    {-136, -288, 136, 288}, // GroupBox: musicSelectorGroupBox
    {-128, -248, 120, 240}, // ListView: songsListView
    {-128, -280, 120, 24}   // Spinner: albumSpinner
  };
  Rectangle recs[3];

  int songsListViewScrollIndex = 0;
  int songsListViewActive = 0;
  bool albumSpinnerEditMode = false;
  int albumSpinnerValue = 0;
  
  MusicGroup() {
    layoutRecs = recs;
    baseLayout = layout;
    numRecs = 3;
    active = true;
    
    strncpy(musicSelectorGroupBoxText, "Music", sizeof(musicSelectorGroupBoxText)-1);
    strncpy(songsListViewText, "ONE;TWO;THREE", sizeof(songsListViewText)-1);
    strncpy(albumSpinnerText, "", sizeof(albumSpinnerText)-1);
  }

  void Draw();
};

struct ChatGroup : public StaticElementGroup {
  char settingsButtonText[32];
  char exitButtonText[32];
  char chatScrollWindowText[32];

  Rectangle layout[3] = {
    {0, -344, 144, 48},     // Button: settingsButton
    {0, -288, 576, 288},    // ScrollPanel: chatScrollWindow
    {152, -344, 144, 48}    // Button: exitButton
  };
  Rectangle recs[3];

  Rectangle chatScrollWindowScrollView = {0, 0, 0, 0};
  Vector2 chatScrollWindowScrollOffset = {0, 0};
  Vector2 chatScrollWindowBoundsOffset = {0, 0};
  
  ChatGroup() {
    layoutRecs = recs;
    baseLayout = layout;
    numRecs = 3;
    active = true;
    
    strncpy(settingsButtonText, "Settings", sizeof(settingsButtonText)-1);
    strncpy(exitButtonText, "Exit", sizeof(exitButtonText)-1);
    strncpy(chatScrollWindowText, "", sizeof(chatScrollWindowText)-1);
  }

  void Draw();
  void SettingsButton();
  void ExitButton();
};

struct GameModesGroup : public StaticElementGroup {
  char realtimeButtonText[32];
  char arenaButtonText[32];
  char sandboxButtonText[32];
  char customGameButtonText[32];

  Rectangle layout[4] = {
    {0, 0, 144, 48},       // Button: realtimeButton
    {0, 56, 144, 48},      // Button: arenaButton
    {0, 168, 144, 48},     // Button: sandboxButton
    {0, 112, 144, 48}      // Button: customGameButton
  };
  Rectangle recs[4];
  
  GameModesGroup() {
    layoutRecs = recs;
    baseLayout = layout;
    numRecs = 4;
    active = true;

    strncpy(realtimeButtonText, "Realtime (1885)", sizeof(realtimeButtonText)-1);
    strncpy(arenaButtonText, "Arena (1756)", sizeof(arenaButtonText)-1);
    strncpy(sandboxButtonText, "Sandbox", sizeof(sandboxButtonText)-1);
    strncpy(customGameButtonText, "Custom game", sizeof(customGameButtonText)-1);
  }

  void Draw();
  void RealtimeButton();
  void ArenaButton();
  void SandboxButton();
  void CustomGameButton();
};

struct MainMenu {
  GameModesGroup gameModes;
  ChatGroup chat;
  MusicGroup music;
  PartyGroup party;

  Settings* s;

  void Init(GameState& state);
  void Update();
  void Draw();
};
