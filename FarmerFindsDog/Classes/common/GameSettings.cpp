#include "common/GameSettings.h"

#include "json/document-wrapper.h"

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


  applyDefaultKeyboardSettings(); // TODO remove this line when file import will
                                  // be completed
  applyDefaultOtherSettings();

  if (!loadSettingsFromFile()) {
    applyDefaultKeyboardSettings();
    applyDefaultOtherSettings();
  }
}

// --- -----------------------------------------------------------------------

void GameSettings::applyDefaultKeyboardSettings() {
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

void GameSettings::applyDefaultOtherSettings() {
  needsFogOfWar = true;
}

// --- -----------------------------------------------------------------------

RequiredAction GameSettings::getActionForKeyCode(const EventKeyboard::KeyCode keyCode) const {
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

EventKeyboard::KeyCode GameSettings::getKeyCodeForAction(const RequiredAction ra) const {
  EventKeyboard::KeyCode result = EventKeyboard::KeyCode::KEY_NONE;

  for (int i = 0; i < RA_TOTAL_AMOUNT; i++) {
    if (ra == requiredActions[i]) {
      result = keyCodes[i];
      break;
    }
  }

  if (result == EventKeyboard::KeyCode::KEY_NONE) {
    log("%s: failed to find code for action %d", __func__, (int)ra);
  }

  return result;
}

// --- -----------------------------------------------------------------------

bool GameSettings::loadSettingsFromFile() {
  auto fileUtils = FileUtils::getInstance();

  log("%s: will search for files in:", __func__);

  for (string s: fileUtils->getSearchPaths()) {
    log("%s:  ---> %s", __func__, s.c_str());
  }
  log("%s: done listing pathes", __func__);

  string fnfp = fileUtils->fullPathForFilename(settingsFileName);

  if (!fileUtils->isFileExist(fnfp)) {
    log("%s: file does not exist (%s)", __func__, fnfp.c_str());
    return false;
  }

  string fnContent = fileUtils->getStringFromFile(fnfp);
  log("%s: received content:\n %s \n", __func__, fnContent.c_str());

  rapidjson::Document reader;
  reader.Parse(fnContent.c_str());

  // --- "Fog of War" setting
  const char *const keyFogOfWar = "fog_of_war";

  if ((reader.HasMember(keyFogOfWar) && reader[keyFogOfWar].IsBool())) {
    needsFogOfWar = reader[keyFogOfWar].GetBool();
  }
  else {
    log("%s: failed to find '%s' setting", __func__, keyFogOfWar);
    return false;
  }

  // --- keyboard settings

  const char *const keyKeyBindings = "key_bindings";

  if ((reader.HasMember(keyKeyBindings) && reader[keyKeyBindings].IsArray())) {
    const rapidjson::Value& kbArrValue = reader[keyKeyBindings];
    log("%s: setting '%s' is array of %d values", __func__,
        keyKeyBindings, kbArrValue.Size());

    // for (SizeType i = 0; i < a.Size(); i++) // Uses SizeType instead of
    // size_t
    // printf("a[%d] = %d\n", i, a[i].GetInt());
  }
  else {
    log("%s: failed to find '%s' setting", __func__, keyFogOfWar);
    return false;
  }


  // --- finally
  return true;
}

// --- -----------------------------------------------------------------------

bool GameSettings::getNeedsFogOfWar() {
  return needsFogOfWar;
}

// --- -----------------------------------------------------------------------

bool GameSettings::saveSettings() {
  return true;
}

// --- -----------------------------------------------------------------------

void GameSettings::setKeyCodeForAction(const EventKeyboard::KeyCode keyCode,
                                       const RequiredAction         action) {
  for (int i = 0; i < RA_TOTAL_AMOUNT; i++) {
    if (action == requiredActions[i]) {
      keyCodes[i] = keyCode;
      break;
    }
  }
}

// --- -----------------------------------------------------------------------
// --- -----------------------------------------------------------------------
// --- -----------------------------------------------------------------------

string keyCodeToString(const cocos2d::EventKeyboard::KeyCode keyCode) {
  std::map<cocos2d::EventKeyboard::KeyCode, std::string> c2s;

  c2s.insert({ EventKeyboard::KeyCode::KEY_UP_ARROW, "UP" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_DOWN_ARROW, "DOWN" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_LEFT_ARROW, "LEFT" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_RIGHT_ARROW, "RIGHT" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_A, "A" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_B, "B" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_C, "C" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_D, "D" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_E, "E" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_F, "F" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_G, "G" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_H, "H" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_I, "I" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_J, "J" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_K, "K" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_L, "L" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_M, "M" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_N, "N" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_O, "O" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_P, "P" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_Q, "Q" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_R, "R" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_S, "S" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_T, "T" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_U, "U" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_V, "V" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_W, "W" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_X, "X" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_Y, "Y" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_Z, "Z" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_0, "0" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_1, "1" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_2, "2" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_3, "3" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_4, "4" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_5, "5" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_6, "6" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_7, "7" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_8, "8" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_9, "9" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_PLUS, "+" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_MINUS, "-" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_COMMA, "," });
  c2s.insert({ EventKeyboard::KeyCode::KEY_PERIOD, "." });
  c2s.insert({ EventKeyboard::KeyCode::KEY_SLASH, "/" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_SPACE, "space" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_INSERT, "ins" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_DELETE, "del" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_HOME, "home" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_END, "end" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_TAB, "tab" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_SHIFT, "shft" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_LEFT_BRACE, "[" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_RIGHT_BRACE, "]" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_EQUAL, "=" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_SEMICOLON, ";" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_PG_UP, "pg up" });
  c2s.insert({ EventKeyboard::KeyCode::KEY_PG_DOWN, "pg d" });

  auto searchResult = c2s.find(keyCode);

  if (searchResult != c2s.end()) {
    return searchResult->second;
  }

  // else
  return "N/A";
}
