#include "InstantMenuScene.h"
#include "TiledBackgroundNode.h"

#include "game/MainGameScene.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace std;

static const string panelFileName       = "gui/menu_panel.png";
static const string activePanelFileName = "gui/menu_panel_active.png";
static const string fontForButtons      = "fonts/Mr_JUNKER_MSX.ttf";

// --- -----------------------------------------------------------------------

InstantMenuScene::InstantMenuScene() {
  interruptedScene = nullptr;
}

// --- -----------------------------------------------------------------------

InstantMenuScene::~InstantMenuScene() {
  // nothing to do here
  log("%s: here", __func__);
}

// --- -----------------------------------------------------------------------


bool InstantMenuScene::initWithBackground(cocos2d::RenderTexture *sceneBackground) {
  log("%s: here", __func__);

  const Size visibleSize = Director::getInstance()->getVisibleSize();
  const Vec2 origin      = Director::getInstance()->getVisibleOrigin();

  setContentSize(visibleSize);

  Node *backgroundNode = sceneBackground;

  if (backgroundNode == nullptr) {
    backgroundNode = TiledBackgroundNode::create(visibleSize);
  }

  backgroundNode->setAnchorPoint(Vec2(0, 0));
  backgroundNode->setPosition(Vec2(0, 0));
  addChild(backgroundNode);

  Node *menuNode = InstantMenuScene::prepareMenuNode();

  const Vec2 positionActive = Vec2(2 * (visibleSize.width / 3) + origin.x,
                                   visibleSize.height / 2 + origin.y);

  menuNode->setPosition(positionActive);
  addChild(menuNode);

  setAnchorPoint(Vec2(0, 0));
  setPosition(Vec2(0, 0));

  return true;
}

// --- -----------------------------------------------------------------------

Scene * InstantMenuScene::create(MainGameScene          *inInterruptedScene,
                                 cocos2d::RenderTexture *sceneBackground) {
  InstantMenuScene *ret = new (std::nothrow) InstantMenuScene();

  if (ret && ret->initWithBackground(sceneBackground))
  {
    ret->autorelease();
    ret->interruptedScene = inInterruptedScene;
  }
  else
  {
    CC_SAFE_DELETE(ret);
  }
  return ret;
}

// --- -----------------------------------------------------------------------

Node * InstantMenuScene::prepareMenuNode() {
  // --- prepare result node
  Node *resultNode      = Node::create();
  const Size parentSize = Director::getInstance()->getVisibleSize();
  const Size resultSize(parentSize.width / 2, parentSize.height);

  resultNode->setContentSize(resultSize);

  // --- prepare buttons
  const int amountOfButtons = 3;
  const int buttonXPos      = resultSize.width / 2;

  const int buttonYStep = parentSize.height / (amountOfButtons + 1);

  string  captions[amountOfButtons] = { "Resume", "Options", "Exit to main" };
  Button *buttons[amountOfButtons];

  for (int buttonIdx = 0; buttonIdx < amountOfButtons; buttonIdx++) {
    Button *btn = Button::create(panelFileName, activePanelFileName);
    btn->setTitleText(captions[buttonIdx]);
    btn->setTitleFontName(fontForButtons);
    btn->setTitleFontSize(28);

    int buttonYPos = resultSize.height - buttonYStep * (buttonIdx + 1);
    btn->setPosition(Vec2(buttonXPos, buttonYPos));
    resultNode->addChild(btn);

    buttons[buttonIdx] = btn;
  }


  // --- set calbacks for buttons
  buttons[0]->addClickEventListener(CC_CALLBACK_1(InstantMenuScene::processResumeRq, this));

  // buttons[1]->addClickEventListener(CC_CALLBACK_1(InstantMenuScene::processOptionsRq,
  // this));
  buttons[2]->addClickEventListener(CC_CALLBACK_1(InstantMenuScene::processExitRq, this));

  // --- result position
  resultNode->setAnchorPoint(Vec2(0.5, 0.5));

  // --- finally
  return resultNode;
}

// --- -----------------------------------------------------------------------

void InstantMenuScene::processExitRq(Ref *pSender) {
  log("%s: here", __func__);

  // Director::getInstance()->end();
  if (interruptedScene != nullptr) {
    interruptedScene->setImmediateExit(true);
  }
  Director::getInstance()->popScene();
}

// --- -----------------------------------------------------------------------

void InstantMenuScene::processResumeRq(Ref *pSender) {
  log("%s: here", __func__);

  Director::getInstance()->popScene();
}

// --- -----------------------------------------------------------------------
