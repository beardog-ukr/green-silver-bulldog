#pragma once

#include "cocos2d.h"
using namespace cocos2d;

class TiledMapKeeper;
class FarmerKeeper;

class MainGameScene : public Scene {
public:

  static Scene* createScene();

  virtual bool  init();

  // implement the "static create()" method manually
  CREATE_FUNC(MainGameScene);

protected:

  TiledMapKeeper *tiledMapKeeper;
  FarmerKeeper *farmerKeeper;

  int currentFarmerX;
  int currentFarmerY;

  int candidateFarmerX;
  int candidateFarmerY;


  void initKeyboardProcessing();

  //
  enum MoveDirection {
    MOVE_DIRECTION_UP = 0,
    MOVE_DIRECTION_DOWN,
    MOVE_DIRECTION_LEFT,
    MOVE_DIRECTION_RIGHT,
    MOVE_DIRECTION_NO_MOVE = 99
  };

  void moveFarmer(const MoveDirection moveDirection);

  void moveFarmerForced(const MoveDirection moveDirection);


  bool initFarmerKeeper();
  bool initTiledMapKeeper();

  void onKeyPressedScene(EventKeyboard::KeyCode keyCode,
                         Event                 *event);

  void processFarmerMovementFinish();
};
