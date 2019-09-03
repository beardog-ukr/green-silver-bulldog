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

  bool initFarmerKeeper();
};
