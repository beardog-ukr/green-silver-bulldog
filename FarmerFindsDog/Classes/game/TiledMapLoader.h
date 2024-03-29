#pragma once

#include "cocos2d.h"
using namespace cocos2d;

class TiledMapLoader final {
public:

  TiledMapLoader();
  ~TiledMapLoader();

  TMXTiledMap                    * loadFile(const std::string filename);

  int                              getFarmerStartX() const;
  int                              getFarmerStartY() const;
  int                              getDogStartX() const;
  int                              getDogStartY() const;

  std::vector<std::pair<int, int> >getHomePositions() const;

protected:

  int dogStartX;
  int dogStartY;
  int farmerStartX;
  int farmerStartY;

  std::vector<std::pair<int, int> >homePositions;

  bool findStartPositions();

  TMXTiledMap *workNode;
};
