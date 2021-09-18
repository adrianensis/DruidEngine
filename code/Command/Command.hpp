#pragma once

#include "Core/Core.hpp"

CLASS(CommandArgument, ObjectBase)
{
    PRI(Name, GETREF_CONST_SET, SStr);
    PRI(Value, GETREF_CONST_SET, SStr);

public:

    COPY(CommandArgument)
    {
        DO_COPY(Name)
        DO_COPY(Value)
    }
};

CLASS(Command, ObjectBase)
{
    PRI(Name, GETREF_CONST_SET, SStr);
    PRI(Args, GETREF_CONST_SET, SVec<CommandArgument>);

public:

    void clearArgs() { mArgs.clear(); };
    void addArg(CNS CommandArgument& arg) { mArgs.push_back(arg); };

    COPY(Command)
    {
        DO_COPY(Name)
        DO_COPY(Args)
    }
};