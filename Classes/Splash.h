#pragma once

#include "cocos2d.h"

namespace CocosNovel {
	class SplashScreen : public cocos2d::Layer {
	public:
		CREATE_FUNC(SplashScreen);

		bool init();

		bool setup();
	};
}