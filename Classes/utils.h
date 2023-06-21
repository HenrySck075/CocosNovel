#pragma once
#include <string>
#include <vector>
#include <map>
#include <any>
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
	struct Setting {
		std::string normalSpriteFile;
		std::string hoverSpriteFile;
		std::string hoverSfxFile;
	} Settings;
	// idk if you even need this
	bool applySetting(std::string name, std::string value);
	// script interpreter or smth
	std::tuple<DictionaryExtra*, DictionaryExtra*> loadScript(std::string script);
}