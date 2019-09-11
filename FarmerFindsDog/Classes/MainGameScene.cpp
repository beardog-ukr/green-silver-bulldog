#include "MainGameScene.h"

#include "DogKeeper.h"
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

  if (!initDogKeeper()) {
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

bool MainGameScene::initDogKeeper() {
  //
  dogKeeper = new DogKeeper();
  Node *dogNode = dogKeeper->prepareNode();
  addChild(dogNode);

  // lastDogMove = 0;

  currentDogX = 12;
  currentDogY = 13;
  const Vec2 tmpp = tiledMapKeeper->getPositionForMapItem(currentDogX, currentDogY);
  dogKeeper->doStraightMove(tmpp);

  DelayTime *pause = DelayTime::create(1);

  CallFunc *cf = CallFunc::create([this]() {
    // this->processFarmerMovementFinish();
    log("%s: init dog move", __func__);
    this->makeDogMove();
  });

  this->runAction(RepeatForever::create(Sequence::create(pause, cf, NULL)));

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

MoveDirection MainGameScene::generateNextDogMove() {
  // "Debug" variant, make it do rounds
  const MoveDirection moveDirections[] = {
    MOVE_DIRECTION_DOWN, MOVE_DIRECTION_LEFT, MOVE_DIRECTION_UP, MOVE_DIRECTION_RIGHT
  };

  static int lastDogMoveCode = 4;

  lastDogMoveCode = lastDogMoveCode + 1;

  if (lastDogMoveCode > 3) {
    lastDogMoveCode = 0;
  }
  return moveDirections[lastDogMoveCode];

  // "True" variant, pure random
  // const MoveDirection moveDirections[] = {
  // MOVE_DIRECTION_UP, MOVE_DIRECTION_DOWN, MOVE_DIRECTION_LEFT,
  // MOVE_DIRECTION_RIGHT
  // };
  // return RandomHelper::random_int(0, 3);
}

void MainGameScene::makeDogMove() {
  // For following arays the order is up, down, left, right, none
  const int diffX[] = { 0, 0, -1, 1, 0 };
  const int diffY[] = { 1, -1, 0, 0, 0 };

  MoveDirection moveDirection = generateNextDogMove();

  const int newTileX = currentDogX + diffX[(int)moveDirection];
  const int newTileY = currentDogY + diffY[(int)moveDirection];

  if (tiledMapKeeper->isBadMove(newTileX, newTileY)) {
    log("%s: proposed move for dog is bad, stay", __func__);
    dogKeeper->doSetIdle();

    return;
  }

  currentDogX = newTileX;
  currentDogY = newTileY;

  CallFunc *cf = CallFunc::create([this]() {
    // this->processFarmerMovementFinish();
    log("boo");
  });

  const Vec2 newPos = tiledMapKeeper->getPositionForMapItem(newTileX, newTileY);
  dogKeeper->doMove(newPos, moveDirection, cf);
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
  const int newTileY = currentFarmerY + diffY[moveDirection];

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
    const Vec2 fPos = tiledMapKeeper->getPositionForMapItem(currentFarmerX, currentFarmerY);
    farmerKeeper->doStraightMove(fPos);
    const Vec2 dPos = tiledMapKeeper->getPositionForMapItem(currentDogX, currentDogY);
    dogKeeper->doStraightMove(dPos);
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
