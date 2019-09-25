#include "common/GameSettings.h"

#include "json/document-wrapper.h"
#include "json/prettywriter.h"
#include "json/stringbuffer.h"

#include <fstream> // ofstream

#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

const char *const keyFogOfWar           = "fog_of_war";
const char *const keyKeyBindings        = "key_bindings";
const char *const keyKeyBindingsAction  = "action";
const char *const keyKeyBindingsKeyCode = "key_code";

const string GameSettings::settingsFileName = "game_settings.json";

// --- -----------------------------------------------------------------------

GameSettings * GameSettings::getInstance() {
  static GameSettings *instance = new GameSettings();

  return instance;
}

// --- -----------------------------------------------------------------------

GameSettings::GameSettings() {
  log("%s: here", __func__);

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

  // log("%s: will search for files in:", __func__);

  // for (string s: fileUtils->getSearchPaths()) {
  //   log("%s:  ---> %s", __func__, s.c_str());
  // }
  // log("%s: done listing pathes", __func__);

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

  if ((reader.HasMember(keyFogOfWar) && reader[keyFogOfWar].IsBool())) {
    needsFogOfWar = reader[keyFogOfWar].GetBool();
  }
  else {
    log("%s: failed to find '%s' setting", __func__, keyFogOfWar);
    return false;
  }

  // --- keyboard settings
  if ((reader.HasMember(keyKeyBindings) && reader[keyKeyBindings].IsArray())) {
    const rapidjson::Value& kbArrValue = reader[keyKeyBindings];
    log("%s: setting '%s' is array of %d values", __func__,
        keyKeyBindings, kbArrValue.Size());

    for (rapidjson::SizeType i = 0; i < kbArrValue.Size(); i++) {
      const rapidjson::Value& kbArrItem = kbArrValue[i];

      if ((kbArrItem.HasMember(keyKeyBindingsAction) &&
           kbArrItem[keyKeyBindingsAction].IsInt())) {
        requiredActions[i] = (RequiredAction)kbArrItem[keyKeyBindingsAction].GetInt();
        log("%s: received '%d' action", __func__, (int)requiredActions[i]);
      }

      if ((kbArrItem.HasMember(keyKeyBindingsKeyCode) &&
           kbArrItem[keyKeyBindingsKeyCode].IsInt())) {
        keyCodes[i] = (EventKeyboard::KeyCode)kbArrItem[keyKeyBindingsKeyCode].GetInt();
        log("%s: received '%d' key code", __func__, (int)keyCodes[i]);
      }
    }

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

bool GameSettings::getNeedsFogOfWar() const {
  return needsFogOfWar;
}

// --- -----------------------------------------------------------------------

bool GameSettings::saveSettings() {
  // --- init
  rapidjson::StringBuffer s;

  rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(s);

  writer.StartObject();

  // --- Fog of war
  writer.Key(keyFogOfWar);
  writer.Bool(needsFogOfWar);

  // --- Key bindings
  writer.Key(keyKeyBindings);
  writer.StartArray();

  for (int i = 0; i < RA_TOTAL_AMOUNT; i++) {
    writer.StartObject();
    writer.Key(keyKeyBindingsAction);
    writer.Int((int)requiredActions[i]);
    writer.Key(keyKeyBindingsKeyCode);
    writer.Int((int)keyCodes[i]);
    writer.EndObject();
  }

  writer.EndArray();
  writer.EndObject();

  auto fileUtils = FileUtils::getInstance();

  // const vector <string> sp = fileUtils->getSearchPaths()
  // for (string s: sp) {
  //   log("%s:  ---> %s", __func__, s.c_str());
  // }
  // log("%s: done listing pathes", __func__);

  string fnfp = fileUtils->fullPathForFilename(settingsFileName);

  if (fnfp.empty()) {
    const vector<string> sp = fileUtils->getSearchPaths();
    fnfp = sp.front() + "/" + settingsFileName;
  }

  ofstream of(fnfp, std::ofstream::out);
  of << s.GetString();

  of.close();

  if (!of.good()) {
    log("%s:Can't write the JSON string to the file '%s'", __func__, fnfp.c_str());
  }
  else
  {
    log("%s: saves to file %s", __func__, fnfp.c_str());
  }

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

bool GameSettings::setNeedsFogOfWar(const bool inValue) {
  needsFogOfWar = inValue;
}

// --- -----------------------------------------------------------------------
// --- -----------------------------------------------------------------------
