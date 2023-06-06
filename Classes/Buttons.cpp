#include "Buttons.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

MenuItemHoverSprite::~MenuItemHoverSprite() {
    _eventDispatcher->removeEventListener(mouseListener);
};

void MenuItemHoverSprite::onMouseMove(EventMouse* e) {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 locationInNode = convertToNodeSpace(Vec2(e->getCursorX(), e->getCursorY() + visibleSize.height));

    Rect r = Rect(0, 0, getContentSize().width, getContentSize().height);

    // Show selected image if mouse over
    r.containsPoint(locationInNode) ? selected2() : unselected2();
}

void MenuItemHoverSprite::onClick() {
    this->removeFromParentAndCleanup(true);
};
void MenuItemHoverSprite::selected2() {
    selected();
    if (!prevInBtn) CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(hoverFilename.c_str());
    prevInBtn = true;
}
void MenuItemHoverSprite::unselected2() {
    unselected();
    prevInBtn = false;
}
void MenuItemHoverSprite::setMouseListener()
{
    // Create a mouse listener
    mouseListener->onMouseMove = CC_CALLBACK_1(MenuItemHoverSprite::onMouseMove, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}