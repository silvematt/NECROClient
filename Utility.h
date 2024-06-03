#ifndef NECROUTILITY_H
#define NECROUTILITY_H

#include "SDL.h"

#include "string"
#include <vector>

class Utility
{
public:
	template <typename T> 
	inline static void RemoveFromVector(std::vector<T>& vec, size_t pos)
	{
		vec.erase(vec.begin() + pos);
	}

	inline static float Clampf(float d, float min, float max)
	{
		const float t = d < min ? min : d;
		return t > max ? max : t;
	}

	inline static float RandomFloat(float min, float max)
	{
		return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
	}
	
	inline static int TryParseInt(const std::string& str)
	{
		int i = 0;

		try
		{
			i = std::stoi(str);
		}
		catch(...)
		{
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Tried to parse '%s' as INT, but the conversion failed.\n", str.c_str());
			return 0;
		}

		return i;
	}

	inline static float TryParseFloat(const std::string& str)
	{
		float f = 0.0f;

		try
		{
			f = std::stof(str);
		}
		catch (...)
		{
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Tried to parse '%s' as FLOAT, but the conversion failed.\n", str.c_str());
			return 0;
		}

		return f;
	}
};

#endif
