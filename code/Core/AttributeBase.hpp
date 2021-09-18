#pragma once

#include "Core/BasicTypes.hpp"

class ObjectBase;
class AttributeBase;

class AttributeRegisterStatic
{
public:
    AttributeRegisterStatic(CNS SStr &classname, CNS SStr &name, SLst<AttributeBase> &outList);
};

class AttributeRegister
{
public:
    AttributeRegister(CNS SStr &name, void *pointer, ObjectBase *owner);
};

class AttributeBase
{
public:
    AttributeBase(CNS SStr &name, void *pointer);

    void *mPointer = nullptr;
    SStr mName = "";
    AttributeBase *mNext = nullptr;
};