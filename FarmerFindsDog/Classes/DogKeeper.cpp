#include "DogKeeper.h"

const float ITEM_MOVE_TIME      = 1.0f;
const float ITEM_ANIMATION_TIME = 0.1f; //

// Item action tags
const int IAT_ANIMATION = 10;           //
const int IAT_MOVE      = 20;           //

const std::string defPosFilename = "f-dog-2.png";

// =============================================================================
// =============================================================================

DogKeeper::DogKeeper() {
  workNode                 = nullptr;
  movementFinishedCallback = nullptr;
  currentMoveDirection     = MOVE_DIRECTION_NO_MOVE;
}

// =============================================================================

DogKeeper::~DogKeeper() {
  // nothing to do here, but ensure it's called
  log("%s: DogKeeper object destroyed", __func__);
}

// =============================================================================

Animation * DogKeeper::prepareAnimation(const MoveDirection moveDirection) const {
  char templateLeft[]  = "l-dog-%d.png";
  char templateRight[] = "r-dog-%d.png";
  char templateUp[]    = "b-dog-%d.png";
  char templateDown[]  = "f-dog-%d.png";

  char *templateArr[] = { templateUp, templateDown, templateLeft, templateRight, nullptr };

  char *templateStr = templateArr[moveDirection];

  if (templateStr == nullptr) {
    log("%s: unexpected", __func__);
    return nullptr;
  }

  SpriteFrameCache *scache = SpriteFrameCache::getInstance();

  Animation *resultAnimation = Animation::create();

  resultAnimation->setDelayPerUnit(0.1);

  char tmps[256];

  for (int i = 1; i <= 3; i++) {
    sprintf(tmps, templateStr, i);
    log("%s: adding %s", __func__, tmps);

    SpriteFrame *sf = scache->getSpriteFrameByName(tmps);

    if (sf != nullptr) {
      resultAnimation->addSpriteFrame(sf);
    }
    else {
      log("%s: failed  to load sprite frame '%s'", __func__, tmps);
    }
  }

  return resultAnimation;
}

// =============================================================================

Node * DogKeeper::prepareNode() {
  if (workNode != nullptr) {
    return workNode;
  }

  SpriteFrameCache::getInstance()->addSpriteFramesWithFile("dog/dog.plist");

  workNode = Sprite::createWithSpriteFrameName(defPosFilename);

  return workNode;
}

// =============================================================================

void DogKeeper::doMove(const Vec2 newPos, const MoveDirection moveDirection,
                       CallFunc *notifyScene) {
  log("%s: move requested to %f:%f", __func__, newPos.x, newPos.y);

  FiniteTimeAction *actionMove = MoveTo::create(ITEM_MOVE_TIME, newPos);
  actionMove->setTag(IAT_MOVE);

  Sequence *seq = Sequence::create(actionMove, notifyScene, nullptr);
  workNode->runAction(seq);

  if ((currentMoveDirection != moveDirection) ||
      (currentMoveDirection == MOVE_DIRECTION_NO_MOVE)) {
    workNode->stopAllActionsByTag(IAT_ANIMATION);

    Animation *animation     = prepareAnimation(moveDirection);
    Animate   *animateAction = Animate::create(animation);
    animateAction->setTag(IAT_ANIMATION);
    Action *rAction = RepeatForever::create(animateAction);
    rAction->setTag(IAT_ANIMATION);
    workNode->runAction(rAction);
    currentMoveDirection = moveDirection;
  }
}

// =============================================================================

void DogKeeper::doSetIdle() {
  log("%s: here", __func__);
  currentMoveDirection = MOVE_DIRECTION_NO_MOVE;
  workNode->stopAllActionsByTag(IAT_ANIMATION);
  workNode->setSpriteFrame(defPosFilename);
}

// =============================================================================

void DogKeeper::doStraightMove(const Vec2 newPos) {
  workNode->setPosition(newPos);
}

// =============================================================================
