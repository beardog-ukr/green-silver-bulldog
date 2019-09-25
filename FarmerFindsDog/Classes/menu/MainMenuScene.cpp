#include "MainMenuScene.h"
#include "common/GameSettings.h"
#include "menu/KeyBindingsMenuNode.h"
#include "menu/TiledBackgroundNode.h"
#include "game/MainGameScene.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace std;


static const string chechboxOffFilename = "gui/chb_off.png";
static const string chechboxOnFilename  = "gui/chb_on.png";
static const string panelFileName       = "gui/menu_panel.png";
static const string activePanelFileName = "gui/menu_panel_active.png";
static const string fontForButtons      = "fonts/Mr_JUNKER_MSX.ttf";
static const float  switchDelayHalf     = 0.5; // time for removing one menu;
                                               // switching one to another will
                                               // take twice as time (in
                                               // seconds)

// --- -----------------------------------------------------------------------

Scene * MainMenuScene::createScene() {
  return create();
}

// --- -----------------------------------------------------------------------

bool MainMenuScene::init() {
  if (!Scene::init()) // super init first
  {
    return false;
  }

  // --- constructor things
  backButton  = nullptr;
  visibleSize = Director::getInstance()->getVisibleSize();
  origin      = Director::getInstance()->getVisibleOrigin();

  positionActive = Vec2(2 * (visibleSize.width / 3) + origin.x,
                        visibleSize.height / 2 + origin.y);
  positionMovedOut = Vec2(2 * (visibleSize.width / 3) + origin.x,
                          visibleSize.height + (visibleSize.height / 2) + origin.y);
  positionBeforeIn = Vec2(visibleSize.width + (visibleSize.width / 2) + origin.x,
                          (visibleSize.height / 2) + origin.y);

  // --- initialization
  if (!initBackground()) {
    return false;
  }

  currentMenu = prepareBasicMenu();
  currentMenu->setPosition(positionActive);
  addChild(currentMenu);

  if (!initBackButton()) {
    return false;
  }

  if (!initKeyboardProcessing()) {
    return false;
  }

  // --- finally
  return true;
}

// --- -----------------------------------------------------------------------

bool MainMenuScene::initBackground() {
  const Size visibleSize = Director::getInstance()->getVisibleSize();

  Node *const backgroundNode = TiledBackgroundNode::create(visibleSize);

  if (nullptr == backgroundNode) {
    return false;
  }

  addChild(backgroundNode);

  return true;
}

// --- -----------------------------------------------------------------------

bool MainMenuScene::initKeyboardProcessing() {
  auto sceneKeyboardListener = EventListenerKeyboard::create();

  sceneKeyboardListener->onKeyPressed = CC_CALLBACK_2(MainMenuScene::onKeyPressedScene, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneKeyboardListener, this);

  return true;
}

// --- -----------------------------------------------------------------------

void MainMenuScene::onKeyPressedScene(EventKeyboard::KeyCode keyCode,  Event *event)
{
  log("%s: processing key %d pressed", __func__, (int)keyCode);

  switch (keyCode) {
  case EventKeyboard::KeyCode::KEY_X:
    log("%s: Need to get out.", __func__);

    // Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
    break;

  default:
    log("%s: key %d will be ignored", __func__, (int)keyCode);
  }
}

// --- -----------------------------------------------------------------------

Node * MainMenuScene::prepareBasicMenu()  {
  // --- prepare result node
  Node *resultNode      = Node::create();
  const Size parentSize = Director::getInstance()->getVisibleSize();
  const Size resultSize(parentSize.width / 2, parentSize.height);

  resultNode->setContentSize(resultSize);

  // --- prepare buttons
  const int amountOfButtons = 3;
  const int buttonXPos      = resultSize.width / 2;

  const int buttonYStep = parentSize.height / (amountOfButtons + 1);

  string  captions[amountOfButtons] = { "New Game", "Options", "Exit" };
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
  buttons[0]->addClickEventListener(CC_CALLBACK_1(MainMenuScene::processNewGameRq, this));
  buttons[1]->addClickEventListener(CC_CALLBACK_1(MainMenuScene::processOptionsRq, this));
  buttons[2]->addClickEventListener(CC_CALLBACK_1(MainMenuScene::processExitRq, this));

  // --- result position
  resultNode->setAnchorPoint(Vec2(0.5, 0.5));

  // --- finally
  return resultNode;
}

// --- -----------------------------------------------------------------------

// Node * MainMenuScene::prepareKeyBindingsMenu() {
//   // --- prepare result node
//   Node *resultNode      = Node::create();
//   const Size parentSize = Director::getInstance()->getVisibleSize();
//   const Size resultSize(parentSize.width / 2, parentSize.height);

//   resultNode->setContentSize(resultSize);

