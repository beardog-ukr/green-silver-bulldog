#include "MainGameScene.h"
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

  // add "HelloWorld" splash screen"
  auto sprite = Sprite::create("HelloWorld.png");

  if (sprite == nullptr)
  {
    log("failed to load 'HelloWorld.png'");
  }
  else
  {
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
  }
  return true;
}
