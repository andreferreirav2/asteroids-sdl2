#pragma once
#include <set>
#include <algorithm>
#include <iterator >
#include <unordered_map>
#include "Entity.h"
#include "Component.h"
#include "System.h"

typedef unsigned __int32 EntityComponentTypeHash;

class ECSManager
{
public:
	//Entity management
	Entity createEntity()
	{
		return m_nextEntity++;
	}

	void destroyEntity(Entity entity)
	{
		for (ComponentType ct : m_entityToComponentTypes[entity])
		{
			m_componentTypesToEntity[ct].erase(entity);
			m_entityComponentTypeHashToComponent.erase(entityComponentHash(entity, ct));

		}
		m_entityToComponentTypes.erase(entity);
	}

	//Component management
	void addComponent(Entity entity, std::shared_ptr<Component> component)
	{
		if (!component)
		{
			return;
		}

		ComponentType componentType = (ComponentType)(typeid(*component).hash_code());

		m_entityComponentTypeHashToComponent[entityComponentHash(entity, componentType)] = component;
		m_componentTypesToEntity[componentType].insert(entity);
		m_entityToComponentTypes[entity].insert(componentType);
	}

	template <typename T>
	void removeComponentOfType(Entity entity)
	{
		static ComponentType componentType = GetComponentType<T>();

		m_entityToComponentTypes.erase(componentType);
		m_componentTypesToEntity[componentType].erase(entity);
		m_entityComponentTypeHashToComponent.erase(entityComponentHash(entity, componentType));
	}

	template <typename T>
	std::shared_ptr<T> getComponentOfType(Entity entity)
	{
		static ComponentType componentType = GetComponentType<T>();
		EntityComponentTypeHash hash = entityComponentHash(entity, componentType);
		return std::static_pointer_cast<T>(m_entityComponentTypeHashToComponent[hash]);
	}

	template <typename T>
	std::set<Entity> getAllEntitiesWithComponentType()
	{
		static ComponentType componentType = GetComponentType<T>();
		return m_componentTypesToEntity[componentType];
	}

	template <typename T, typename U>
	std::set<Entity> getAllEntitiesWithComponentType()
	{
		return intersect(getAllEntitiesWithComponentType<T>(), getAllEntitiesWithComponentType<U>());
	}

	template <typename T, typename U, typename V>
	std::set<Entity> getAllEntitiesWithComponentType()
	{
		return intersect(getAllEntitiesWithComponentType<T>(), intersect(getAllEntitiesWithComponentType<U>(), getAllEntitiesWithComponentType<V>()));
	}

	template <typename T>
	std::set<std::shared_ptr<T>> getAllComponentsOfType()
	{
		static ComponentType componentType = GetComponentType<T>();
		std::set<std::shared_ptr<T>> components = {};

		for (auto entity : m_componentTypesToEntity[componentType])
		{
			EntityComponentTypeHash hash = entityComponentHash(entity, componentType);
			components.insert(std::static_pointer_cast<T>(m_entityComponentTypeHashToComponent[hash]));
		}

		return components;
	}

private:
	Entity m_nextEntity = 0;

	EntityComponentTypeHash entityComponentHash(Entity e, ComponentType t)
	{
		return (e << 16) | t;
	}

	std::set<Entity> intersect(std::set<Entity> a, std::set<Entity> b)
	{
		std::set<Entity> intersect;
		std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
			std::inserter(intersect, intersect.begin()));
		return intersect;
	}

	/*
	entity&componentType: component
	entity&componentType: component
	*/
	std::unordered_map<Entity, std::set<ComponentType>> m_entityToComponentTypes;
	std::unordered_map<ComponentType, std::set<Entity>> m_componentTypesToEntity;
	std::unordered_map<EntityComponentTypeHash, std::shared_ptr<Component>> m_entityComponentTypeHashToComponent;
};

