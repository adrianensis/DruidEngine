#include "Core/Subsystem.hpp"
#include "Scenes/Component.hpp"

void ISubsystem::init()
{
    REGISTER_SUBSYSTEM(this);
}

void ISubsystem::addComponent(Component *component)
{
    component->setAlreadyAddedToEngine(true);
}

void SubsystemsManager::addComponentToSubsystem(Component *component)
{
    if (component && !component->getAlreadyAddedToEngine())
    {
        ClassId componentClassId = component->getClassId();
        bool added = false;
        FOR_LIST_COND(itSubsytem, mSubsystems, !added)
        {
            ISubsystem *sub = (*itSubsytem);
            if (sub->isComponentClassAccepted(componentClassId))
            {
                sub->addComponent(component);
                added = true;
            }
        }
    }
}
