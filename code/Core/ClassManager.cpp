#include "ClassManager.hpp"
#include "Assert/Assert.hpp"

SMap<SStr, ClassRegisterCallback> ClassManager::smRegisters;

//#include "../../generated-code/code.includes.generated"

ClassRegister::ClassRegister(CNS SStr &className, ClassRegisterCallback callback)
{
    MAP_INSERT(ClassManager::smRegisters, className, callback);
}

void ClassManager::init()
{
    TRACE()

    //#include "../../generated-code/code.generated"
    
    FOR_MAP(it, smRegisters)
    {
        registerClassByName(it->first, it->second);
    }
}

void ClassManager::registerClassByName(CNS SStr &className, ClassRegisterCallback callback)
{
    MAP_INSERT(mInstanceByNameMap, className, callback);
}
