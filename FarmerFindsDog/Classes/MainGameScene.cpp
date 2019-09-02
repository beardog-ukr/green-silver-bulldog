#include "MainGameScene.h"

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

  return true;
}
