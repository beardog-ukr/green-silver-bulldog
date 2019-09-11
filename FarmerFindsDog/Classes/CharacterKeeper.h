#pragma once

#include "cocos2d.h"
using namespace cocos2d;

#include "MoveDirection.h"

class CharacterKeeper {
public:

  CharacterKeeper();
  virtual ~CharacterKeeper();

  virtual Node* prepareNode();

  void          doMove(const Vec2          newPos,
                       const MoveDirection moveDirection,
                       CallFunc           *notifySceneCallback);
  virtual void  doStraightMove(const Vec2 newPos);

  virtual void  doSetIdle();

protected:

  Sprite *workNode;

  MoveDirection currentMoveDirection;
  Animation         * prepareAnimation(const MoveDirection moveDirection) const;

  virtual std::string getAnimationTemplateName(const MoveDirection md) const = 0;

  std::string idlePosSpriteName;
  std::string plistFileName;

  CallFunc *movementFinishedCallback;
};
