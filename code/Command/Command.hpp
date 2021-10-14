#pragma once

#include "Core/Core.hpp"

CLASS(CommandArgument, ObjectBase)
{
    PRI(SStr, Name, GETREF_CONST_SET);
    PRI(SStr, Value, GETREF_CONST_SET);

public:

    COPY(CommandArgument)
    {
        DO_COPY(Name)
        DO_COPY(Value)
    }
};

CLASS(Command, ObjectBase)
{
    PRI(SStr, Name, GETREF_CONST_SET);
    PRI(SMap(SStr, CommandArgument), Args, GETREF_CONST);

public:

    void clearArgs() { mArgs.clear(); };
    void addArg(const CommandArgument& arg) { MAP_INSERT(mArgs, arg.getName(), arg); };

    COPY(Command)
    {
        DO_COPY(Name)
        DO_COPY(Args)
    }
};