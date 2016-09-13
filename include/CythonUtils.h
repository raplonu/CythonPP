#pragma once

#include "Common.h"

template<typename T>
class Interface
{
public:
	static T* castVariant(Variant ptr)
	{
		return reinterpret_cast<T*>(ptr);
	}

	static T get(Variant ptr)
	{
		return *reinterpret_cast<T*>(ptr);
	}
};

template<typename T>
Variant castPtr(T* ptr)
{
	return reinterpret_cast<Variant>(ptr);
}

template<typename T>
Variant makePtr(T val)
{
	T* ptr = new T(val);
	return castPtr(ptr);
}

void deletePtr(Variant ptr)
{
	delete reinterpret_cast<int*>(ptr);
}