//   // --- prepare buttons
//   const int amountOfButtons = 7;
//   const int buttonXPos      = resultSize.width / 2;

//   const int buttonYStep = parentSize.height / (amountOfButtons + 1);

//   string captions[amountOfButtons] = { "Forward: %s",    "Back: %s",
//                                        "Left: %s",       "Right: %s",
//                                        "Stop: %s",
//                                        "Dog, follow:%s", "Dog, go home:%s"
//   };

//   RequiredAction actions[amountOfButtons] =
//   { RequiredAction::RA_GO_UP,       RequiredAction::RA_GO_DOWN,
//     RequiredAction::RA_GO_LEFT,     RequiredAction::RA_GO_RIGHT,
//     RequiredAction::RA_STOP_MOVING, RequiredAction::RA_FOLLOW,
// RequiredAction::RA_GO_HOME };

//   Button *buttons[amountOfButtons];

//   GameSettings *gs = GameSettings::getInstance();

//   for (int buttonIdx = 0; buttonIdx < amountOfButtons; buttonIdx++) {
//     Button *btn = Button::create(panelFileName, activePanelFileName);

//     char buff[100];
//     snprintf(buff, sizeof(buff), captions[buttonIdx].c_str(),
//
//
//
//      keyCodeToString(gs->getKeyCodeForAction(actions[buttonIdx])).c_str());
//     btn->setTitleText(string(buff));
//     btn->setTitleFontName(fontForButtons);
//     btn->setTitleFontSize(24);

//     auto bbcb =  std::bind(&MainMenuScene::processKeyBindingButtonCall, this,
// (int)0);
//     backButton->addClickEventListener(bbcb);

//     int buttonYPos = resultSize.height - buttonYStep * (buttonIdx + 1);
//     btn->setPosition(Vec2(buttonXPos, buttonYPos));
//     resultNode->addChild(btn);

//     buttons[buttonIdx] = btn;
//   }

//   string tmps;

//   // --- set calbacks for buttons

//   //
// buttons[0]->addClickEventListener(CC_CALLBACK_1(MainMenuScene::processNewGameRq,
//   // this));
//   //
// buttons[1]->addClickEventListener(CC_CALLBACK_1(MainMenuScene::processOptionsRq,
//   // this));
//   //
// buttons[2]->addClickEventListener(CC_CALLBACK_1(MainMenuScene::processExitRq,
//   // this));

//   // --- result position
//   resultNode->setAnchorPoint(Vec2(0.5, 0.5));

//   // --- finally
//   return resultNode;
// }

// void MainMenuScene::processKeyBindingButtonCall(const int buttonIdx) {
//   log("%s:here", __func__);
// }

// --- -----------------------------------------------------------------------

Node * MainMenuScene::prepareOptionsMenu() {
  Node *resultNode      = Node::create();
  const Size parentSize = Director::getInstance()->getVisibleSize();
  const Size resultSize(parentSize.width / 2, parentSize.height);

  resultNode->setContentSize(resultSize);

  // --- "Key bindings" button
  Button *btn = Button::create(panelFileName, activePanelFileName);
  btn->setTitleText("Key bindings");
  btn->setTitleFontName("fonts/Mr_JUNKER_MSX.ttf");
  btn->setTitleFontSize(28);

  btn->addClickEventListener(CC_CALLBACK_1(MainMenuScene::processKeyBindingsRq, this));

  int buttonXPos = resultSize.width / 2;

  int buttonYPos = resultSize.height - (resultSize.height / 4);
  btn->setPosition(Vec2(buttonXPos, buttonYPos));
  resultNode->addChild(btn);

  // --- "Fog of war" checkbox
  Sprite *containerFoW = Sprite::create(panelFileName);

  if (containerFoW == nullptr) {     // bad idea of error processing
    containerFoW = Sprite::create(); // but better than nothing
    containerFoW->setContentSize(btn->getContentSize());
  }
  const Size containerFoWSize = containerFoW->getContentSize();

  // const int checkboxLevel = resultSize.height / 2;
  CheckBox *checkBox = CheckBox::create(chechboxOffFilename, chechboxOnFilename);
  checkBox->setPosition(Vec2(containerFoWSize.width / 5, containerFoWSize.height / 2));
  checkBox->setSelectedState(GameSettings::getInstance()->getNeedsFogOfWar());
  checkBox->addEventListener(CC_CALLBACK_2(MainMenuScene::processFOWEvent, this));
  containerFoW->addChild(checkBox);

  const char *const textFoW = "Fog of war (TBD)";
  Label *labelFoW           = Label::createWithTTF(textFoW, fontForButtons, 24);

  if (labelFoW == nullptr)  {
    labelFoW = Label::create();
    labelFoW->setString(textFoW);
  }

  // labelFoW->setTextColor(Color4B(200, 200, 200, 255));

  labelFoW->setPosition(Vec2(3 * (containerFoWSize.width / 5),
                             containerFoWSize.height / 2));
  containerFoW->addChild(labelFoW);

  containerFoW->setPosition(Vec2(resultSize.width / 2, resultSize.height / 2));
  resultNode->addChild(containerFoW);


  resultNode->setAnchorPoint(Vec2(0.5, 0.5));

  return resultNode;
}

