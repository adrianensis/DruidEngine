#include "Core/AttributeBase.hpp"
#include "Core/ObjectBase.hpp"

AttributeRegisterStatic::AttributeRegisterStatic(CNS SStr &classname, CNS SStr &name, SLst<AttributeBase> &outList)
{
    //std::cout << "INSERTED " << classname << "::" << name << std::endl;
    outList.push_back(AttributeBase(name, nullptr));
}

AttributeRegister::AttributeRegister(CNS SStr &name, void *pointer, ObjectBase *owner)
{
    if (!MAP_CONTAINS(owner->mAttributes, name))
    {
        MAP_INSERT(owner->mAttributes, name, AttributeBase(name, pointer));
    }
}

AttributeBase::AttributeBase(CNS SStr &name, void *pointer)
{
    mName = name;
    mPointer = pointer;
}