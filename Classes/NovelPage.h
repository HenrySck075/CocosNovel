
#pragma once
#ifndef __NOVELPAGE_H__
#define __NOVELPAGE_H__

#include "cocos2d.h"
#include "Buttons.h"
#include <vector>
#include <variant>
#include <map>
#include <any>
using namespace cocos2d;

namespace CocosNovel {
    DictionaryExtra* scripts = DictionaryExtra::create();
    DictionaryExtra* transforms = DictionaryExtra::create();
    class NovelTextbox : public Node {
    private:
        bool reading = false;
    public:
        virtual bool init();
        void loadLine(std::string line, std::string character = "", float textSpeed = 0.2f);
        void readingState(bool h);
        CREATE_FUNC(NovelTextbox);
    };


    class NovelPage : public Layer {
    private:
        float textSpeed = 0.2f;
        void onMouseDown(cocos2d::EventMouse* event);
        void setMouseListener();
        bool nonBlocking(EventMouse* e);
        void a();
        NovelTextbox* textbox = NovelTextbox::create();
    protected:
        cocos2d::EventListenerMouse* mouseListener = cocos2d::EventListenerMouse::create();
        std::map<std::string, int> currInstruct = { { "start",0 } };
        std::string currInstructName = "start";
        std::vector<std::string> callHistory = { "start" };
        bool reading = false;
        bool hiddenUI = false;
        std::map<std::string, std::string> characterAlias;
        // it's mostly buttons that blocks the mouse listener (suppose to)
        std::vector<MenuItemHoverSprite*> buttons = {};
    public:
        virtual bool init(std::string scriptFile, float delay);

        // a selector callback
        void menuCloseCallback(Ref* pSender);
        void readingState(bool h);
        void onFinishLine();
        void onEndScript();
        // implement the "static create()" method manually
        static NovelPage* create(std::string scriptFile, float delay = 0)
        {
            NovelPage* pRet = new(std::nothrow) NovelPage();
            if (pRet && pRet->init(scriptFile, 2))
            {
                pRet->setMouseListener();
                pRet->autorelease();
                return pRet;
            }
            else
            {
                delete pRet;
                pRet = nullptr;
                return nullptr;
            }
        }
    };
}

#endif // __NOVELPAGE_H__
