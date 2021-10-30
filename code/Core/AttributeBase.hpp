#pragma once

#include "Core/BasicTypes.hpp"

class ObjectBase;
class AttributeBase;

class AttributeRegisterStatic
{
PUB
    AttributeRegisterStatic(const SStr &classname, const SStr &name, SLst(AttributeBase) &outList);
};

class AttributeRegister
{
PUB
    AttributeRegister(const SStr &name, void *pointer, ObjectBase *owner);
};

class AttributeBase
{
PUB
    AttributeBase(const SStr &name, void *pointer);

    void *mPointer = nullptr;
    SStr mName = "";
    AttributeBase *mNext = nullptr;
};