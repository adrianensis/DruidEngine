#pragma once

#include "Core/BasicTypes.hpp"
#include "Core/ObjectBase.hpp"
#include "Core/Singleton.hpp"
#include "Core/Assert/Assert.hpp"
#include "Core/Memory.hpp"

#define REGISTER_CLASS_BY_NAME(...) \
    ClassRegister __classRegister ## _ ## __VA_ARGS__ = ClassRegister(#__VA_ARGS__, []() { \
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

using ClassRegisterCallback = std::function<ObjectBase*()>;

class ClassRegister
{
    private:        
        std::string mClassName;
        ClassRegisterCallback mCallback;
    public:
        ClassRegister(const std::string &className, ClassRegisterCallback callback);
};

class ClassManager: public Singleton<ClassManager>
{
friend ClassRegister;
private:

    static std::map<std::string, ClassRegisterCallback> smRegisters;

    std::map<std::string, ClassRegisterCallback> mInstanceByNameMap;

public:

    void init();

    void registerClassByName(const std::string &className, ClassRegisterCallback callback);

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