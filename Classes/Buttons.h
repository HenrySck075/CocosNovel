#pragma once
#ifndef __CCNOVEL_BUTTONS_H__
#define __CCNOVEL_BUTTONS_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include <tuple>
#include "macros.h"
namespace CocosNovel {
    class MenuItemHoverSprite : public cocos2d::MenuItemSprite
    {
    public:
        static MenuItemHoverSprite* MenuItemHoverSprite::create(Node* normalSprite, Node* hoverSprite, Node* disabledSprite, std::string hoverSFXFilename = "", const cocos2d::ccMenuCallback& callback = nullptr)
        {
            MenuItemHoverSprite* ret = new (std::nothrow) MenuItemHoverSprite();

            if (ret && ret->initWithNormalSprite(normalSprite, hoverSprite, disabledSprite, callback))
            {
                ret->hoverFilename = hoverSFXFilename;
                ret->setMouseListener();
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        };
        static MenuItemHoverSprite* MenuItemHoverSprite::create(Node* normalSprite, Node* hoverSprite, Node* disabledSprite, std::string hoverSFXFilename = "", Ref *target, SEL_MenuHandler selector)
        {
            MenuItemHoverSprite* ret = new (std::nothrow) MenuItemHoverSprite();

            if (ret && ret->initWithNormalSprite(normalSprite, hoverSprite, disabledSprite, std::bind(selector, target, std::placeholders::_1)))
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
        void selected();
        void unselected();
        bool prevInBtn = false;
        std::string hoverFilename;
    };
}
#endif // __CCNOVEL_BUTTONS_H__
