#include "Core/Memory.hpp"
#include "Core/Log/Log.hpp"

std::map<std::string, i32> Memory::mAllocationsCounter;

void Memory::init()
{
}

void Memory::terminate()
{
    ECHO("-------- MEM SUMMARY --------")
    FOR_MAP(it, Memory::mAllocationsCounter)
    {
        std::cout << it->first << ": " << it->second << std::endl;
    }
    ECHO("-----------------------------")
}