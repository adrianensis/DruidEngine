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
    PRI(SStr, ArgumentsString, GETREF_CONST_SET);
    PRI(SMap(SStr, CommandArgument), Arguments, GETREF_CONST);

public:

    void clearArguments() { mArguments.clear(); };
    void addArgument(const CommandArgument& arg) { MAP_INSERT(mArguments, arg.getName(), arg); };

    COPY(Command)
    {
        DO_COPY(Name)
        DO_COPY(Arguments)
        DO_COPY(ArgumentsString)
    }
};