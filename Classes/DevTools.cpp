#include "DevTools.h"

void NovelDevTools::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_F11) {
        auto scene = Director::getInstance()->getRunningScene();

    }
}
