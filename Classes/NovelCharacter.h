
#ifndef __NOVELCHAR_SPR_H__
#define __NOVELCHAR_SPR_H__
#include "cocos2d.h"

namespace CocosNovel {
    class NovelCharacter : public cocos2d::Sprite {
    public:
        CREATE_FUNC(NovelCharacter)

        void setOffsetX(float offset);
        void setOffsetY(float offset);

        void getOffsetX();
        void getOffsetY();
    }
    protected:
        float originalPosX;
        float originalPosY;

        float offsetX;
        float offsetY;
}
#endif