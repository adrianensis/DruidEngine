#pragma once

#include "json.hpp"

using JSON = nlohmann::json;

class ISerializable
{
public:
	virtual JSON serialize() const = 0;
	virtual void deserialize(const JSON &jsonObject) = 0;
};