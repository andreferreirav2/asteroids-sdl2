#pragma once
#include <set>
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

	template <typename T>
	std::set<std::tuple<Entity, std::shared_ptr<T>>> getAllEntitiesWithComponentTypes()
	{
		static ComponentType componentType = GetComponentType<T>();

		auto entities = m_componentTypesToEntity[componentType];

		std::set<std::tuple<Entity, std::shared_ptr<T>>> setTuples = {};
		for (auto ent : entities)
		{
			setTuples.insert(std::make_tuple<Entity, std::shared_ptr<T>>(std::move(ent), getComponentOfType<T>(ent)));
		}
		return setTuples;
	}

	template <typename T, typename U>
	std::vector<std::tuple<Entity, std::shared_ptr<T>, std::shared_ptr<U>>>& getAllEntitiesWithComponentTypes()
	{
		static ComponentType componentTypeT = GetComponentType<T>();
		static ComponentType componentTypeU = GetComponentType<U>();

		auto entities = m_componentTypesToEntity[componentTypeT];
		auto entitiesU = m_componentTypesToEntity[componentTypeU];
		entities.insert(entitiesU.begin(), entitiesU.end());

		auto tuples = std::vector<std::tuple<Entity, std::shared_ptr<T>, std::shared_ptr<U>>>(entities.size());
		int i = 0;
		for (auto ent : entities)
		{
			EntityComponentTypeHash hashT = entityComponentHash(ent, componentTypeT);
			EntityComponentTypeHash hashU = entityComponentHash(ent, componentTypeU);
			tuples[i++] = std::make_tuple<Entity, std::shared_ptr<T>, std::shared_ptr<U>>(
				std::move(ent),
				std::static_pointer_cast<T>(m_entityComponentTypeHashToComponent[hashT]),
				std::static_pointer_cast<U>(m_entityComponentTypeHashToComponent[hashU]));
		}
		return tuples;
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

	/*
	entity&componentType: component
	entity&componentType: component
	*/
	std::unordered_map<Entity, std::set<ComponentType>> m_entityToComponentTypes;
	std::unordered_map<ComponentType, std::set<Entity>> m_componentTypesToEntity;
	std::unordered_map<EntityComponentTypeHash, std::shared_ptr<Component>> m_entityComponentTypeHashToComponent;
};

