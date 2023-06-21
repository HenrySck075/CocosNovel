#include "DictExtra.h"


#define validType std::is_base_of<std::string, T>::value || std::is_base_of<const char *, T>::value || std::is_base_of<int, T>::value || std::is_base_of<float, T>::value || std::is_base_of<bool, T>::value || std::is_base_of<double, T>::value || std::is_base_of<cocos2d::Ref*, T>::value
template<typename VT>
VT& CocosNovel::DictionaryExtra::objectOfType(std::string i) {
	static_assert(validType, "Allowed types are std::string, int, float, bool, double, and inheritance of cocos2d::Ref");
	auto o = this->objectForKey(i);
	if (std::is_base_of<std::string, VT>::value || std::is_base_of<const char*, VT>::value) return std::string(static_cast<cocos2d::String*>(o)->getCString());
	else if (std::is_base_of<int, VT>::value) return static_cast<cocos2d::Integer*>(o)->getValue();
	else if (std::is_base_of<float, VT>::value) return static_cast<cocos2d::Float*>(o)->getValue();
	else if (std::is_base_of<double, VT>::value) return static_cast<cocos2d::Double*>(o)->getValue();
	else if (std::is_base_of<bool, VT>::value) return static_cast<cocos2d::Bool*>(o)->getValue();
	else return static_cast<VT>(o);
};

template<typename VT>
void CocosNovel::DictionaryExtra::setObjectWithType(VT value, std::string key) {
	static_assert(validType, "Allowed types are std::string, int, float, bool, double, and inheritance of cocos2d::Ref");
	if (std::is_base_of<std::string, VT>::value || std::is_base_of<const char*, VT>::value) setObject(cocos2d::String(value), key);
	else if (std::is_base_of<int, VT>::value) setObject(cocos2d::Integer(value), key);
	else if (std::is_base_of<float, VT>::value) setObject(cocos2d::Float(value), key);
	else if (std::is_base_of<double, VT>::value) setObject(cocos2d::Double(value), key);
	else if (std::is_base_of<bool, VT>::value) setObject(cocos2d::Bool(value), key);
	else setObject(value, key);
}