#include "MainGameScene.h"

#include "DogKeeper.h"
#include "FarmerKeeper.h"
#include "TiledMapKeeper.h"
#include "TiledMapLoader.h"

#include "common/GameSettings.h"
#include "menu/InstantMenuScene.h"
#include "menu/MainMenuScene.h"

#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace std;

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

  // --- constructor things
  needsImmediateExit = false;
  TiledMapLoader *mapLoader = nullptr;
  bool result               = true;

  // --- init
  do {
    mapLoader = new TiledMapLoader();

    if (!initTiledMapKeeper(mapLoader)) {
      result = false;
      break;
    }

    if (!initDogKeeper(mapLoader)) {
      result = false;
      break;
    }

    if (!initFarmerKeeper(mapLoader)) {
      result = false;
      break;
    }
  }
  while (false);

  delete mapLoader;

  if (!result) {
    return result;
  }


  initKeyboardProcessing();

  farmerIsMoving         = false;
  candidateMoveDirection = MOVE_DIRECTION_NO_MOVE;

  setOnEnterCallback([this]() {
    if (this->needsImmediateExit) {
      log("%s: needs immediate exit", __func__);
      Scene *mms = MainMenuScene::createScene();
      Director::getInstance()->replaceScene(mms);
    }
  });

  return true;
}

// =============================================================================

bool MainGameScene::initFarmerKeeper(TiledMapLoader *const mapLoader) {
  //
  farmerKeeper = new FarmerKeeper();
  Node *farmerNode = farmerKeeper->prepareNode();
  addChild(farmerNode);

  currentFarmerX = mapLoader->getFarmerStartX();
  currentFarmerY = mapLoader->getFarmerStartY();

  homePositions = mapLoader->getHomePositions();

  const Vec2 tmpp = tiledMapKeeper->getPositionForMapItem(currentFarmerX, currentFarmerY);
  farmerKeeper->doStraightMove(tmpp);

  return true;
}

// =============================================================================

bool MainGameScene::initDogKeeper(TiledMapLoader *const mapLoader) {
  //
  dogKeeper = new DogKeeper();
  Node *dogNode = dogKeeper->prepareNode();
  addChild(dogNode);

  dogBehavior = DB_ROTATES;

  currentDogX = mapLoader->getDogStartX();
  currentDogY = mapLoader->getDogStartY();
  const Vec2 tmpp = tiledMapKeeper->getPositionForMapItem(currentDogX, currentDogY);
  dogKeeper->doStraightMove(tmpp);

  DelayTime *pause = DelayTime::create(1);

  CallFunc *cf = CallFunc::create([this]() {
    // this->processFarmerMovementFinish();
    log("%s: init dog move", __func__);

    if ((DB_GETS_HOME == dogBehavior) &&
        (currentDogX == dogTargetX) && (currentDogY == dogTargetY)) {
      log("%s: Dog is at home", __func__);
    }
    else {
      this->makeDogMove();
    }
  });

  this->runAction(RepeatForever::create(Sequence::create(pause, cf, NULL)));

  return true;
}

// =============================================================================

bool MainGameScene::initTiledMapKeeper(TiledMapLoader *const mapLoader) {
  string mapFilename = "tiles/m03.tmx";
  TMXTiledMap *nd    = mapLoader->loadFile(mapFilename);

  if (nd == nullptr) {
    return false;
  }

  addChild(nd);

  tiledMapKeeper = new TiledMapKeeper();
  tiledMapKeeper->setWorkNode(nd);
  tiledMapKeeper->bringMapPointToCenter(mapLoader->getFarmerStartX(), mapLoader->getFarmerStartY());

  return true;
}

// =============================================================================

void MainGameScene::initKeyboardProcessing() {
  auto sceneKeyboardListener = EventListenerKeyboard::create();

  sceneKeyboardListener->onKeyPressed = CC_CALLBACK_2(MainGameScene::onKeyPressedScene, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneKeyboardListener, this);
}

// =============================================================================
MoveDirection MainGameScene::generateNextDogMoveOnRotates() {
  const MoveDirection moveDirections[] = {
    MOVE_DIRECTION_DOWN, MOVE_DIRECTION_LEFT, MOVE_DIRECTION_UP, MOVE_DIRECTION_RIGHT
  };

  static int lastDogMoveCode = 4;

  lastDogMoveCode = lastDogMoveCode + 1;

  if (lastDogMoveCode > 3) {
    lastDogMoveCode = 0;
  }
  return moveDirections[lastDogMoveCode];
}

// =============================================================================

