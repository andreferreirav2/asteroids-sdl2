#pragma once
#include <typeinfo>
#include <string>


struct Component
{
	virtual ~Component() = default;
};


typedef size_t ComponentType;

template <class T>
constexpr ComponentType GetComponentType() { return typeid(T).hash_code(); }