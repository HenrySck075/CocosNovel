#include "NovelPage.h"
#include "SimpleAudioEngine.h"
#include "utils.h"
#include "macros.h"
using namespace CocosNovel;
USING_NS_CC;
// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in NovelPage.cpp\n");
}
void NovelTextbox::readingState(bool h) { reading = h; };
bool NovelTextbox::init() {
    /////////////////////////////
    auto textbox = Sprite::create("textbox.png");
    textbox->setAnchorPoint(Vec2(0.5f, 0.5f));
    textbox->setName("textbox");
    auto namebox = Sprite::create("namebox.png");
    namebox->setAnchorPoint(Vec2(0.5f, 0.5f));
    namebox->setName("namebox");
    Size a = textbox->getTextureRect().size;
    namebox->setPosition(-291, a.height / 2+15);
    namebox->setZOrder(-2);
    //ccMenuCallback die = [](Ref* ref) {};
    //auto hoverBtn = MenuItemHoverSprite::create(Sprite::create("choice_idle.png"), Sprite::create("choice_hover.png"), nullptr, die);
    //hoverBtn->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y+50));
    this->addChild(textbox);
    this->addChild(namebox);
    namebox->setVisible(false);
    //this->addChild(hoverBtn, 0);
    return true;
}
void NovelTextbox::loadLine(std::string line, std::string character, float textSpeed) {
    Node* h;
    if ((h = this->getChildByName("textLabel")) != nullptr) {
        h->removeFromParentAndCleanup(true);
    };
    if (character != "") {
        auto nameLabel = Label::createWithTTF(character, "fonts/nameboxFont.ttf", 20, Size::ZERO, TextHAlignment::LEFT);
        nameLabel->enableOutline(Color4B(187, 85, 153, 255), 4);
        nameLabel->setName("nameLabel");
        float a = static_cast<Sprite*>(this->getChildByName("textbox"))->getTextureRect().size.height;
        float b = static_cast<Sprite*>(this->getChildByName("namebox"))->getTextureRect().size.height;
        nameLabel->setPosition(this->getChildByName("namebox")->getPosition() + Vec2(0,4));

        this->addChild(nameLabel);
        this->getChildByName("namebox")->setVisible(true);
    }
    else {
        this->getChildByName("namebox")->setVisible(false);
        auto a = this->getChildByName("nameLabel");
        if (a!=nullptr) a->setVisible(false);
    }
    auto text = Label::createWithTTF("", "fonts/textboxFont.ttf", 22, Size::ZERO, TextHAlignment::LEFT);
    text->setAnchorPoint(Vec2(0, 1));
    text->setName("textLabel");
    Size c = static_cast<Sprite*>(this->getChildByName("textbox"))->getTextureRect().size * 0.9;
    text->setDimensions(c.width, c.height);
    auto scheduler = this->getScheduler();
    if (scheduler->isScheduled("no", this)) scheduler->unschedule("no", this);
    text->setString(line);
    text->setPosition(-371, 47);
    text->enableOutline(Color4B(0, 0, 0, 255), 1);
    this->addChild(text);
    auto strLen = text->getStringLength();
    for (int i = 0; i < strLen; i++) {
        auto h = text->getLetter(i);
        if (h != nullptr) {
            h->setVisible(false);
        }
    }
    reading = true;
    auto questionable = CallFunc::create([=]() {
        for (int i = 0; i < strLen; i++) {

            Sprite* letter = text->getLetter(i);
            if (letter != nullptr) {
                auto func = CallFuncN::create([=](Ref* sender) {
                    letter->setVisible(true);

                    // TODO: add the callback to set the "continue" signal (or smth) when the last letter shown
                    /*
                    if (callback && i == (strLen - 1)) {
                        try {
                            callback(label);
                        }
                        catch (std::bad_function_call e) {
                            log("BaseScene:: DelayedReveal - %s : Bad callback!", __FUNCTION__);
                        }
                    }
                    */
                    });


                auto delay = DelayTime::create(textSpeed * i / 10);
                auto sequence = Sequence::create(delay, func, nullptr);
                letter->runAction(sequence);
            }
        }
        reading = false;
        static_cast<NovelPage*>(this->getParent())->readingState(false);
        });
    //auto funcseq = Sequence::create(questionable, CallFunc::create([&](){reading=false;}))
    questionable->setTag(1337);
    text->runAction(questionable);
}


void NovelPage::readingState(bool h) { reading = h; };
bool NovelPage::init(std::string scriptFile, float delay) {
    if (!Layer::init())
    {
        return false;
    }
    cocos2d::log(std::to_string(scripts->count()).c_str());
    Director* directorInst = Director::getInstance();
    auto visibleSize = directorInst->getVisibleSize();
    Vec2 origin = directorInst->getVisibleOrigin();
    textbox->setPosition(visibleSize.width / 2, 120);
    this->addChild(textbox);
    auto h = MenuItemHoverSprite::create(Sprite::create("choice_idle.png"), Sprite::create("choice_hover.png"), nullptr, "sfx/hover.wav", emptyCallback(Ref*));
    h->setPosition(visibleSize / 2);
    this->addChild(h);
    buttons.push_back(h);
    this->runAction(DelayTime::create(delay));
    this->a();
    reading = true;
    return true;
}


