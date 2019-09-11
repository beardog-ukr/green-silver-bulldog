#pragma once

#include "cocos2d.h"
using namespace cocos2d;

#include "MoveDirection.h"

class DogKeeper {
public:

  DogKeeper();
  virtual ~DogKeeper();

  Node* prepareNode();

  void  doMove(const Vec2          newPos,
               const MoveDirection moveDirection,
               CallFunc           *notifySceneCallback);
  void  doStraightMove(const Vec2 newPos);

  void  doSetIdle();

protected:

  Sprite *workNode;

  MoveDirection currentMoveDirection;
  Animation* prepareAnimation(const MoveDirection moveDirection) const;


  CallFunc *movementFinishedCallback;
};
