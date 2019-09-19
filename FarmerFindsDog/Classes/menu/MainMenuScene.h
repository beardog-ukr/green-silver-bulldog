#pragma once

#include "cocos2d.h"
using namespace cocos2d;


class MainMenuScene : public Scene {
public:

  static Scene* createScene();

  virtual bool  init();

  CREATE_FUNC(MainMenuScene);

protected:

  bool initBackground();

  void initKeyboardProcessing();

  void onKeyPressedScene(EventKeyboard::KeyCode keyCode,
                         Event                 *event);
};
