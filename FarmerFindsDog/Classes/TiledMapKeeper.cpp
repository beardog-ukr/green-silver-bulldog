#include "TiledMapKeeper.h"

#include <cmath> // fmod,
// #include <sstream> // ostringstream

// =============================================================================
// =============================================================================

TiledMapKeeper::TiledMapKeeper() {
  workNode = nullptr;
}

// =============================================================================

TiledMapKeeper::~TiledMapKeeper() {
  // nothing to do here, but ensure it's called
  log("%s: tiled map node destroyed", __func__);
}

// =============================================================================

void TiledMapKeeper::bringMapPointToCenter(const int posX,  const int posY) {
  const int mapWidth  = mapSize.width * tileSize.width;
  const int mapHeight = mapSize.height * tileSize.height;

  //
  const float cX = westBorder + (visibleSize.width - westBorder - eastBorder) / 2 +
                   tileSize.width /  2;
  const float cY = northBorder + (visibleSize.height - southBorder - northBorder) / 2 +
                   tileSize.height / 2;

  log("%s: screen center is as %f : %f ", __func__, cX, cY);

  // common case
  float baseX = posX * tileSize.width;  //
  float baseY = posY * tileSize.height; //

  float pX = baseX;
  float pY = baseY;

  // // exceptional case 1 : x too close to west border
  if (baseX < cX) {
    pX = cX + (tileSize.width - westBorder);
  }
  else if ((mapWidth - baseX) < cX) {
    // exceptional case 2: x too close to east border
    pX = mapWidth - cX + (tileSize.width - eastBorder);
  }

  if (baseY < cY) {
    // exceptional case 3 : y too close to south border
    log("%s: exceptional case 3 : y too close to south border %f %f ", __func__, pY, cY);
    pY = cY + (tileSize.height - southBorder);
  }
  else if ((mapHeight - baseY) < cY) {
    //   // exceptional case 4: y too close to north border
    pY = mapHeight - cY + (tileSize.height - northBorder);
  }

  // finally
  mapBaseX = cX - pX;
  mapBaseY = cY - pY;

  log("%s: new viewpoint is %f : %f ", __func__, mapBaseX, mapBaseY);
  workNode->setPosition(mapBaseX, mapBaseY);

  mapFocusedX = posX;
  mapFocusedY = posY;
}

// =============================================================================

bool TiledMapKeeper::isBadMove(const int tileX, const int tileY) const {
  if ((tileX <= 0) || (tileX >= (mapSize.width - 1))) {
    log("%s: %d:%d is bad move because of X", __func__, tileX, tileY);
    return true;
  }

  if ((tileY <= 0) || (tileY >= (mapSize.height - 1))) {
    log("%s: %d:%d is bad move because of Y", __func__, tileX, tileY);
    return true;
  }

  // log("%s: %d:%d is not on edge (%f:%f)", __func__, tileX, tileY, pos.x,
  // pos.y);
  return false;
}

// =============================================================================

bool TiledMapKeeper::isEdgeTile(const int tileX, const int tileY) const {
  Vec2 pos = getPositionForMapItem(tileX, tileY);

  if ((pos.x - tileSize.width) <= westBorder) {
    log("%s: is on west edge", __func__);
    return true;
  }

  if ((visibleSize.width - (pos.x + tileSize.width)) <= eastBorder) {
    log("%s: is on east edge", __func__);
    return true;
  }

  if ((pos.y - tileSize.height) <= southBorder) {
    log("%s: is on south edge", __func__);
    return true;
  }

  if ((visibleSize.height - (pos.y + tileSize.height)) <= northBorder) {
    log("%s: is on north edge", __func__);
    return true;
  }


  log("%s: %d:%d is not on edge (%f:%f)", __func__, tileX, tileY, pos.x, pos.y);
  return false;
}

// =============================================================================

Vec2 TiledMapKeeper::getPositionForMapItem(const int tileX, const int tileY) const {
  const float posX = tileX * tileSize.width + (tileSize.width / 2);
  const float posY = tileY * tileSize.height + (tileSize.height / 2);

  log("%s: tile pos is  %f : %f ",    __func__, posX,     posY);
  log("%s: map base is at  %f : %f ", __func__, mapBaseX, mapBaseY);

  const float x = posX + mapBaseX;
  const float y = posY + mapBaseY;

  log("%s: item position on the screen is %f : %f ", __func__, x, y);

  return Vec2(x, y);
}

// =============================================================================

Node * TiledMapKeeper::prepareNode()
{
  if (workNode != nullptr) {
    return workNode;
  }

  std::string mapFilename = "tiles/m02.tmx";

  workNode = TMXTiledMap::create(mapFilename);

  if (workNode == nullptr)  {
    log("failed to load tiled map");
    return nullptr;
  }
  else {
    log("map loaded...");
  }

  tileSize = workNode->getTileSize();
  mapSize  = workNode->getMapSize();

  visibleSize = Director::getInstance()->getVisibleSize();

  float tmpf = fmod(visibleSize.height, tileSize.height);

  if (0 == tmpf) {
    northBorder = tileSize.height / 2;
    southBorder = tileSize.height / 2;
  }
  else {
    northBorder = tmpf / 2;
    southBorder = tmpf / 2;
  }

  tmpf = fmod(visibleSize.width, tileSize.width);

  if (0 == tmpf) {
    eastBorder = tileSize.width / 2;
    westBorder = tileSize.width / 2;
  }
  else {
    eastBorder = tmpf / 2;
    westBorder = tmpf / 2;
  }

  log("%s: borders calculated as %f,%f, %f, %f ", __func__,
      northBorder, southBorder, westBorder, eastBorder);

  return workNode;
}
