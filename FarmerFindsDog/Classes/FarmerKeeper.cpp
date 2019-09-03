#include "FarmerKeeper.h"

// #include <cmath>   // fmod, sin, cos
// #include <math.h>
// #include <sstream> // ostringstream

const float SI_MOVE_TIME     = 1.0f;
const float SI_ROTATION_TIME = 0.3f; //

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

Node * FarmerKeeper::prepareNode()
{
  if (workNode != nullptr) {
    return workNode;
  }

  const char spaceshipFilename[] = "farmer/left-2.png";

  std::ostringstream errMsgStream;
  bool errorReceived = false;

  Sprite *spaceshipSprite = nullptr;
  Node   *ssNode          = nullptr;

  do
  {
    spaceshipSprite = Sprite::create(spaceshipFilename);

    if (spaceshipSprite == nullptr)
    {
      errMsgStream << "Failed to load spaceship file " << spaceshipFilename;
      errorReceived = true;
      break;
    }

    ssNode = Node::create();

    if (ssNode == nullptr)
    {
      errMsgStream << "Failed to allocate node ";
      errorReceived = true;
      break;
    }
  } while (false);

  if (errorReceived)
  {
    log("%s: failed, error message: %s", __func__, errMsgStream.str().c_str());
    free(spaceshipSprite);
    free(ssNode);
    return nullptr;
  }

  ssNode->addChild(spaceshipSprite);

  workNode = ssNode;
  return ssNode;
}

// =============================================================================

void FarmerKeeper::doMove(const Vec2 newPos, const float newRotationAngle, CallFunc *notifyScene) {
  log("%s: move requested to %f:%f", __func__, newPos.x, newPos.y);

  // if (isMoving()) {
  //   log("%s: move cancelled because it's moving", __func__);
  //   return;
  // }

  FiniteTimeAction *actionMove = MoveTo::create(SI_MOVE_TIME, newPos);

  // actionMove->setTag(IAC_SPACESHIP_MOVING);
  const float currentRotation = workNode->getRotation();
  log("%s: current rotation %f, requested %f", __func__, currentRotation, newRotationAngle);

  Sequence *seq = nullptr;

  if (currentRotation == newRotationAngle) {
    // no rotation requested, only move
    seq = Sequence::create(actionMove, notifyScene, nullptr);
  }
  else {
    actionMove->setDuration(SI_MOVE_TIME - SI_ROTATION_TIME);

    log("%s: action rotation to %f", __func__, newRotationAngle);
    auto actionRotation = cocos2d::RotateTo::create(SI_ROTATION_TIME, newRotationAngle);
    seq = Sequence::create(actionRotation, actionMove,
                           notifyScene, nullptr);
  }

  workNode->runAction(seq);
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
