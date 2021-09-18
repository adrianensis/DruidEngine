#pragma once

#include "json.hpp"

#include <algorithm>

#include "Core/Macros.hpp"
#include "Core/Assert/Assert.hpp"

using JSON = nlohmann::ordered_json;

// TODO : use ordered_json only for debug and release? but json for ship version?
// ordered_json will keep the data order, json will sort the data alphabetically

class ISerializable
{
public:
	VIR void serialize(JSON &json) CNS {};
	VIR void deserialize(CNS JSON &json) {};
};

class SerializationUtils
{
public:
	template<class T>
	STC JSON serializeTemplated(T value)
	{
		JSON json;
		if constexpr (IS_BASE_OF(ISerializable, REMOVE_POINTER(REMOVE_REF(T))))
		{
			if constexpr (IS_POINTER(T))
			{
				if(value)
				{
					value->serialize(json);
				}
			}
			else
			{
				value.serialize(json);
			}
		}
		else
		{
			json = value;
		}

		return json;
	}

	template<class T>
	STC void deserializeTemplated(T& value, CNS JSON& json)
	{
		if constexpr (IS_BASE_OF(ISerializable, REMOVE_POINTER(REMOVE_REF(T))))
		{
			if constexpr (IS_POINTER(T))
			{
				if(value)
				{
					value->deserialize(json);
				}
			}
			else
			{
				value.deserialize(json);
			}
		}
		else
		{
			value = json.get<T>();
		}
	}
};