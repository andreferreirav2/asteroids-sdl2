#pragma once
#include <typeinfo>
#include <string>


struct Component
{
	virtual ~Component() = default;
};


typedef unsigned __int16 ComponentType;

template <class T>
constexpr ComponentType GetComponentType()
{
	return (ComponentType)(typeid(T).hash_code());
}