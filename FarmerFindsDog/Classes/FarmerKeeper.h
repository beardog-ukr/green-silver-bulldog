#pragma once

#include "cocos2d.h"
using namespace cocos2d;

#include "MoveDirection.h"

class FarmerKeeper {
public:

  FarmerKeeper();
  virtual ~FarmerKeeper();

  Node* prepareNode();

  void  doMove(const Vec2          newPos,
               const MoveDirection moveDirection,
               CallFunc           *notifySceneCallback);
  void  doStraightMove(const Vec2 newPos);

  void  doSetIdle();

  bool  isMoving() const;

protected:

  Sprite *workNode;

  Animation* prepareAnimation(const MoveDirection moveDirection) const;


  CallFunc *movementFinishedCallback;
};
