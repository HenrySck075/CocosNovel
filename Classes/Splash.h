#pragma once

#include "cocos2d.h"

namespace CocosNovel {
	class SplashScreen : public cocos2d::Layer {
	public:
		CREATE_FUNC(SplashScreen);

		bool init();

		bool setup();
	private:
		cocos2d::Menu* menu;
		CocosNovel::MenuItemHoverSprite* playBtn;
		CocosNovel::MenuItemHoverSprite* continueBtn;
		CocosNovel::MenuItemHoverSprite* historyBtn;
		CocosNovel::MenuItemHoverSprite* settingsBtn;
		CocosNovel::MenuItemHoverSprite* quitBtn;
	protected:
		void onPlayBtn(Ref*){
			auto scene = cocos2d::Scene::create();
			scene->addChild(NovelPage::create());
			Director::getInstance()->pushScene(scene);
		}
	};
}