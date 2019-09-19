#include "TiledBackgroundNode.h"

using namespace cocos2d;
using namespace std;


const string backgroundFileName("gui/paper_background.png");

// --- -----------------------------------------------------------------------

Node * TiledBackgroundNode::create(const Size& requiredSize) {
  Node *resultNode = Node::create();

  resultNode->setAnchorPoint(Vec2(0, 0));

  int rowPos = 0;
  int watchi = 0;

  do {
    int colPos     = 0;
    Sprite *sprite = nullptr;

    do {
      sprite = Sprite::create(backgroundFileName);

      if (nullptr == sprite) {
        log("%s: failed to load file %s", __func__, backgroundFileName.c_str());
        return nullptr;
      }

      sprite->setAnchorPoint(Vec2(0, 0));
      sprite->setPosition(Vec2(colPos, rowPos));

      resultNode->addChild(sprite);

      colPos += sprite->getContentSize().width;
    }
    while (colPos < requiredSize.width);

    rowPos += sprite->getContentSize().height;
    watchi++;
  }
  while ((rowPos < requiredSize.height) && (watchi < 10));

  resultNode->setPosition(Vec2(0, 0));
  resultNode->setContentSize(requiredSize);

  return resultNode;
}

// --- -----------------------------------------------------------------------
