#pragma once
#include <typeinfo>
#include <string>

class Component
{
public:
	virtual ~Component() = default;
};

typedef size_t ComponentType;

template <class T>
ComponentType GetComponentType() { return typeid(T).hash_code(); }