#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class MainMenuScene : public cocos2d::Scene {
public:

  static cocos2d::Scene* createScene();

  virtual bool           init();

  CREATE_FUNC(MainMenuScene);

protected:

  bool           initBackground();
  bool           initKeyboardProcessing();

  cocos2d::Node* prepareBasicMenu();
  cocos2d::Node* prepareOptionsMenu();

  cocos2d::Node *currentMenu;

  cocos2d::ui::Button *backButton;
  bool initBackButton();

  cocos2d::Size visibleSize;
  cocos2d::Size commonMenuSize;
  cocos2d::Vec2 origin;
  cocos2d::Vec2 positionMovedOut;
  cocos2d::Vec2 positionBeforeIn;
  cocos2d::Vec2 positionActive;

  enum BackButtonTarget {
    BBT_NONE,
    BBT_BASIC,
    BBT_OPTIONS,
  };
  void processBackButtonCall(const BackButtonTarget backButtonTarget);

  void switchStates(cocos2d::Node *const   newMenu,
                    const BackButtonTarget backButtonTarget);


  ///
  void processNewGameRq(cocos2d::Ref *pSender);
  void processOptionsRq(cocos2d::Ref *pSender);
  void processExitRq(cocos2d::Ref *pSender);

  void onKeyPressedScene(cocos2d::EventKeyboard::KeyCode keyCode,
                         cocos2d::Event                 *event);
};
