#include "MainMenuScene.h"
#include "menu/TiledBackgroundNode.h"

// #include "SimpleAudioEngine.h"

USING_NS_CC;

// --- -----------------------------------------------------------------------

Scene * MainMenuScene::createScene() {
  return create();
}

// --- -----------------------------------------------------------------------

bool MainMenuScene::init() {
  if (!Scene::init()) // super init first
  {
    return false;
  }

  // --- constructor things
  // nothing for now

  // --- initialization
  if (!initBackground()) {
    return false;
  }

  Sprite *sprite = Sprite::create("gui/menu_panel.png");

  if (nullptr == sprite) {
    return false;
  }

  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin      = Director::getInstance()->getVisibleOrigin();
  sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x,
                           visibleSize.height / 2 + origin.y));
  addChild(sprite);

  initKeyboardProcessing();

  // --- finally
  return true;
}

// --- -----------------------------------------------------------------------

bool MainMenuScene::initBackground() {
  const Size visibleSize = Director::getInstance()->getVisibleSize();

  Node *const backgroundNode = TiledBackgroundNode::create(visibleSize);

  if (nullptr == backgroundNode) {
    return false;
  }

  addChild(backgroundNode);

  return true;
}

// --- -----------------------------------------------------------------------

void MainMenuScene::initKeyboardProcessing() {
  auto sceneKeyboardListener = EventListenerKeyboard::create();

  sceneKeyboardListener->onKeyPressed = CC_CALLBACK_2(MainMenuScene::onKeyPressedScene, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneKeyboardListener, this);
}

// --- -----------------------------------------------------------------------

void MainMenuScene::onKeyPressedScene(EventKeyboard::KeyCode keyCode,  Event *event)
{
  log("%s: processing key %d pressed", __func__, (int)keyCode);

  switch (keyCode) {
  case EventKeyboard::KeyCode::KEY_X:
    log("%s: Need to get out.", __func__);

    // Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
    break;

  default:
    log("%s: key %d will be ignored", __func__, (int)keyCode);
  }
}
