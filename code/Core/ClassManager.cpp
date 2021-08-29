#include "ClassManager.hpp"
#include "Assert/Assert.hpp"

#include "../../generated-code/ClassManager.includes.generated"

ClassManager::ClassManager()
{
    #include "../../generated-code/ClassManager.generated"
}

void ClassManager::registerClassByName(const std::string &className, std::function<ObjectBase*()> callback)
{
    MAP_INSERT(mInstanceByNameMap, className, callback);
}