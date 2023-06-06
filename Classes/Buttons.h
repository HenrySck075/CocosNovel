#pragma once
#ifndef __CCNOVEL_BUTTONS_H__
#define __CCNOVEL_BUTTONS_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include <tuple>
class MenuItemHoverSprite : public cocos2d::MenuItemSprite
{
public:
    static MenuItemHoverSprite* MenuItemHoverSprite::create(Node* normalSprite, Node* hoverSprite, Node* disabledSprite, std::string hoverSFXFilename = "")
    {
        MenuItemHoverSprite* ret = new (std::nothrow) MenuItemHoverSprite();

        if (ret && ret->initWithNormalSprite(normalSprite, hoverSprite, disabledSprite, nullptr))
        {
            ret->hoverFilename = hoverSFXFilename;
            ret->setMouseListener();
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    };
    void onClick();
protected:
    MenuItemHoverSprite::~MenuItemHoverSprite();
    cocos2d::EventListenerMouse* mouseListener = cocos2d::EventListenerMouse::create();
private:
    void onMouseMove(cocos2d::EventMouse* e);
    void setMouseListener();
    void selected2();
    void unselected2();
    bool prevInBtn = false;
    std::string hoverFilename;
};

#endif // __CCNOVEL_BUTTONS_H__
