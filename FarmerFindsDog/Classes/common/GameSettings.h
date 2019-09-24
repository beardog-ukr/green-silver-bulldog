#pragma once

#include <string>

#include "cocos2d.h"

enum RequiredAction {
  RA_GO_UP = 0,
  RA_GO_DOWN,
  RA_GO_LEFT,
  RA_GO_RIGHT,
  RA_STOP_MOVING,
  RA_FOLLOW,
  RA_GO_HOME,

  RA_TOTAL_AMOUNT = 7, // don't forget to change this valye if new RA_* item is
                       // added

  RA_UNDEFINED = 200
};

class GameSettings {
public:

  static GameSettings* getInstance();


  RequiredAction       getActionForKeyCode(const cocos2d::EventKeyboard::KeyCode keyCode) const;

  // bool needsFogOfWar();

private:

  GameSettings();
  GameSettings(const GameSettings&) {}

  GameSettings& operator=(const GameSettings&) {}

  static const std::string settingsFileName;


  cocos2d::EventKeyboard::KeyCode keyCodes[RA_TOTAL_AMOUNT];
  RequiredAction requiredActions[RA_TOTAL_AMOUNT];

  int gameScore = 1; // TODO: remove this line

  // void performDefaultKeyboardSettings();
  // void performDefaultOtherSettings();
  // bool loadSettingsFromFile(const std::string& filename);
};
