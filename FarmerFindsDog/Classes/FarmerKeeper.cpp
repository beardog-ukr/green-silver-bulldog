#include "FarmerKeeper.h"

// #include <cmath>   // fmod, sin, cos
// #include <math.h>
// #include <sstream> // ostringstream

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

Node * FarmerKeeper::prepareNode()
{
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

  // if (isMoving()) {
  //   log("%s: move cancelled because it's moving", __func__);
  //   return;
  // }

  FiniteTimeAction *actionMove = MoveTo::create(ITEM_MOVE_TIME, newPos);
  actionMove->setTag(IAT_MOVE);

  Sequence *seq = Sequence::create(actionMove, notifyScene, nullptr);
  workNode->runAction(seq);


  Animation *animation = prepareAnimation(moveDirection);
  Action    *aaction   = RepeatForever::create(Animate::create(animation));
  aaction->setTag(IAT_ANIMATION);
  workNode->runAction(aaction);
}

// =============================================================================

bool FarmerKeeper::isMoving() const {
  if (workNode->getNumberOfRunningActions() > 0) {
    return true;
  }

  // else
  return false;
}

// =============================================================================

void FarmerKeeper::doSetIdle() {
  workNode->setSpriteFrame(defPosFilename);

  workNode->stopAllActionsByTag(IAT_ANIMATION);
}

// =============================================================================

void FarmerKeeper::doStraightMove(const Vec2 newPos) {
  workNode->setPosition(newPos);
}

// =============================================================================

// void Spaceship::moveSpaceshipForward(Node *spaceshipNode)
// {
//   log("%s: moving ss forward", __func__);
//
//   Point nextMovePoint = calculateNextMovePoint(spaceshipNode);
//
//   // Rotation gets stopped if spaceship starts moving forward; it feels more
//   // naturally
//   spaceshipNode->stopActionByTag(IAC_SPACESHIP_ROTATION);
//
//   spaceshipNode->stopActionByTag(IAC_SPACESHIP_MOVING);
//
//   FiniteTimeAction *actionMove = MoveBy::create(SS_MOVE_TIME, nextMovePoint);
//   actionMove->setTag(IAC_SPACESHIP_MOVING);
//   spaceshipNode->runAction(actionMove);
//
//   for (const auto& ssNodeChild : spaceshipNode->getChildren())
//   {
//     if (ssNodeChild->getTag() != SS_NODE_TAG_ENGINE_TRACE)
//     {
//       continue;
//     }
//
//     ssNodeChild->setVisible(true);
//
//     auto actionFadeIn  = FadeIn::create(SS_MOVE_TIME / 2);
//     auto actionFadeOut = FadeOut::create(SS_MOVE_TIME / 2);
//
//     // auto callbackFadeIn = CallFunc::create([]() {
//     //   log("faded in");
//     // });
//     auto callbackFadeOut = CallFunc::create([ssNodeChild]() {
//       log("faded out!");
//       ssNodeChild->setVisible(false);
//     });
//
//     ssNodeChild->stopActionByTag(IAC_SPACESHIP_MOVING);
//
//     auto seq = Sequence::create(actionFadeIn, // callbackFadeIn,
//                                 actionFadeOut, callbackFadeOut, nullptr);
//     seq->setTag(IAC_SPACESHIP_MOVING);
//     ssNodeChild->runAction(seq);
//   }
// }

// =============================================================================
