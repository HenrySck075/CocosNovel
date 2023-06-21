#pragma once

// the core of contributing issue
#define concat(x,y) x##y
#define menuCallback(func) [&](Ref*a){func(a);}
#define emptyCallback(params) [](params){}
#define protect(...) __VA_ARGS__
#define beginElifChain if (false) {}
#define elif else if
#define loadTransformSOWT(n) n, #n
#define getDEObj(o,k,t) o->objectOfType<t>(k)
#define current getDEObj(scripts, currInstructName, std::vector<DictionaryExtra*>)[currInstruct[currInstructName]]
#define novelParam(p) getDEObj(current,p,std::string)
#define transform transforms->objectOfType<DictionaryExtra*>(transformName)