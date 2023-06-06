#include "utils.h"
#include <regex>
using namespace std;
vector<string> tokenize(string s, string del) {
    vector<string> res = {};
    size_t pos = 0;
    while ((pos = s.find(del)) != string::npos) {
        string token = s.substr(0, pos);
        res.push_back(token);
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
cocos2d::Dictionary* loadTransform(string transformBlock) {
    istringstream stream(transformBlock);
    string cmdl;
    while (getline(stream, cmdl)) {
        if (cmdl.size() == 0) continue;
        auto obj = cocos2d::Dictionary::create();
        auto cmd = tokenize(cmdl);

        if (cmd[0] == "transform") {
            auto a = tokenize(cmd[1], "(");
            cmd[1] = a[0];
            cmd.insert(cmd.begin() + 2, a[1]);
            cmd[cmd.size() - 1]= regex_replace(cmd[cmd.size() - 1], regex("):"), "");
        }
    }
}
tuple<
    map<string, vector<map<string, string>>>, // scripts
    map<string, map<string, vector<map<string, string>>>>// transformations
> loadScript(string scriptFileName) {

    map<string, map<string, map<string, string>>> transforms;
    map<string, map<string, vector<map<string, string>>>> scripts;
    map<string, string> waitList = { {scriptFileName, "start"} };
    while (!waitList.empty()) {
        vector<map<string, string>> scriptVect = {};
        istringstream scriptStream(cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::FileUtils::getInstance()->fullPathForFilename(waitList.begin()->first + ".novel")));
        string cmdl;
        bool transformProc = false;
        string transformStr = "";
        while (getline(scriptStream, cmdl)) {
            cmdl = regex_replace(cmdl, regex("\r"), "");

            if (cmdl.size() == 0) continue;
            map<string, string> obj;
            auto a = tokenize(cmdl, "\"");
            auto cmd = tokenize(a[0]);
            cocos2d::log(cmd[0].c_str());
            // image transform
            if (cmd[0] == "transform") {
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
                obj["command"] = "call";
                obj["call"] = cmd[1];
            }
            // character alias. DO NOT alias character as "mc" unless the player doesn't appear in the story
            else if (cmd[0] == "alias") {
                obj["command"] = "alias";
                auto alias = tokenize(cmd[1], "=");
                cocos2d::log((alias[0] + alias[1]).c_str());
                obj["aliasTarget"] = alias[0];
                obj["alias"] = alias[1];
            }
            // treat the line as a say statement
            else {
                obj["command"] = "say";
                obj["script"] = a[1];
                obj["character"] = regex_replace(a[0], regex(" "), "");
            }
            // wrap the line with double quote if a character is speaking
            if (transformProc) transformStr += cmdl + "\n";
            if (obj["character"].size() != 0) obj["script"] = "\"" + obj["script"] + "\"";
            scriptVect.push_back(obj);
        }
        if (!scriptVect.empty()) {
            scripts[waitList.begin()->second] = scriptVect;
            waitList.erase(waitList.begin());
        };
    }
    return scripts;
}