void NovelPage::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
void NovelPage::onEndScript() { menuCloseCallback(nullptr); };
bool NovelPage::nonBlocking(EventMouse*e) {

    Size visibleSize = Director::getInstance()->getVisibleSize();

    for (MenuItemHoverSprite* n : buttons) {
        Vec2 locationInNode = n->convertToNodeSpace(Vec2(e->getCursorX(), e->getCursorY() + visibleSize.height));
        Rect r = Rect(0, 0, n->getContentSize().width, n->getContentSize().height);
        if (r.containsPoint(locationInNode)) {
            n->onClick();
            buttons.erase(buttons.begin());
            return false;
        }
    }
    return true;
}
void NovelPage::onMouseDown(EventMouse* event) {
    if (currInstruct[currInstructName] > getDEObj(this->scripts, currInstructName, std::vector<DictionaryExtra*>).size()-1) {
        callHistory.pop_back();
        currInstruct.erase(currInstructName);
        if (callHistory.size() == 0) {
            onEndScript();
        }
        else {
            currInstructName = callHistory.back();
            currInstruct[currInstructName] += 1;
        }
    };
    if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT && nonBlocking(event)){
        if (reading) {
            Node* e;
            if ((e = this->getChildByName("textLabel")) != nullptr) {
                e->stopActionByTag(1337);
                Label* d = static_cast<Label*>(e);

                auto strLen = d->getStringLength();
                for (int i = 0; i < strLen; i++) {
                    auto h = d->getLetter(i);
                    if (h != nullptr) {
                        h->setVisible(true);
                    }
                }
            };
            reading = false;
            textbox->readingState(false);
        } else if (hiddenUI) {
            textbox->setVisible(true);
            hiddenUI = false;
        } else {
            a();
        }
    } else if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) {
        textbox->setVisible(!textbox->isVisible());
    }
}
ActionInterval* actionWithEasing(std::string easing, ActionInterval* action) {
    ActionInterval* easeaction;
    if (easing == "easein") easeaction = EaseIn::create(action, 0);
    if (easing == "easeout") easeaction = EaseOut::create(action, 0);
    else easeaction = action;
    return easeaction;
}
Vector<FiniteTimeAction*> NovelPage::createTransforms(std::string transformName, Node* node) {
    auto attributes = getDEObj(transform, "attributes", DictionaryExtra*);
    auto transformList = getDEObj(transform, "defaultTf", std::vector<std::vector<std::string>>);
    auto showTf = getDEObj(transform, "showTf", std::vector<std::vector<std::string>>);
    auto replaceTf = getDEObj(transform, "replaceTf", std::vector<std::vector<std::string>>);
    Vector<FiniteTimeAction*> actions;
    std::string currentEasing = nullptr;
    float easingTime = 0;

    bool nodeAlreadyShown = false;
    if (node->getParent() != nullptr) nodeAlreadyShown = true;

    if (!nodeAlreadyShown) transformList.insert(transformList.end(), showTf.begin(), showTf.end());
    else transformList.insert(transformList.end(), replaceTf.begin(), replaceTf.end());

    for (auto& t : transformList) {
        //easing
        if (t[0] == "easein") {
            currentEasing = t[0];
            easingTime = std::stof(t[1]);
        };

        if (t[0] == "easeend") currentEasing = nullptr;
        if (t[0] == "xanchor") actions.pushBack(CallFunc::create([node,t](){ 
            node->setAnchorPoint(Vec2(
                std::stof(t[1]), node->getAnchorPoint().y)
            );
        }));
        if (t[0] == "yanchor") actions.pushBack(CallFunc::create([node,t](){ 
            node->setAnchorPoint(Vec2(
                node->getAnchorPoint().x, std::stof(t[1]))
            );
        }));
        if (t[0] == "xpos") actions.pushBack(actionWithEasing(currentEasing, MoveTo::create(easingTime,Vec2(std::stof(t[1]),node->getPositionY()))));
        if (t[0] == "ypos") actions.pushBack(actionWithEasing(currentEasing, MoveTo::create(easingTime,Vec2(node->getPositionX(),std::stof(t[1])))));
        
        if (t[0] == "alpha") actions.pushBack(actionWithEasing(currentEasing, FadeTo::create(easingTime, std::stof(t[1]))));
    };

    node->runAction(Sequence::create(actions));

};

void NovelPage::a() {
    // one more check since if we ended a script, it'll increase the index of the caller
    // and pull out the "vector subscript out of range" error if there's no more script in it
    if (currInstruct[currInstructName] > getDEObj(scripts, currInstructName, std::vector<DictionaryExtra*>).size() - 1) onEndScript(); 
    while (novelParam("command") != "say") {
        if (novelParam("command") == "alias") {
            characterAlias[novelParam("alias")] = novelParam("aliasTarget");
        }
        if (novelParam("command") == "call") {
            callHistory.push_back(novelParam("call"));
            currInstructName = novelParam("call");

            continue; // to prevent increasing the instruction index (below)
        }
        if (novelParam("command") == "transform") {
            
        }
        currInstruct[currInstructName]+=1;
    };
    reading = true;
    textbox->loadLine(novelParam("script"), characterAlias[novelParam("character")], textSpeed);
    currInstruct[currInstructName]+=1;
}

void NovelPage::setMouseListener()
{
    // Create a mouse listener
    mouseListener->onMouseDown = CC_CALLBACK_1(NovelPage::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}