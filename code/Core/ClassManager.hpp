#pragma once

#include <string>
#include <map>
#include <functional>
#include "Core/Singleton.hpp"
#include "Core/Core.hpp"

#define REGISTER_CLASS_BY_NAME(...) \
    MAP_INSERT(mInstanceByNameMap, #__VA_ARGS__, []() { \
        __VA_ARGS__ *object = nullptr;\
        if constexpr (! std::is_abstract<__VA_ARGS__>::value)\
        {\
            object = NEW(__VA_ARGS__);\
        }\
        else\
        {\
            ASSERT_MSG(false, "Cannot instantiate Abstract Class object " + std::string(#__VA_ARGS__));\
        }\
        return object;\
    });

class ClassManager: public Singleton<ClassManager>
{
private:

    std::map<std::string, std::function<ObjectBase*()>> mInstanceByNameMap;

public:

    ClassManager();

    template<class T, typename = std::enable_if_t<std::is_base_of<ObjectBase, T>::value> >
    T* instanceByName(const std::string &className)
    {
        return static_cast<T*>(instanceByName(className));
    }

    ObjectBase* instanceByName(const std::string &className)
    {
        if(MAP_CONTAINS(mInstanceByNameMap, className))
        {
            return mInstanceByNameMap.at(className)();
        }

        return nullptr;
    }
};