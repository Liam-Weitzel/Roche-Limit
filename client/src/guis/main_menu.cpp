#include "main_menu.h"
#include "game_state.h"
#include "settings.h"
#include "ui_scale.h"

// Button handlers
void GameModesGroup::RealtimeButton() { /* TODO: Implement control logic */ }
void GameModesGroup::ArenaButton() { /* TODO: Implement control logic */ }
void GameModesGroup::SandboxButton() { /* TODO: Implement control logic */ }
void GameModesGroup::CustomGameButton() { /* TODO: Implement control logic */ }
void ChatGroup::SettingsButton() { /* TODO: Implement control logic */ }
void PartyGroup::PartyUsernameButton4() { /* TODO: Implement control logic */ }
void PartyGroup::PartyUsernameButton3() { /* TODO: Implement control logic */ }
void PartyGroup::PartyUsernameButton2() { /* TODO: Implement control logic */ }
void PartyGroup::PartyUsernameButton1() { /* TODO: Implement control logic */ }
void PartyGroup::ProfileUsernameButton() { /* TODO: Implement control logic */ }
void PartyGroup::ProfileIconButton() { /* TODO: Implement control logic */ }
void PartyGroup::PartyIconButton1() { /* TODO: Implement control logic */ }
void PartyGroup::PartyIconButton2() { /* TODO: Implement control logic */ }
void PartyGroup::PartyIconButton3() { /* TODO: Implement control logic */ }
void PartyGroup::PartyIconButton4() { /* TODO: Implement control logic */ }
void PartyGroup::PartyKickButton1() { /* TODO: Implement control logic */ }
void PartyGroup::PartyKickButton2() { /* TODO: Implement control logic */ }
void PartyGroup::PartyKickButton3() { /* TODO: Implement control logic */ }
void PartyGroup::PartyKickButton4() { /* TODO: Implement control logic */ }
void PartyGroup::PartyLeaveButton() { /* TODO: Implement control logic */ }
void PartyGroup::PartyInviteButton() { /* TODO: Implement control logic */ }
void ChatGroup::ExitButton() { /* TODO: Implement control logic */ }

void MainMenu::Update() {

  if(IsWindowResized()) {
    gameModes.dirty = true;
    chat.dirty = true;
    music.dirty = true;
    party.dirty = true;
  }

  // Only update groups that need updating
  if(gameModes.dirty || chat.dirty || music.dirty || party.dirty) {
    float width = GetScreenWidth();
    float height = GetScreenHeight();
    UIScale scale = CalculateUIScale(s->uiScale);
    GuiSetStyle(DEFAULT, TEXT_SIZE, (int)ScaleSize(12.0f, scale.uniformScale));

    gameModes.anchor = {24 * scale.scaleX, 24 * scale.scaleY};
    chat.anchor = {24 * scale.scaleX, height - 24 * scale.scaleY};
    music.anchor = {width - 24 * scale.scaleX, height - 24 * scale.scaleY};
    party.anchor = {width - 24 * scale.scaleX, 24 * scale.scaleY};

    if(gameModes.dirty) gameModes.Update(s->uiScale);
    if(chat.dirty) chat.Update(s->uiScale);
    if(music.dirty) music.Update(s->uiScale);
    if(party.dirty) party.Update(s->uiScale);
  }
}

void GameModesGroup::Draw() {
  if (GuiButton(layoutRecs[0], realtimeButtonText)) RealtimeButton();
  if (GuiButton(layoutRecs[1], arenaButtonText)) ArenaButton();
  if (GuiButton(layoutRecs[2], sandboxButtonText)) SandboxButton();
  if (GuiButton(layoutRecs[3], customGameButtonText)) CustomGameButton();
}

void ChatGroup::Draw() {
  if (GuiButton(layoutRecs[0], settingsButtonText)) SettingsButton();
  GuiScrollPanel(
    {layoutRecs[1].x, layoutRecs[1].y,
     layoutRecs[1].width - chatScrollWindowBoundsOffset.x,
     layoutRecs[1].height - chatScrollWindowBoundsOffset.y},
    chatScrollWindowText,
    layoutRecs[1],
    &chatScrollWindowScrollOffset,
    &chatScrollWindowScrollView
  );
  if (GuiButton(layoutRecs[2], exitButtonText)) ExitButton();
}

void MusicGroup::Draw() {
  GuiGroupBox(layoutRecs[0], musicSelectorGroupBoxText);
  GuiListView(layoutRecs[1], songsListViewText, &songsListViewScrollIndex, &songsListViewActive);
  if (GuiSpinner(layoutRecs[2], albumSpinnerText, &albumSpinnerValue, 0, 100, albumSpinnerEditMode))
    albumSpinnerEditMode = !albumSpinnerEditMode;
}

void PartyGroup::Draw() {
  GuiGroupBox(layoutRecs[0], partyGroupBoxText);
  GuiGroupBox(layoutRecs[1], profileGroupBoxText);

  if (GuiButton(layoutRecs[2], partyUsernameTexts[3])) PartyUsernameButton4();
  if (GuiButton(layoutRecs[3], partyUsernameTexts[2])) PartyUsernameButton3();
  if (GuiButton(layoutRecs[4], partyUsernameTexts[1])) PartyUsernameButton2();
  if (GuiButton(layoutRecs[5], partyUsernameTexts[0])) PartyUsernameButton1();
  if (GuiButton(layoutRecs[6], profileUsernameButtonText)) ProfileUsernameButton();

  if (GuiButton(layoutRecs[7], profileIconButtonText)) ProfileIconButton();
  if (GuiButton(layoutRecs[8], partyIconText)) PartyIconButton1();
  if (GuiButton(layoutRecs[9], partyIconText)) PartyIconButton2();
  if (GuiButton(layoutRecs[10], partyIconText)) PartyIconButton3();
  if (GuiButton(layoutRecs[11], partyIconText)) PartyIconButton4();

  if (GuiButton(layoutRecs[12], partyKickText)) PartyKickButton1();
  if (GuiButton(layoutRecs[13], partyKickText)) PartyKickButton2();
  if (GuiButton(layoutRecs[14], partyKickText)) PartyKickButton3();
  if (GuiButton(layoutRecs[15], partyKickText)) PartyKickButton4();

  if (GuiButton(layoutRecs[16], partyLeaveButtonText)) PartyLeaveButton();
  if (GuiButton(layoutRecs[17], partyInviteButtonText)) PartyInviteButton();
}

void MainMenu::Draw() {
  gameModes.Draw();
  chat.Draw();
  music.Draw();
  party.Draw();
}

void MainMenu::Init(GameState& state) {
  s = &state.settings;
}
