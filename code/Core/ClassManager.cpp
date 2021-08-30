#include "ClassManager.hpp"
#include "Assert/Assert.hpp"

std::map<std::string, ClassRegisterCallback> ClassManager::smRegisters;

//#include "../../generated-code/code.includes.generated"

ClassRegister::ClassRegister(const std::string &className, ClassRegisterCallback callback)
{
    MAP_INSERT(ClassManager::smRegisters, className, callback);
}

void ClassManager::init()
{
    TRACE()

    //#include "../../generated-code/code.generated"
    
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
