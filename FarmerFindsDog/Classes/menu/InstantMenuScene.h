#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class MainGameScene;

class InstantMenuScene : public cocos2d::Scene {
public:

  /**
   * Creates a node
   *
   * This kind of node appears on the screen when game becomes interrupted
   * Like when menu gets called during game
   *
   * @param requiredSize Size of the nore requested.
   */
  static cocos2d::Scene* create(MainGameScene          *interruptedScene,
                                cocos2d::RenderTexture *sceneBackground);

CC_CONSTRUCTOR_ACCESS:
  InstantMenuScene();
  InstantMenuScene(cocos2d::RenderTexture *sceneBackground);

  ~InstantMenuScene();

  // virtual bool init();
  bool initWithBackground(cocos2d::RenderTexture *sceneBackground);

protected:

  void processResumeRq(Ref *pSender);
  void processExitRq(Ref *pSender);

  MainGameScene *interruptedScene;

  cocos2d::Node* prepareMenuNode();
};
