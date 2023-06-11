#pragma once
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <iostream>
#include <sstream>
#include <fstream>
#include <tuple>
#include <numeric>
#include "DictExtra.h"
#include "third/exprtk/exprtk.hpp"
//for logging bc idk how cout doesnt work, im bad at cpp
#include "cocos2d.h"
std::vector<std::string> tokenize(std::string s, std::string del = " ");

namespace CocosNovel {
	// script interpreter or smth
	std::tuple<DictionaryExtra, DictionaryExtra> loadScript(std::string script);
}