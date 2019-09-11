#include "CharacterKeeper.h"

const float ITEM_MOVE_TIME      = 1.0f;
const float ITEM_ANIMATION_TIME = 0.1f; //

// Item action tags
const int IAT_ANIMATION = 10;           //
const int IAT_MOVE      = 20;           //

// =============================================================================
// =============================================================================

CharacterKeeper::CharacterKeeper() {
  workNode                 = nullptr;
  movementFinishedCallback = nullptr;
  currentMoveDirection     = MOVE_DIRECTION_NO_MOVE;

  idlePosSpriteName = "";
  plistFileName     = "";
}

// =============================================================================

CharacterKeeper::~CharacterKeeper() {
  // nothing to do here, but ensure it's called
  log("%s: CharacterKeeper object destroyed", __func__);
}

// =============================================================================

Animation * CharacterKeeper::prepareAnimation(const MoveDirection moveDirection) const {
  std::string templateStr = getAnimationTemplateName(moveDirection);

  SpriteFrameCache *scache = SpriteFrameCache::getInstance();

  Animation *resultAnimation = Animation::create();

  resultAnimation->setDelayPerUnit(0.1);

  char tmps[256];

  for (int i = 1; i <= 3; i++) {
    sprintf(tmps, templateStr.c_str(), i);
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

Node * CharacterKeeper::prepareNode() {
  if (workNode != nullptr) {
    return workNode;
  }

  if (plistFileName.empty() || idlePosSpriteName.empty()) {
    log("%s: incomplete object", __func__);
    return nullptr;
  }

  SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistFileName);

  workNode = Sprite::createWithSpriteFrameName(idlePosSpriteName);

  return workNode;
}

// =============================================================================

void CharacterKeeper::doMove(const Vec2 newPos, const MoveDirection moveDirection,
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

void CharacterKeeper::doSetIdle() {
  if (idlePosSpriteName.empty()) {
    log("%s: incomplete class", __func__);
    return;
  }

  log("%s: here", __func__);

  currentMoveDirection = MOVE_DIRECTION_NO_MOVE;
  workNode->stopAllActionsByTag(IAT_ANIMATION);
  workNode->setSpriteFrame(idlePosSpriteName);
}

// =============================================================================

void CharacterKeeper::doStraightMove(const Vec2 newPos) {
  if (workNode == nullptr) {
    log("%s: err", __func__);
    return;
  }

  workNode->setPosition(newPos);
}

// =============================================================================