MoveDirection MainGameScene::generateNextDogMoveOnRandom() {
  const MoveDirection moveDirections[] = {
    MOVE_DIRECTION_UP, MOVE_DIRECTION_DOWN, MOVE_DIRECTION_LEFT,
    MOVE_DIRECTION_RIGHT
  };

  return moveDirections[RandomHelper::random_int(0, 3)];
}

// =============================================================================

MoveDirection MainGameScene::generateNextDogMoveOnFollows() {
  // Tries to get close to farmer on X
  const int diffX[] = { 0, 0, -1, 1, 0 };
  const int diffY[] = { 1, -1, 0, 0, 0 };

  MoveDirection result = MOVE_DIRECTION_NO_MOVE;

  if (abs(currentDogX - currentFarmerX) > 1) {
    if (currentDogX > currentFarmerX) {
      result = MOVE_DIRECTION_LEFT;
    }
    else {
      result = MOVE_DIRECTION_RIGHT;
    }

    const int newTileX = currentDogX + diffX[(int)result];

    if (!tiledMapKeeper->isBadMove(newTileX, currentDogY)) {
      return result;
    }
  }

  // if impossible or already on X, tries to get close on Y
  if (abs(currentDogY - currentFarmerY) > 1) {
    if (currentDogY > currentFarmerY) {
      result = MOVE_DIRECTION_DOWN;
    }
    else {
      result = MOVE_DIRECTION_UP;
    }

    const int newTileY = currentDogY + diffY[(int)result];

    if (!tiledMapKeeper->isBadMove(currentDogX, newTileY)) {
      return result;
    }
  }

  // if impossible, that's all
  return MOVE_DIRECTION_NO_MOVE;
}

// =============================================================================

MoveDirection MainGameScene::generateNextDogMoveOnGoHome() {
  // Tries to get close to farmer on X
  const int diffX[] = { 0, 0, -1, 1, 0 };
  const int diffY[] = { 1, -1, 0, 0, 0 };

  MoveDirection result = MOVE_DIRECTION_NO_MOVE;

  if (abs(currentDogX - dogTargetX) > 0) {
    if (currentDogX > dogTargetX) {
      result = MOVE_DIRECTION_LEFT;
    }
    else {
      result = MOVE_DIRECTION_RIGHT;
    }

    const int newTileX = currentDogX + diffX[(int)result];

    if (!tiledMapKeeper->isBadMove(newTileX, currentDogY)) {
      return result;
    }
  }

  // if impossible or already on X, tries to get close on Y
  if (abs(currentDogY - dogTargetY) > 0) {
    if (currentDogY > dogTargetY) {
      result = MOVE_DIRECTION_DOWN;
    }
    else {
      result = MOVE_DIRECTION_UP;
    }

    const int newTileY = currentDogY + diffY[(int)result];

    if (!tiledMapKeeper->isBadMove(currentDogX, newTileY)) {
      return result;
    }
  }

  // if impossible, move as random
  log("%s: does not know what to do, move random", __func__);
  return generateNextDogMoveOnRandom();
}

// =============================================================================

MoveDirection MainGameScene::generateNextDogMove() {
  MoveDirection result = MOVE_DIRECTION_NO_MOVE;

  switch (dogBehavior) {
  case DB_FOLLOWS:
    result = generateNextDogMoveOnFollows();
    break;

  case DB_RANDOM:
    result = generateNextDogMoveOnRandom();
    break;

  case DB_ROTATES:
    result = generateNextDogMoveOnRotates();
    break;

  case DB_GETS_HOME:
    result = generateNextDogMoveOnGoHome();
    break;

  default:
    result = MOVE_DIRECTION_NO_MOVE;
  }

  return result;
}

// =============================================================================

