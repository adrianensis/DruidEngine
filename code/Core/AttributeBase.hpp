#pragma once

#include "Core/BasicTypes.hpp"

class ObjectBase;
class AttributeBase;

class AttributeRegisterStatic
{
public:
    AttributeRegisterStatic(const SStr &classname, const SStr &name, SLst(AttributeBase) &outList);
};

class AttributeRegister
{
public:
    AttributeRegister(const SStr &name, void *pointer, ObjectBase *owner);
};

class AttributeBase
{
public:
    AttributeBase(const SStr &name, void *pointer);

    void *mPointer = nullptr;
    SStr mName = "";
    AttributeBase *mNext = nullptr;
};