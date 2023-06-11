#include "utils.h"
#include <regex>
#include <vector>
using namespace std;
using namespace CocosNovel;
vector<string> tokenize(string s, string del) {
    vector<string> res = {};
    size_t pos = 0;
    while ((pos = s.find(del)) != string::npos) {
        string token = s.substr(0, pos);
        s.erase(0, pos + del.length());
    }
    res.push_back(s);
    return res;
};
template<typename T>
string printVec(T vec) {
    ostringstream oss;

    if (!vec.empty())
    {
        // Convert all but the last element to avoid a trailing ","
        copy(vec.begin(), vec.end() - 1,
            ostream_iterator<string>(oss, ","));

        // Now add the last element with no delimiter
        oss << vec.back();
    }

    return oss.str();
}
DictionaryExtra* loadTransform(string transformBlock) {
    istringstream stream(transformBlock);
    string cmdl;
    auto ret = DictionaryExtra::create();
    while (getline(stream, cmdl)) {
        if (cmdl.size() == 0) continue;
        auto cmd = tokenize(cmdl);

        if (cmd[0] == "transform") {
            auto a = DictionaryExtra::create();
            for (auto& param : vector<std::string>(cmd.begin() + 3, cmd.end())) {
                auto p = tokenize(param, "=");
                a->setObjectWithType(p[1], p[0]);
            }
            ret->setObjectWithType(a, "attributes");
        }
    }
    return ret;
}
tuple<DictionaryExtra, DictionaryExtra> loadScript(string scriptFileName) {

    DictionaryExtra transforms;
    DictionaryExtra scripts;
    map<string, string> waitList = { {scriptFileName, "start"} };
    while (!waitList.empty()) {
        vector<DictionaryExtra> scriptVect;
        istringstream scriptStream(cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::FileUtils::getInstance()->fullPathForFilename(waitList.begin()->first + ".novel")));
        string cmdl;
        bool transformProc = false;
        string transformStr = "";
        while (getline(scriptStream, cmdl)) {
            cmdl = regex_replace(cmdl, regex("\r"), "");
            // ignore blank line
            if (cmdl.size() == 0) continue;
            DictionaryExtra obj;
            auto a = tokenize(cmdl, "\"");
            auto cmd = tokenize(a[0]);
            cocos2d::log(cmd[0].c_str());
            // top tier interpreter
            if (false) {
                //so the code looks reasonable
            }
            else if (cmd[0] == "end") {
                if (cmd[1] == "transform") transformProc = false;
            }
            else if (cmd[0] == "transform") {
                transformProc = true;
            }
            // ignore the comment
            else if (cmd[0].find("#") != string::npos) continue;
            // loading the script file with a label
            else if (cmd[0] == "load") {
                // assuming they write "load scriptFile as scriptLabel"
                waitList[cmd[1]] = cmd[3];
            }
            // call the label
            else if (cmd[0] == "call") {
                obj.setObjectWithType("call", "command");
                obj.setObjectWithType(cmd[1], "call");
            }
            // character alias. DO NOT alias character as "mc" unless the player doesn't appear in the story
            else if (cmd[0] == "alias") {
                obj.setObjectWithType("alias", "command");
                auto alias = tokenize(cmd[1], "=");
                cocos2d::log((alias[0] + alias[1]).c_str());
                obj.setObjectWithType(alias[0], "aliasTarget");
                obj.setObjectWithType(alias[1], "alias");
            }
            // treat the line as a say statement
            else {
                a.push_back("hi");
                obj.setObjectWithType("say", "command");
                obj.setObjectWithType(a[1], "script");
                obj.setObjectWithType(regex_replace(a[0], regex(" "), ""), "character");
            }
            if (transformProc) transformStr += cmdl + "\n";
            // wrap the line with double quote if a character is speaking
            if (obj.objectOfType<string>("character").size() != 0) obj.setObjectWithType("\"" + obj.objectOfType<string>("script") + "\"", "script");
            scriptVect.push_back(obj);
        }
        if (!scriptVect.empty()) {
            scripts.setObjectWithType(scriptVect, waitList.begin()->second);
            waitList.erase(waitList.begin());
        };
    }
    return make_tuple(scripts, transforms);
};