void MainGameScene::makeDogMove() {
  // For following arays the order is up, down, left, right, none
  const int diffX[] = { 0, 0, -1, 1, 0 };
  const int diffY[] = { 1, -1, 0, 0, 0 };

  const MoveDirection moveDirection = generateNextDogMove();

  if (moveDirection == MOVE_DIRECTION_NO_MOVE) {
    dogKeeper->doSetIdle();
    return;
  }

  const int newTileX = currentDogX + diffX[(int)moveDirection];
  const int newTileY = currentDogY + diffY[(int)moveDirection];

  if (tiledMapKeeper->isBadMove(newTileX, newTileY)) {
    log("%s: proposed move for dog is bad, stay", __func__);
    dogKeeper->doSetIdle();

    return;
  }

  currentDogX = newTileX;
  currentDogY = newTileY;

  const Vec2 newPos = tiledMapKeeper->getPositionForMapItem(newTileX, newTileY);
  dogKeeper->doMove(newPos, moveDirection);
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

// --- -----------------------------------------------------------------------

void MainGameScene::onKeyPressedScene(EventKeyboard::KeyCode keyCode,  Event *event) {
  log("%s: processing key %d pressed", __func__, (int)keyCode);

  GameSettings  *gameSettings   = GameSettings::getInstance();
  RequiredAction requiredAction = gameSettings->getActionForKeyCode(keyCode);


  switch (requiredAction) {
  case RequiredAction::RA_GO_UP:
    moveFarmer(MOVE_DIRECTION_UP);
    break;

  case RA_GO_DOWN:
    moveFarmer(MOVE_DIRECTION_DOWN);
    break;

  case RA_GO_LEFT:
    moveFarmer(MOVE_DIRECTION_LEFT);
    break;

  case RA_GO_RIGHT:
    moveFarmer(MOVE_DIRECTION_RIGHT);
    break;

  case RA_STOP_MOVING:
    candidateMoveDirection = MOVE_DIRECTION_NO_MOVE;
    break;

  case RA_FOLLOW:
    processFarmerCall();
    break;

  case RA_GO_HOME:
    processGoHomeRequest();
    break;

  default:
    doHardcodedKeyProcessing(keyCode);
  }
}

// --- -----------------------------------------------------------------------

void MainGameScene::doHardcodedKeyProcessing(const EventKeyboard::KeyCode keyCode) {
  if (keyCode == EventKeyboard::KeyCode::KEY_J) {
    log("%s: Showing interrupt menu.", __func__);

    const Size visibleSize = Director::getInstance()->getVisibleSize();
    RenderTexture *rt      = RenderTexture::create(visibleSize.width,
                                                   visibleSize.height);

    rt->begin();
    this->visit();
    rt->end();
    rt->getSprite()->setAnchorPoint(Vec2(0, 0));

    Scene *ims = InstantMenuScene::create(this, rt);

    Director::getInstance()->pushScene(ims);
  }
  else if (keyCode == EventKeyboard::KeyCode::KEY_X) {
    log("%s: Need to get out.", __func__);

    // Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
  }
}

// --- -----------------------------------------------------------------------

void MainGameScene::processFarmerCall() {
  const int absDiffX = abs(currentDogX - currentFarmerX);
  const int absDiffY = abs(currentDogY - currentFarmerY);

  const int distanceThreshold = 3;

  if (!((absDiffX <= distanceThreshold) && (absDiffY <= distanceThreshold))) {
    log("%s: called fro too far away", __func__);
    return;
  }

  // else
  log("%s: call was accepted", __func__);
  dogBehavior = DB_FOLLOWS;
}

// =============================================================================

void MainGameScene::processGoHomeRequest() {
  bool isNearHome = false;

  for (auto hpos: homePositions) {
    const int absDiffX = abs(hpos.first - currentDogX);
    const int absDiffY = abs(hpos.second - currentDogY);

    const int distanceThreshold = 3;

    if ((absDiffX <= distanceThreshold) && (absDiffY <= distanceThreshold)) {
      dogTargetX = hpos.first;
      dogTargetY = hpos.second;
      isNearHome = true;
      break;
    }
  }

  if (isNearHome) {
    dogBehavior = DB_GETS_HOME;
  }
  else {
    log("%s: too far from all %d home locations", __func__, (int)homePositions.size());
  }
}

// --- -----------------------------------------------------------------------

void MainGameScene::processFarmerMovementFinish() {
  log("%s: here", __func__);

  farmerIsMoving = false;
  currentFarmerX = candidateFarmerX;
  currentFarmerY = candidateFarmerY;

  if (tiledMapKeeper->isEdgeTile(currentFarmerX, currentFarmerY)) {
    log("%s: will initiate refocus", __func__);
    tiledMapKeeper->bringMapPointToCenter(currentFarmerX, currentFarmerY);
    const Vec2 dPos = tiledMapKeeper->getPositionForMapItem(currentDogX, currentDogY);
    dogKeeper->doStraightMove(dPos);
    dogKeeper->doSetIdle();
    const Vec2 fPos = tiledMapKeeper->getPositionForMapItem(currentFarmerX, currentFarmerY);
    farmerKeeper->doStraightMove(fPos);
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

// --- -----------------------------------------------------------------------

void MainGameScene::setImmediateExit(const bool performExit) {
  needsImmediateExit = performExit;
}

// --- -----------------------------------------------------------------------
