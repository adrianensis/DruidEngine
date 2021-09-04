#pragma once

#include "Core/BasicTypes.hpp"
#include "Core/Singleton.hpp"

class Component;

#define REGISTER_COMPONENT_CLASS_IN_SUBSYSTEM(...) \
    registerComponentClass(__VA_ARGS__::getClassIdStatic());

#define REGISTER_SUBSYSTEM(subsystem) \
    SubsystemsManager::getInstance()->registerSubsystem(subsystem);

#define ADD_COMPONENT_TO_SUBSYSTEM(component) \
    SubsystemsManager::getInstance()->addComponentToSubsystem(component);

CLASS(ISubsystem, ObjectBase)
{
private:
    std::set<ClassId> mAcceptedComponentClasses;

public:
    void registerComponentClass(ClassId classId)
    {
        mAcceptedComponentClasses.insert(classId);
    }

    bool isComponentClassAccepted(ClassId classId)
    {
        return mAcceptedComponentClasses.find(classId) != mAcceptedComponentClasses.end();
        ;
    }

    virtual void init();

    virtual void addComponent(Component * component);
};

class SubsystemsManager : SINGLETON(SubsystemsManager)
{
private:
    std::list<ISubsystem *> mSubsystems;

public:
    void addComponentToSubsystem(Component *component);

    void registerSubsystem(ISubsystem *subsystem)
    {
        mSubsystems.push_back(subsystem);
    }

    const std::list<ISubsystem *> &getSubsystems() const
    {
        return mSubsystems;
    }
};