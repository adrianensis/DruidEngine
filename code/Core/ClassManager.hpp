#pragma once

#include <string>
#include <map>
#include <functional>
#include "Core/Singleton.hpp"
#include "Core/ObjectBase.hpp"

#define REGISTER_CLASS_BY_NAME(...) \
    MAP_INSERT(mInstanceByNameMap, #__VA_ARGS__, []() { \
        __VA_ARGS__ *object = nullptr;\
        if constexpr (! std::is_abstract<__VA_ARGS__>::value)\
        {\
            object = NEW(__VA_ARGS__);\
        }\
        \
        return object;\
    });

class ClassManager: public Singleton<ClassManager>
{
private:

    std::map<std::string, std::function<ObjectBase*()>> mInstanceByNameMap;

public:

    ClassManager();

    ObjectBase* instanceByName(const std::string &className)
    {
        if(MAP_CONTAINS(mInstanceByNameMap, className))
        {
            return mInstanceByNameMap.at(className)();
        }

        return nullptr;
    }
};