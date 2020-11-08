#pragma once
#include <typeinfo>
#include <string>

struct Component
{
	virtual ~Component() = default;
};

typedef std::string ComponentType;

template <class T>
ComponentType GetComponentType() { return std::string(typeid(T).name()); }