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
    PRI(SVec(CommandArgument), Args, GETREF_CONST_SET);

public:

    void clearArgs() { mArgs.clear(); };
    void addArg(const CommandArgument& arg) { mArgs.push_back(arg); };

    COPY(Command)
    {
        DO_COPY(Name)
        DO_COPY(Args)
    }
};