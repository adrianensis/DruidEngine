#pragma once

#include "Core/Core.hpp"

class CommandArgument: public ObjectBase
{
    GENERATE_METADATA(CommandArgument)
    PRI_M(SStr, Name, GETREF_CONST_SET);
    PRI_M(SStr, Value, GETREF_CONST_SET);

PUB

    COPY(CommandArgument)
    {
        DO_COPY(Name)
        DO_COPY(Value)
    }
};

class Command: public ObjectBase
{
    GENERATE_METADATA(Command)
    PRI_M(SStr, Name, GETREF_CONST_SET);
    PRI_M(SStr, ArgumentsString, GETREF_CONST_SET);
    PRI_M(SMap(SStr, CommandArgument), Arguments, GETREF_CONST);

    inline static const SStr smDefaultArgumentValue = ""; 

PUB

    void clearArguments() { mArguments.clear(); };
    void addArgument(const CommandArgument& arg) { MAP_INSERT(mArguments, arg.getName(), arg); };
    bool argumentExists(const SStr& argName) const { return MAP_CONTAINS(mArguments, argName); };
    const SStr& getValue(const SStr& argName) const { return argumentExists(argName) ? mArguments.at(argName).getValue() : smDefaultArgumentValue; };

    COPY(Command)
    {
        DO_COPY(Name)
        DO_COPY(Arguments)
        DO_COPY(ArgumentsString)
    }
};