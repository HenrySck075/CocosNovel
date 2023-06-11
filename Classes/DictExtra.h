#ifndef __CCNOVEL_DICT__
#define __CCNOVEL_DICT__

#include "cocos2d.h"
#include <any>
#include <string>
namespace CocosNovel {
	class DictionaryExtra : public cocos2d::__Dictionary {
	public:
		template<typename VT>
		VT& objectOfType(std::string i);

		template<typename VT>
		void setObjectWithType(VT value, std::string key);

		CREATE_FUNC(DictionaryExtra)
	};
}
#endif