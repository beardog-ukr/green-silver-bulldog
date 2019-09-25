#include "menu/KeyBindingsMenuNode.h"

#include "common/GameSettings.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace std;

static const string panelFileName       = "gui/menu_panel.png";
static const string activePanelFileName = "gui/menu_panel_active.png";
static const string fontForButtons      = "fonts/Mr_JUNKER_MSX.ttf";

// --- -----------------------------------------------------------------------

KeyBindingsMenuNode::KeyBindingsMenuNode() {
  captions[0]        = "Forward: %s";
  requiredActions[0] = RequiredAction::RA_GO_UP;

  captions[1]        = "Back: %s";
  requiredActions[1] =  RequiredAction::RA_GO_DOWN;

  captions[2]        =  "Left: %s";
  requiredActions[2] = RequiredAction::RA_GO_LEFT;

  captions[3]        =  "Right: %s";
  requiredActions[3] = RequiredAction::RA_GO_RIGHT;

  captions[4]        = "Stop: %s";
  requiredActions[4] = RequiredAction::RA_STOP_MOVING;

  captions[5]        = "Dog, follow: %s";
  requiredActions[5] = RequiredAction::RA_FOLLOW;

  captions[6]        = "Dog, go home: %s";
  requiredActions[6] = RequiredAction::RA_GO_HOME;

  listeningButtonIndex = -1;
}

// --- -----------------------------------------------------------------------

KeyBindingsMenuNode::~KeyBindingsMenuNode() {
  log("%s: here, removing KeyBindingsMenuNode", __func__);
}

// --- -----------------------------------------------------------------------

KeyBindingsMenuNode * KeyBindingsMenuNode::create() {
  KeyBindingsMenuNode *result = new (std::nothrow) KeyBindingsMenuNode();

  // Note, init()  here is inherited from Node
  if (result && result->init()) {
    result->autorelease();
    result->prepareKeyBindingsNode();
  }
  else {
    CC_SAFE_DELETE(result); // deletes and sets ptr to NULL
  }
  return result;
}

// --- -----------------------------------------------------------------------

void KeyBindingsMenuNode::prepareKeyBindingsNode() {
  // ---
  const Size parentSize = Director::getInstance()->getVisibleSize();
  const Size resultSize(parentSize.width / 2, parentSize.height);

  setContentSize(resultSize);

  // --- prepare buttons
  const int amountOfButtons = 7;
  const int buttonXPos      = resultSize.width / 2;

  const int buttonYStep = parentSize.height / (amountOfButtons + 1);

  GameSettings *gs = GameSettings::getInstance();

  for (int buttonIdx = 0; buttonIdx < amountOfButtons; buttonIdx++) {
    Button *btn = Button::create(panelFileName, activePanelFileName);


    btn->setTitleText(makeCaptionForButton(buttonIdx));
    btn->setTitleFontName(fontForButtons);
    btn->setTitleFontSize(24);

    auto bbcb = std::bind(&KeyBindingsMenuNode::processButtonCall, this, (int)buttonIdx);
    btn->addClickEventListener(bbcb);

    int buttonYPos = resultSize.height - buttonYStep * (buttonIdx + 1);
    btn->setPosition(Vec2(buttonXPos, buttonYPos));
    addChild(btn);

    buttons[buttonIdx] = btn;
  }

  // --- result position
  setAnchorPoint(Vec2(0.5, 0.5));


  auto keyboardListener = EventListenerKeyboard::create();

  keyboardListener->onKeyPressed = CC_CALLBACK_2(KeyBindingsMenuNode::onKeyPressedScene, this);

  // also possible to Director::getInstance()->getEventDispatcher()
  // instead of _eventDispatcher
  _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

// --- -----------------------------------------------------------------------

void KeyBindingsMenuNode::processButtonCall(const int buttonIndex) {
  log("%s:here for %d", __func__, buttonIndex);

  if (listeningButtonIndex >= 0) {
    // if already listening to setting for some button,
    // restore that button to previous condition
    buttons[listeningButtonIndex]->setTitleText(makeCaptionForButton(buttonIndex));
  }

  listeningButtonIndex = buttonIndex;
  const string newCaption = makeEmptyCaptionForButton(buttonIndex);
  buttons[listeningButtonIndex]->setTitleText(newCaption);

  log("%s: changed %d's caption to %s", __func__, buttonIndex, newCaption.c_str());
}

// --- -----------------------------------------------------------------------

string KeyBindingsMenuNode::makeCaptionForButton(const int buttonIndex) const {
  if ((buttonIndex < 0) || (buttonIndex >= amountOfButtons)) {
    return "";
  }

  char buff[100];

  const GameSettings *gs          = GameSettings::getInstance();
  const EventKeyboard::KeyCode kc = gs->getKeyCodeForAction(requiredActions[buttonIndex]);

  snprintf(buff, sizeof(buff), captions[buttonIndex].c_str(), keyCodeToString(kc).c_str());

  return string(buff);
}

// --- -----------------------------------------------------------------------

string KeyBindingsMenuNode::makeEmptyCaptionForButton(const int buttonIndex) const {
  if ((buttonIndex < 0) || (buttonIndex >= amountOfButtons)) {
    return "";
  }

  char buff[100];

  snprintf(buff, sizeof(buff), captions[buttonIndex].c_str(), "___");

  return string(buff);
}

// --- -----------------------------------------------------------------------

void KeyBindingsMenuNode::onKeyPressedScene(cocos2d::EventKeyboard::KeyCode keyCode,
                                            cocos2d::Event                 *event) {
  log("%s:here for %d, known as '%s'", __func__, (int)keyCode,
      keyCodeToString(keyCode).c_str());

  if (listeningButtonIndex < 0) {
    return;
  }

  // --- else
  const string kcs = keyCodeToString(keyCode);

  if (kcs != "N/A") {
    GameSettings::getInstance()->setKeyCodeForAction(keyCode,
                                                     requiredActions[listeningButtonIndex]);
  }
  else {
    log("%s: code unrecognized, restore default", __func__);
  }

  buttons[listeningButtonIndex]->setTitleText(makeCaptionForButton(listeningButtonIndex));
  listeningButtonIndex = -1;

  event->stopPropagation();
}

// --- -----------------------------------------------------------------------
