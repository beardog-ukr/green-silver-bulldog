#pragma once

#include "cocos2d.h"
using namespace cocos2d;

#include "MoveDirection.h"
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

  MoveDirection candidateMoveDirection;
  int candidateFarmerX;
  int candidateFarmerY;

  bool farmerIsMoving;

  void initKeyboardProcessing();

  void moveFarmer(const MoveDirection moveDirection);

  void moveFarmerForced(const MoveDirection moveDirection);


  bool initFarmerKeeper();
  bool initTiledMapKeeper();

  void onKeyPressedScene(EventKeyboard::KeyCode keyCode,
                         Event                 *event);

  void processFarmerMovementFinish();
};
