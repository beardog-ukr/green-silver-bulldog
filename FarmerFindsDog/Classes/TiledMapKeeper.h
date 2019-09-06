#pragma once

#include "cocos2d.h"
using namespace cocos2d;

class TiledMapKeeper {
public:

  TiledMapKeeper();
  virtual ~TiledMapKeeper();

  Node* prepareNode();

  void  bringMapPointToCenter(const int posX,
                              const int posY);

  Vec2  getPositionForMapItem(const int tileX,
                              const int tileY) const;

  bool  isEdgeTile(const int tileX,
                   const int tileY) const;

  bool  isBadMove(const int tileX,
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
};
