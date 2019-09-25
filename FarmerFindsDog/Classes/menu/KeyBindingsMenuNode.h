#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <string>
#include "common/GameSettings.h"

class KeyBindingsMenuNode : public cocos2d::Node {
public:

  KeyBindingsMenuNode();
  virtual ~KeyBindingsMenuNode();


  static KeyBindingsMenuNode* create();

protected:

  int listeningButtonIndex;


  static const int amountOfButtons = 7;
  std::string captions[amountOfButtons];
  RequiredAction requiredActions[amountOfButtons];
  cocos2d::ui::Button *buttons[amountOfButtons];

  void        prepareKeyBindingsNode();

  void        processButtonCall(const int buttonIdx);

  std::string makeCaptionForButton(const int buttonIndex) const;
  std::string makeEmptyCaptionForButton(const int buttonIndex) const;

  void        onKeyPressedScene(cocos2d::EventKeyboard::KeyCode keyCode,
                                cocos2d::Event                 *event);
};
