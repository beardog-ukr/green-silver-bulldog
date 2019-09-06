#include "FarmerKeeper.h"

const float ITEM_MOVE_TIME      = 1.0f;
const float ITEM_ANIMATION_TIME = 0.1f; //

// Item action tags
const int IAT_ANIMATION = 10;           //
const int IAT_MOVE      = 20;           //

const std::string defPosFilename = "f-farmer-2.png";

// =============================================================================
// =============================================================================

FarmerKeeper::FarmerKeeper() {
  workNode                 = nullptr;
  movementFinishedCallback = nullptr;
  currentMoveDirection     = MOVE_DIRECTION_NO_MOVE;
}

// =============================================================================

FarmerKeeper::~FarmerKeeper() {
  // nothing to do here, but ensure it's called
  log("%s: ShipKeeper object destroyed", __func__);
}

// =============================================================================

Animation * FarmerKeeper::prepareAnimation(const MoveDirection moveDirection) const {
  char templateLeft[]  = "left-%d.png";
  char templateRight[] = "right-%d.png";
  char templateUp[]    = "b-farmer-%d.png";
  char templateDown[]  = "f-farmer-%d.png";

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

Node * FarmerKeeper::prepareNode() {
  if (workNode != nullptr) {
    return workNode;
  }

  SpriteFrameCache::getInstance()->addSpriteFramesWithFile("farmer/farmer.plist");


  workNode = Sprite::createWithSpriteFrameName(defPosFilename);

  return workNode;
}

// =============================================================================

void FarmerKeeper::doMove(const Vec2 newPos, const MoveDirection moveDirection,
                          CallFunc *notifyScene) {
  log("%s: move requested to %f:%f", __func__, newPos.x, newPos.y);

  FiniteTimeAction *actionMove = MoveTo::create(ITEM_MOVE_TIME, newPos);
  actionMove->setTag(IAT_MOVE);

  Sequence *seq = Sequence::create(actionMove, notifyScene, nullptr);
  workNode->runAction(seq);

  if (currentMoveDirection != moveDirection) {
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

void FarmerKeeper::doSetIdle() {
  log("%s: here", __func__);
  workNode->stopAllActionsByTag(IAT_ANIMATION);
  workNode->setSpriteFrame(defPosFilename);
}

// =============================================================================

void FarmerKeeper::doStraightMove(const Vec2 newPos) {
  workNode->setPosition(newPos);
}

// =============================================================================
