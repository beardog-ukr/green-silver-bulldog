#include "DogKeeper.h"

// =============================================================================
// =============================================================================

DogKeeper::DogKeeper() {
  // workNode                 = nullptr;
  movementFinishedCallback = nullptr;
  currentMoveDirection     = MOVE_DIRECTION_NO_MOVE;

  idlePosSpriteName = "f-dog-2.png";
  plistFileName     = "dog/dog.plist";
}

// =============================================================================

DogKeeper::~DogKeeper() {
  // nothing to do here, but ensure it's called
  log("%s: DogKeeper object destroyed", __func__);
}

// =============================================================================

std::string DogKeeper::getAnimationTemplateName(const MoveDirection moveDirection) const {
  char templateLeft[]  = "l-dog-%d.png";
  char templateRight[] = "r-dog-%d.png";
  char templateUp[]    = "b-dog-%d.png";
  char templateDown[]  = "f-dog-%d.png";

  char *templateArr[] = { templateUp, templateDown, templateLeft, templateRight, nullptr };

  char *templateStr = templateArr[moveDirection];

  std::string resultStr = "";

  if (templateStr != nullptr) {
    resultStr = std::string(templateStr);
  }

  return resultStr;
}

// =============================================================================
