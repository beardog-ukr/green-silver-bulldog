#pragma once

#include "cocos2d.h"
using namespace cocos2d;

#include "MoveDirection.h"
class DogKeeper;
class FarmerKeeper;
class TiledMapKeeper;
class TiledMapLoader;

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


  bool initFarmerKeeper(TiledMapLoader *const mapLoader);
  bool initDogKeeper(TiledMapLoader *const mapLoader);
  bool initTiledMapKeeper(TiledMapLoader *const mapLoader);

  int currentDogX;
  int currentDogY;
  bool dogIsActive;
  void makeDogMove();

  std::vector<std::pair<int, int> >homePositions;

  enum DogBehavior {
    DB_ROTATES = 0, // "Debug" variant, make it do rounds
    DB_RANDOM,      // "True" variant, pure random
    DB_FOLLOWS,
    DB_GETS_HOME,
  };
  DogBehavior dogBehavior;

  int dogTargetX;
  int dogTargetY;

  // int lastDogMove;
  MoveDirection generateNextDogMove();
  MoveDirection generateNextDogMoveOnFollows();
  MoveDirection generateNextDogMoveOnGoHome();
  MoveDirection generateNextDogMoveOnRandom();
  MoveDirection generateNextDogMoveOnRotates();

  //
  void          processFarmerCall();
  void          processGoHomeRequest();

  void          onKeyPressedScene(EventKeyboard::KeyCode keyCode,
                                  Event                 *event);

  void          processFarmerMovementFinish();
};
