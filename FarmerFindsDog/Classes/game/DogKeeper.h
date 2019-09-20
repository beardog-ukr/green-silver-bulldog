#pragma once

#include "cocos2d.h"
using namespace cocos2d;

#include "CharacterKeeper.h"
#include "MoveDirection.h"

class DogKeeper : public CharacterKeeper {
public:

  DogKeeper();
  virtual ~DogKeeper();

protected:

  virtual std::string getAnimationTemplateName(const MoveDirection moveDirection) const;
};
