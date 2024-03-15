#ifndef NECROUTILITY_H
#define NECROUTILITY_H

#include <vector>

class Utility
{
public:
	template <typename T> 
	static void RemoveFromVector(std::vector<T>& vec, size_t pos);
};

template <typename T>
void Utility::RemoveFromVector(std::vector<T>& vec, size_t pos)
{ 
	vec.erase(vec.begin() + pos); 
}
#endif
