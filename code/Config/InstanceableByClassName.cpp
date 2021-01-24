#include "Memory/Memory.hpp"
#include "Scene/GameObject.hpp"

namespace DE
{
    void registerAllInstaceableByClassName() {

        DE_INSTANCEABLE_BY_CLASSNAME(GameObject)
        
    }
} // namespace DE
