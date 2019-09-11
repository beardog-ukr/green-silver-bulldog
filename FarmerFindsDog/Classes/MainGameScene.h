#pragma once

#include "cocos2d.h"
using namespace cocos2d;

#include "MoveDirection.h"
class DogKeeper;
class FarmerKeeper;
class TiledMapKeeper;

class MainGameScene : public Scene {
public:

  static Scene* createScene();

  virtual bool  init();

  // implement the "static create()" method manually
  CREATE_FUNC(MainGameScene);

protected:

  DogKeeper *dogKeeper;
  FarmerKeeper *farmerKeeper;
  TiledMapKeeper *tiledMapKeeper;

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
  bool initDogKeeper();
  bool initTiledMapKeeper();

  int currentDogX;
  int currentDogY;
  bool dogIsActive;
  void          makeDogMove();

  // int lastDogMove;
  MoveDirection generateNextDogMove();

  void          onKeyPressedScene(EventKeyboard::KeyCode keyCode,
                                  Event                 *event);

  void          processFarmerMovementFinish();
};
