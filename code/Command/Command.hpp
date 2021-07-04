#pragma once

#include "Core/ObjectBase.hpp"

#include <string>
#include <vector>

CLASS(CommandArgument, ObjectBase)
{
    PRI(Name, GETREF_CONST_SET, std::string);
    PRI(Value, GETREF_CONST_SET, std::string);

public:

    COPY(CommandArgument)
    {
        DO_COPY(Name)
        DO_COPY(Value)
    }
};

CLASS(Command, ObjectBase)
{
    PRI(Name, GETREF_CONST_SET, std::string);
    PRI(Args, GETREF_CONST_SET, std::vector<CommandArgument>);

public:

    void clearArgs() { mArgs.clear(); };
    void addArg(const CommandArgument& arg) { mArgs.push_back(arg); };

    COPY(Command)
    {
        DO_COPY(Name)
        DO_COPY(Args)
    }
};