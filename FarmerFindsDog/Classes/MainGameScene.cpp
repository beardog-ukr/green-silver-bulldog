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

bool MainGameScene::init()
{
  if (!Scene::init()) // super init first
  {
    return false;
  }

  if (!initTiledMapKeeper()) {
    return false;
  }

  if (!initFarmerKeeper()) {
    return false;
  }

  initKeyboardProcessing();

  farmerIsMoving         = false;
  candidateMoveDirection = MOVE_DIRECTION_NO_MOVE;

  return true;
}

// =============================================================================

bool MainGameScene::initFarmerKeeper() {
  // ShipKeeper
  farmerKeeper = new FarmerKeeper();
  Node *farmerNode = farmerKeeper->prepareNode();
  addChild(farmerNode);

  currentFarmerX = 15;
  currentFarmerY = 14;
  const Vec2 tmpp = tiledMapKeeper->getPositionForMapItem(currentFarmerX, currentFarmerY);
  farmerKeeper->doStraightMove(tmpp);

  return true;
}

// =============================================================================

bool MainGameScene::initTiledMapKeeper() {
  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin      = Director::getInstance()->getVisibleOrigin();

  tiledMapKeeper = new TiledMapKeeper();
  Node *nd = tiledMapKeeper->prepareNode();

  if (nd != nullptr) {
    addChild(nd);
  }

  tiledMapKeeper->bringMapPointToCenter(15, 14);

  return true;
}

// =============================================================================

void MainGameScene::initKeyboardProcessing() {
  auto sceneKeyboardListener = EventListenerKeyboard::create();

  sceneKeyboardListener->onKeyPressed = CC_CALLBACK_2(MainGameScene::onKeyPressedScene, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneKeyboardListener, this);
}

// =============================================================================

void MainGameScene::moveFarmer(const MoveDirection moveDirection) {
  if (farmerIsMoving) {
    candidateMoveDirection = moveDirection;
    return;
  }

  // else
  moveFarmerForced(moveDirection);
}

// =============================================================================

void MainGameScene::moveFarmerForced(const MoveDirection moveDirection) {
  // const Size mapSize = tiledMapNode->getMapSize();

  const int diffX[] = { 0, 0, -1, 1 }; // order is up, down, left, right
  const int diffY[] = { 1, -1, 0, 0 };

  const int newTileX = currentFarmerX + diffX[moveDirection];

  // if ((newTileX < 0) || (newTileX >= mapSize.width)) {
  //   log("%s: move to %d cancelled because of X (becomes %d)", __func__,
  // moveDirection, newTileX);
  //   candidateMoveDirection = MOVE_DIRECTION_NO_MOVE;
  //   farmerIsMoving         = false;
  //   return;
  // }

  const int newTileY = currentFarmerY + diffY[moveDirection];

  // if ((newTileY < 0) || (newTileY >= mapSize.height)) {
  //   log("%s: move to %d cancelled because of Y (becomes %d)", __func__,
  // moveDirection, newTileY);
  //   return;
  // }

  if (tiledMapKeeper->isBadMove(newTileX, newTileY)) {
    candidateMoveDirection = MOVE_DIRECTION_NO_MOVE;
    farmerIsMoving         = false;
    return;
  }

  farmerIsMoving         = true;
  candidateMoveDirection = moveDirection;

  candidateFarmerX = newTileX;
  candidateFarmerY = newTileY;

  CallFunc *cf = CallFunc::create([this]() {
    this->processFarmerMovementFinish();
  });

  const Vec2 newPos = tiledMapKeeper->getPositionForMapItem(newTileX, newTileY);
  farmerKeeper->doMove(newPos, moveDirection, cf);
}

// =============================================================================

void MainGameScene::onKeyPressedScene(EventKeyboard::KeyCode keyCode,  Event *event)
{
  log("%s: processing key %d pressed", __func__, (int)keyCode);

  switch (keyCode) {
  case EventKeyboard::KeyCode::KEY_UP_ARROW:
    moveFarmer(MOVE_DIRECTION_UP);
    break;

  case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    moveFarmer(MOVE_DIRECTION_DOWN);
    break;

  case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    moveFarmer(MOVE_DIRECTION_LEFT);
    break;

  case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    moveFarmer(MOVE_DIRECTION_RIGHT);
    break;

  case EventKeyboard::KeyCode::KEY_SPACE:
    candidateMoveDirection = MOVE_DIRECTION_NO_MOVE;
    break;

  case EventKeyboard::KeyCode::KEY_X:
    log("%s: Need to get out.", __func__);

    // Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
    break;

  default:
    log("%s: key %d will be ignored", __func__, (int)keyCode);
  }
}

// =============================================================================

void MainGameScene::processFarmerMovementFinish() {
  log("%s: here", __func__);

  farmerIsMoving = false;
  currentFarmerX = candidateFarmerX;
  currentFarmerY = candidateFarmerY;

  if (tiledMapKeeper->isEdgeTile(currentFarmerX, currentFarmerY)) {
    log("%s: will initiate refocus", __func__);
    tiledMapKeeper->bringMapPointToCenter(currentFarmerX, currentFarmerY);
    const Vec2 nPos = tiledMapKeeper->getPositionForMapItem(currentFarmerX, currentFarmerY);
    farmerKeeper->doStraightMove(nPos);
  }

  if (candidateMoveDirection == MOVE_DIRECTION_NO_MOVE) {
    farmerKeeper->doSetIdle();
  }
  else {
    moveFarmer(candidateMoveDirection);

    // once more
    if (candidateMoveDirection == MOVE_DIRECTION_NO_MOVE) {
      farmerKeeper->doSetIdle();
    }
  }
}