// --- -----------------------------------------------------------------------

bool MainMenuScene::initBackButton() {
  backButton = Button::create(panelFileName, activePanelFileName);
  backButton->setTitleText("Back");
  backButton->setTitleFontName(fontForButtons);
  backButton->setTitleFontSize(28);

  const Size parentSize = Director::getInstance()->getVisibleSize();
  const Vec2 origin     = Director::getInstance()->getVisibleOrigin();
  int buttonXPos        = parentSize.width / 4 + origin.x;
  int buttonYPos        = parentSize.height - (parentSize.height / 5) + origin.y;
  backButton->setPosition(Vec2(buttonXPos, buttonYPos));

  addChild(backButton);
  backButton->setVisible(false);

  return true;
}

// --- -----------------------------------------------------------------------

void MainMenuScene::processBackButtonCall(const BackButtonTarget backButtonTarget) {
  log("%s: here to get back to %d", __func__, (int)backButtonTarget);

  Node *newMenu           = nullptr;
  BackButtonTarget newBBT = BBT_NONE;

  switch (backButtonTarget) {
  case BBT_BASIC: // From "Options" to "Main"
    newMenu = prepareBasicMenu();
    newBBT  = BBT_NONE;

    GameSettings::getInstance()->saveSettings();
    break;

  case BBT_OPTIONS: // From "Key bindings" to "Options"
    newMenu = prepareOptionsMenu();
    newBBT  = BBT_BASIC;
    break;

  default:
    log("%s: failed to process %d", __func__, (int)backButtonTarget);
    return;
  }

  newMenu->setPosition(positionBeforeIn);
  addChild(newMenu);

  switchStates(newMenu, newBBT);
}

// --- -----------------------------------------------------------------------

void MainMenuScene::processFOWEvent(Ref *, CheckBox::EventType type) {
  GameSettings *gs = GameSettings::getInstance();

  if (CheckBox::EventType::SELECTED == type) {
    gs->setNeedsFogOfWar(true);
  }
  else if (CheckBox::EventType::UNSELECTED == type) {
    gs->setNeedsFogOfWar(false);
  }
}

// --- -----------------------------------------------------------------------

void MainMenuScene::processExitRq(Ref *pSender) {
  log("%s: here", __func__);
  GameSettings::getInstance()->saveSettings();

  Director::getInstance()->end();
}

// --- -----------------------------------------------------------------------

void MainMenuScene::processNewGameRq(Ref *pSender) {
  log("%s: here", __func__);

  Scene *mgs = MainGameScene::createScene();
  Director::getInstance()->replaceScene(mgs);
}

// --- -----------------------------------------------------------------------

void MainMenuScene::processKeyBindingsRq(Ref *pSender) {
  log("%s: here", __func__);

  Node *const kbMenu = KeyBindingsMenuNode::create(); // prepareKeyBindingsMenu();
  kbMenu->setPosition(positionBeforeIn);
  addChild(kbMenu);

  switchStates(kbMenu, BBT_OPTIONS);
}

// --- -----------------------------------------------------------------------

void MainMenuScene::processOptionsRq(Ref *pSender) {
  log("%s: here", __func__);

  Node *const optionsMenu = prepareOptionsMenu();
  optionsMenu->setPosition(positionBeforeIn);
  addChild(optionsMenu);

  switchStates(optionsMenu, BBT_BASIC);
}

// --- -----------------------------------------------------------------------

void MainMenuScene::switchStates(Node *const            newMenu,
                                 const BackButtonTarget backButtonTarget) {
  log("%s: here", __func__);

  // --- init moving actions
  MoveBy   *moveOut = MoveTo::create(switchDelayHalf, positionMovedOut);
  Sequence *seqOut  = Sequence::create(moveOut, RemoveSelf::create(), nullptr);
  currentMenu->runAction(seqOut);
  currentMenu = newMenu;

  MoveTo   *moveIn = MoveTo::create(switchDelayHalf, positionActive);
  Sequence *seqIn  = Sequence::create(DelayTime::create(switchDelayHalf), moveIn, nullptr);

  newMenu->runAction(seqIn);

  // ---
  if (backButtonTarget == BBT_NONE) {
    backButton->setVisible(false);
  }
  else {
    auto bbcb =  std::bind(&MainMenuScene::processBackButtonCall, this, backButtonTarget);
    backButton->addClickEventListener(bbcb);
    backButton->setVisible(true);
  }
}

// --- -----------------------------------------------------------------------
