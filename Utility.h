#ifndef NECROUTILITY_H
#define NECROUTILITY_H

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
};

#endif
