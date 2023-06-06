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
#include <third/rapidjson/rapidjson.h>
#include "third/exprtk/exprtk.hpp"
//for logging bc idk how cout doesnt work, im bad at cpp
#include "cocos2d.h"
std::vector<std::string> tokenize(std::string s, std::string del = " ");

// script interpreter or smth
std::tuple<
    std::map<std::string, std::vector<std::map<std::string, std::string>>>,
    std::map<std::string, std::map<std::string, std::map<std::string, std::string>>>
>  loadScript(std::string script);