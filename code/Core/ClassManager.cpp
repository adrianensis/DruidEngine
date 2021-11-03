#include "ClassManager.hpp"
#include "Assert/Assert.hpp"

std::map<std::string, ClassRegisterCallback> ClassManager::smRegisters;

ClassRegister::ClassRegister(const std::string &className, ClassRegisterCallback callback)
{
    MAP_INSERT(ClassManager::smRegisters, className, callback);
}

void ClassManager::init()
{
    TRACE()
    
    FOR_MAP(it, smRegisters)
    {
        registerClassByName(it->first, it->second);
    }
}

void ClassManager::registerClassByName(const std::string &className, ClassRegisterCallback callback)
{
    MAP_INSERT(mInstanceByNameMap, className, callback);
}
