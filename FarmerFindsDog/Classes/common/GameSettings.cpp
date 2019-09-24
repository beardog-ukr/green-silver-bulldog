#include "common/GameSettings.h"

#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

const string GameSettings::settingsFileName = "game_settings.json";

// --- -----------------------------------------------------------------------

GameSettings * GameSettings::getInstance() {
  static GameSettings *instance = new GameSettings();

  return instance;
}

// --- -----------------------------------------------------------------------

GameSettings::GameSettings() {
  log("%s: here", __func__);

  requiredActions[0] = RA_GO_UP;
  requiredActions[1] = RA_GO_DOWN;
  requiredActions[2] = RA_GO_LEFT;
  requiredActions[3] = RA_GO_RIGHT;
  requiredActions[4] = RA_STOP_MOVING;
  requiredActions[5] = RA_FOLLOW;
  requiredActions[6] = RA_GO_HOME;

  keyCodes[0] = EventKeyboard::KeyCode::KEY_UP_ARROW;
  keyCodes[1] = EventKeyboard::KeyCode::KEY_DOWN_ARROW;
  keyCodes[2] = EventKeyboard::KeyCode::KEY_LEFT_ARROW;
  keyCodes[3] = EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
  keyCodes[4] = EventKeyboard::KeyCode::KEY_SPACE;
  keyCodes[5] = EventKeyboard::KeyCode::KEY_F;
  keyCodes[6] = EventKeyboard::KeyCode::KEY_H;
}

// --- -----------------------------------------------------------------------

RequiredAction GameSettings::getActionForKeyCode(const cocos2d::EventKeyboard::KeyCode keyCode)
const {
  RequiredAction result = RA_UNDEFINED;

  for (int i = 0; i < RA_TOTAL_AMOUNT; i++) {
    if (keyCode == keyCodes[i]) {
      result = requiredActions[i];
      break;
    }
  }

  return result;
}

// --- -----------------------------------------------------------------------
