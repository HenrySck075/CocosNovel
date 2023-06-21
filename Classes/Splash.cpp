#include "Splash.h"
#include "Buttons.h"
#include "NovelPage.h"
#include "macros.h"
using namespace CocosNovel;
bool SplashScreen::init() {
	// Original init function
	cocos2d::Layer::init();
	this->addChild(menu);
	this->playBtn = MenuItemHoverSprite::create(
		cocos2d::Sprite::create(normalSpriteFile), 
		cocos2d::Sprite::create(hoverSpriteFile), 
		nullptr, 
		hoverSfxFile,
		menuCallback(SplashScreen::onPlayBtn)
	);
	return setup();
}

// Additional init implementation
bool SplashScreen::setup() {
	return true;
}