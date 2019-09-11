#pragma once

#include "cocos2d.h"
using namespace cocos2d;

#include "CharacterKeeper.h"
#include "MoveDirection.h"

class FarmerKeeper : public CharacterKeeper {
public:

  FarmerKeeper();
  virtual ~FarmerKeeper();

protected:

  std::string getAnimationTemplateName(const MoveDirection moveDirection) const;
};
