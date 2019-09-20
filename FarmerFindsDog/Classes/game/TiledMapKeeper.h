#pragma once

#include "cocos2d.h"
using namespace cocos2d;

class TiledMapKeeper {
public:

  TiledMapKeeper();
  virtual ~TiledMapKeeper();

  // Node* prepareNode();

  void setWorkNode(TMXTiledMap *tiledNode);

  void bringMapPointToCenter(const int posX,
                             const int posY);

  Vec2 getPositionForMapItem(const int tileX,
                             const int tileY) const;

  // returns true if tile with given coordinates appears at screen edge
  bool isEdgeTile(const int tileX,
                  const int tileY) const;

  // return true if tile with given coordinates is out of bounds or there is
  // some obstacle in this tile
  bool isBadMove(const int tileX,
                 const int tileY) const;

protected:

  Size visibleSize;
  Size tileSize;
  Size mapSize;

  float northBorder;
  float southBorder;
  float westBorder;
  float eastBorder;

  float mapBaseX;
  float mapBaseY;
  int mapFocusedX;
  int mapFocusedY;

  TMXTiledMap *workNode;

  Vec2 getTiledPos(const int tileX,
                   const int tileY) const;
};
