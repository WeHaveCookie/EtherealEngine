#pragma once

namespace ValueType
{
	enum Enum
	{
		Float,
		Int,
		Uint,
		String,
		Bool,
		Vector2,
	};
}

static void checkAndAffect(rapidjson::Document* doc, const char* nameAttribut, ValueType::Enum type, void** attPtr, void* defaultValue = NULL, int sizeOfDefaultValue = 0)
{
	if (doc->HasMember(nameAttribut))
	{
		switch (type)
		{
		case ValueType::Float:
			if ((*doc)[nameAttribut].IsFloat())
			{
				float value = (*doc)[nameAttribut].GetFloat();
				memcpy(*attPtr, (void*)&value, sizeof(float));
			}
			break;
		case ValueType::Int:
			if ((*doc)[nameAttribut].IsInt())
			{
				int value = (*doc)[nameAttribut].GetInt();
				memcpy(*attPtr, (void*)&value, sizeof(int));
			}
			break;
		case ValueType::Uint:
			if ((*doc)[nameAttribut].IsUint())
			{
				unsigned int value = (*doc)[nameAttribut].GetUint();
				memcpy(*attPtr, (void*)&value, sizeof(unsigned int));
			}
			break;
		case ValueType::String:
			if ((*doc)[nameAttribut].IsString())
			{
				std::string value = (*doc)[nameAttribut].GetString();
				memcpy(*attPtr, (void*)&value, sizeof(std::string));
			}
			break;
		case ValueType::Bool:
			if ((*doc)[nameAttribut].IsBool())
			{
				bool value = (*doc)[nameAttribut].GetBool();
				memcpy(*attPtr, (void*)&value, sizeof(bool));
			}
			break;
		case ValueType::Vector2:
			if ((*doc)[nameAttribut].IsArray())
			{
				assert((*doc)[nameAttribut].GetArray().Size() == 2);
				float value0 = (*doc)[nameAttribut][0].GetFloat();
				float value1 = (*doc)[nameAttribut][1].GetFloat();
				Vector2 finalVal(value0, value1);
				memcpy(*attPtr, (void*)&finalVal, sizeof(Vector2));
			}
			break;
		default:
			assert(true);
			break;
		}
	}
	else
	{
		if (defaultValue != NULL)
		{
			switch (type)
			{
			case ValueType::Float:
				memcpy(*attPtr, defaultValue, sizeof(float));
				break;
			case ValueType::Int:
				memcpy(*attPtr, defaultValue, sizeof(int));
				break;
			case ValueType::Uint:
				memcpy(*attPtr, defaultValue, sizeof(unsigned int));
				break;
			case ValueType::String:
				memcpy(*attPtr, (void*)&defaultValue, sizeOfDefaultValue);
				break;
			case ValueType::Bool:
				memcpy(*attPtr, defaultValue, sizeof(bool));
				break;
			case ValueType::Vector2:
				memcpy(*attPtr, defaultValue, sizeof(Vector2));
				break;
			default:
				assert(true);
				break;
			}
		}
		else
		{
			assert(true);
		}
	}
}