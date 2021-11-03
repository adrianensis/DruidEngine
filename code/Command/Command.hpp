#pragma once

#include "Core/Core.hpp"

class CommandArgument: public ObjectBase
{
    GENERATE_METADATA(CommandArgument)
    PRI std::string mName = {}; GETREF_CONST_SET(Name);
    PRI std::string mValue = {}; GETREF_CONST_SET(Value);

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
    PRI std::string mName = {}; GETREF_CONST_SET(Name);
    PRI std::string mArgumentsString = {}; GETREF_CONST_SET(ArgumentsString);
    PRI std::map<std::string, CommandArgument> mArguments = {}; GETREF_CONST(Arguments);

    inline static const std::string smDefaultArgumentValue = ""; 

PUB

    void clearArguments() { mArguments.clear(); };
    void addArgument(const CommandArgument& arg) { MAP_INSERT(mArguments, arg.getName(), arg); };
    bool argumentExists(const std::string& argName) const { return MAP_CONTAINS(mArguments, argName); };
    const std::string& getValue(const std::string& argName) const { return argumentExists(argName) ? mArguments.at(argName).getValue() : smDefaultArgumentValue; };

    COPY(Command)
    {
        DO_COPY(Name)
        DO_COPY(Arguments)
        DO_COPY(ArgumentsString)
    }
};