#pragma once
#include <typeinfo>
#include <string>

class Component
{
public:
	virtual ~Component() = default;
};

typedef std::string ComponentType;

template <class T>
ComponentType GetComponentType() { return typeid(T).name(); }