#pragma once

#include "Core/BasicTypes.hpp"

class ObjectBase;
class AttributeBase;

class AttributeRegisterStatic
{
PUB
    AttributeRegisterStatic(const std::string &classname, const std::string &name, std::list<AttributeBase> &outList);
};

class AttributeRegister
{
PUB
    AttributeRegister(const std::string &name, void *pointer, ObjectBase *owner);
};

class AttributeBase
{
PUB
    AttributeBase(const std::string &name, void *pointer);

    void *mPointer = nullptr;
    std::string mName = "";
    AttributeBase *mNext = nullptr;
};