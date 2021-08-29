#include "ClassManager.hpp"
#include "Assert/Assert.hpp"

std::map<std::string, ClassRegisterCallback> ClassManager::smRegisters;

//#include "../../generated-code/ClassManager.includes.generated"

ClassRegister::ClassRegister(const std::string &className, ClassRegisterCallback callback)
{
    MAP_INSERT(ClassManager::smRegisters, className, callback);
}

ClassManager::ClassManager()
{
    //#include "../../generated-code/ClassManager.generated"
}

void ClassManager::init()
{
    TRACE()
    
    FOR_MAP(it, smRegisters)
    {
        std::cout << it->first << std::endl;
        registerClassByName(it->first, it->second);
    }
}

void ClassManager::registerClassByName(const std::string &className, ClassRegisterCallback callback)
{
    MAP_INSERT(mInstanceByNameMap, className, callback);
}
