#pragma once

#include "cocos2d.h"

class TiledBackgroundNode {
public:

  /**
   * Creates a node with tiled background
   *
   * Tiled background node is  the one that has a lot of tiles forming an image
   *of requested size
   *
   * @param requiredSize Size of the nore requested.
   */
  static cocos2d::Node* create(const cocos2d::Size& requiredSize);
};
