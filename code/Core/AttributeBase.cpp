#include "Core/AttributeBase.hpp"
#include "Core/ObjectBase.hpp"

AttributeRegisterStatic::AttributeRegisterStatic(const std::string& classname, const std::string& name, std::list<AttributeBase>& outList)
{
        //std::cout << "INSERTED " << classname << "::" << name << std::endl;
        outList.push_back(AttributeBase(name, nullptr));
}

AttributeRegister::AttributeRegister(const std::string& name, void* pointer, ObjectBase* owner)
{
        if(!MAP_CONTAINS(owner->mAttributes, name))
        {       
                MAP_INSERT(owner->mAttributes, name, AttributeBase(name, pointer));
        }
}

AttributeBase::AttributeBase(const std::string& name, void* pointer)
{
        mName = name;
        mPointer = pointer;
}