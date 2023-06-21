#include "utils.h"
#include "macros.h"
#include <regex>
#include <vector>
using namespace CocosNovel;

std::vector<std::string> tokenize(std::string s, std::string del) {
    std::vector<std::string> res = {};
    std::size_t pos = 0;
    while ((pos = s.find(del)) != std::string::npos) {
        std::string token = s.substr(0, pos);
        s.erase(0, pos + del.length());
    }
    res.push_back(s);
    return res;
};

template<typename T>
std::string printVec(T vec) {
    std::ostringstream oss;

    if (!vec.empty())
    {
        // Convert all but the last element to avoid a trailing ","
        std::copy(vec.begin(), vec.end() - 1,
            std::ostream_iterator<string>(oss, ","));

        // Now add the last element with no delimiter
        oss << vec.back();
    }

    return oss.str();
}

void removeSubstrs(std::string& s,
                   const std::string& p) {
    std::string::size_type n = p.length();

    for (std::string::size_type i = s.find(p); i != std::string::npos; i = s.find(p))
    s.erase(i, n);
}

std::map<std::string, int> no = {
    {"show", 1},
    {"replace", 2}
};

DictionaryExtra* loadTransform(std::string transformBlock) {
    std::istringstream stream(transformBlock);
    std::string cmdl;
    auto ret = DictionaryExtra::create();
    std::vector<std::vector<std::string>> defaultTf;
    std::vector<std::vector<std::string>> showTf;
    std::vector<std::vector<std::string>> replaceTf;
    int curline = 0;
    bool transformProc = false;
    std::string onProc = "";

    while (std::getline(stream, cmdl)) {
        if (cmdl.size() == 0) continue;
        removeSubstrs(cmdl, "    ");
        std::vector<std::vector<std::string>> cmd = {tokenize(cmdl)};
        //transform command is the only case that uses multiple spaces
        if (cmd[0][0] != "transform") {
            while (cmd[0].size() <= 2) {
                std::vector<std::string> no(cmd[0].begin(), cmd[0].end() - 2);
                cmd.insert(cmd.begin(), no);
                cmd[1].erase(cmd[1].begin(), cmd[1].end() - 2);
            };
            transformProc = true;
        }
        for (auto& c : cmd) {
            beginElifChain
            elif (c[0] == "transform") {
                auto a = DictionaryExtra::create();
                for (auto& param : std::vector<std::string>(c.begin() + 2, c.end())) {
                    auto p = tokenize(param, "=");
                    a->setObjectWithType(p[1], p[0]);
                }
                ret->setObjectWithType(a, "attributes");
            }
            elif (c[0] == "end") {
                if (c[1] == "transform") transformProc = false;
                if (c[1] == "on") onProc = "";
                if (c[1] == "easing") c={"ease"};
            }
            else {
                beginElifChain
                elif (onProc == "") defaultTf.push_back(c);
                elif (onProc == "show") showTf.push_back(c);
                elif (onProc == "replace") replaceTf.push_back(c);
            };
        }
        curline++;
    };
    ret->setObjectWithType(loadTransformSOWT(defaultTf));
    ret->setObjectWithType(loadTransformSOWT(showTf));
    ret->setObjectWithType(loadTransformSOWT(replaceTf));
    return ret;
}
bool CocosNovel::applySetting(std::string name, std::string value) {
    beginElifChain
    elif (name == "btnHoverSfx") Settings.hoverSfxFile = value;
    elif (name == "btnHoverSprite") Settings.hoverSpriteFile = value;
    elif (name == "btnnormalSprite") Settings.normalSpriteFile = value;
    return true;
}

std::tuple<DictionaryExtra*, DictionaryExtra*> CocosNovel::loadScript(std::string scriptFileName) {

    DictionaryExtra* transforms;
    DictionaryExtra* scripts;
    std::map<std::string, std::string> waitList = { {scriptFileName, "start"} };
    while (!waitList.empty()) {
        std::vector<DictionaryExtra*> scriptVect;
        std::istringstream scriptStream(cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::FileUtils::getInstance()->fullPathForFilename(waitList.begin()->first + ".novel")));
        std::string cmdl;
        bool transformProc = false;
        std::string transformStr = "";
        std::string transformName = "";
        while (std::getline(scriptStream, cmdl)) {
            cmdl = std::regex_replace(cmdl, std::regex("\r"), "");
            // ignore blank line
            if (cmdl.size() == 0) continue;
            DictionaryExtra* obj;
            auto a = tokenize(cmdl, "\"");
            auto cmd = tokenize(a[0]);
            cocos2d::log(cmd[0].c_str());
            // top tier interpreter
            beginElifChain
            elif (cmd[0] == "set") {
                auto kv = tokenize(cmd[1], "=");
                applySetting(kv[0],kv[1]);
            }
            elif (cmd[0] == "end") {
                if (cmd[1] == "transform") {
                    transformProc = false;
                    auto h = loadTransform(transformStr);
                    transforms->setObjectWithType(h, transformName);
                    transformStr = "";
                    transformName = "";
                    obj->setObjectWithType("transform", "command");
                    obj->setObjectWithType(transformName, "transform");
                }
            }
            elif (cmd[0] == "transform") {
                transformProc = true;
                transformName = cmd[1];
            }
            // ignore the comment
            elif (cmd[0].find("#") != std::string::npos) continue;
            // loading the script file with a label
            elif (cmd[0] == "load") {
                // assuming they write "load scriptFile as scriptLabel"
                waitList[cmd[1]] = cmd[3];
            }
            // call the label
            elif (cmd[0] == "call") {
                obj->setObjectWithType("call", "command");
                obj->setObjectWithType(cmd[1], "call");
            }
            // character alias. DO NOT alias character as "mc" unless the player doesn't appear in the story
            elif (cmd[0] == "alias") {
                obj->setObjectWithType("alias", "command");
                auto alias = tokenize(cmd[1], "=");
                cocos2d::log((alias[0] + alias[1]).c_str());
                obj->setObjectWithType(alias[0], "aliasTarget");
                obj->setObjectWithType(alias[1], "alias");
            }
            // treat the line as a say statement
            else {
                a.push_back("hi");
                obj->setObjectWithType("say", "command");
                obj->setObjectWithType(a[1], "script");
                obj->setObjectWithType(std::regex_replace(a[0], std::regex(" "), ""), "character");
            }
            if (transformProc) transformStr += cmdl + "\n";
            // wrap the line with double quote if a character is speaking
            if (obj->objectOfType<std::string>("character").size() != 0) obj->setObjectWithType("\"" + obj->objectOfType<std::string>("script") + "\"", "script");
            scriptVect.push_back(obj);
        }
        if (!scriptVect.empty()) {
            scripts->setObjectWithType(scriptVect, waitList.begin()->second);
            waitList.erase(waitList.begin());
        };
    }
    return std::make_tuple(scripts, transforms);
};