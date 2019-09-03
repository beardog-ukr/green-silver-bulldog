#include "MainGameScene.h"

#include "FarmerKeeper.h"
#include "TiledMapKeeper.h"

#include "SimpleAudioEngine.h"

USING_NS_CC;

// =================================================================================================

Scene * MainGameScene::createScene()
{
  return MainGameScene::create();
}

// =================================================================================================
// on "init" you need to initialize your instance
bool MainGameScene::init()
{
  // super init first
  if (!Scene::init())
  {
    return false;
  }

  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin      = Director::getInstance()->getVisibleOrigin();

  tiledMapKeeper = new TiledMapKeeper();
  Node *nd = tiledMapKeeper->prepareNode();

  if (nd != nullptr) {
    addChild(nd);
  }

  tiledMapKeeper->bringMapPointToCenter(16, 10);

  if (!initFarmerKeeper()) {
    return false;
  }


  return true;
}

bool MainGameScene::initFarmerKeeper() {
  // ShipKeeper
  farmerKeeper = new FarmerKeeper();
  Node *farmerNode = farmerKeeper->prepareNode();
  addChild(farmerNode);

  currentFarmerX = 15;
  currentFarmerY = 11;
  const Vec2 tmpp = tiledMapKeeper->getPositionForMapItem(currentFarmerX, currentFarmerY);
  farmerKeeper->doStraightMove(tmpp);

  return true;
}
