#include "FarmerKeeper.h"

// =============================================================================
// =============================================================================

FarmerKeeper::FarmerKeeper() {
  // workNode                 = nullptr;
  movementFinishedCallback = nullptr;
  currentMoveDirection     = MOVE_DIRECTION_NO_MOVE;

  idlePosSpriteName = "f-farmer-2.png";
  plistFileName     = "farmer/farmer.plist";
}

// =============================================================================

FarmerKeeper::~FarmerKeeper() {
  // nothing to do here, but ensure it's called
  log("%s: ShipKeeper object destroyed", __func__);
}

// =============================================================================

std::string FarmerKeeper::getAnimationTemplateName(const MoveDirection moveDirection) const {
  char templateLeft[]  = "l-farmer-%d.png";
  char templateRight[] = "r-farmer-%d.png";
  char templateUp[]    = "b-farmer-%d.png";
  char templateDown[]  = "f-farmer-%d.png";

  char *templateArr[] = { templateUp, templateDown, templateLeft, templateRight, nullptr };

  char *templateStr = templateArr[moveDirection];

  std::string resultStr = "";

  if (templateStr != nullptr) {
    resultStr = std::string(templateStr);
  }

  return resultStr;
}

// =============================================================================
