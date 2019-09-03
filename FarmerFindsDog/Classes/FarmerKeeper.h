#pragma once

#include "cocos2d.h"
using namespace cocos2d;

class FarmerKeeper {
public:

  FarmerKeeper();
  virtual ~FarmerKeeper();

  Node* prepareNode();

  void  doMove(const Vec2  newPos,
               const float newRotationAngle,
               CallFunc   *notifySceneCallback);
  void  doStraightMove(const Vec2 newPos);

  bool  isMoving() const;

protected:

  Node *workNode;

  CallFunc *movementFinishedCallback;
};
