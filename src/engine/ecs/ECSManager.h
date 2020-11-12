#pragma once
#include <vector>
#include <unordered_map>
#include "Entity.h"
#include "Component.h"
#include "System.h"

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
		m_entityToComponentTypeToComponent.erase(entity);

		for (auto& compTypeToMapEntityComponent : m_componentTypeToEntityToComponent)
		{
			compTypeToMapEntityComponent.second.erase(entity);
		}
	}

	//Component management
	void addComponent(Entity entity, std::shared_ptr<Component> component)
	{
		if (!component)
		{
			return;
		}

		ComponentType componentType = ComponentType(typeid(*component).hash_code());
		auto it = m_entityToComponentTypeToComponent.find(entity);
		if (it != m_entityToComponentTypeToComponent.end())  // if entity already has components
		{
			it->second[componentType] = component;
		}
		else
		{
			m_entityToComponentTypeToComponent[entity] = { {componentType, component} };
		}

		auto it2 = m_componentTypeToEntityToComponent.find(componentType);
		if (it2 != m_componentTypeToEntityToComponent.end())  // if componentType is already present in entities
		{
			it2->second[entity] = component;
		}
		else
		{
			m_componentTypeToEntityToComponent[componentType] = { {entity, component} };
		}
	}

	template <typename T>
	void removeComponentOfType(Entity entity)
	{
		static ComponentType componentType = GetComponentType<T>();

		auto it = m_entityToComponentTypeToComponent.find(entity);
		if (it != m_entityToComponentTypeToComponent.end())
		{
			it->second.erase(componentType);
		}

		auto it2 = m_componentTypeToEntityToComponent.find(componentType);
		if (it2 != m_componentTypeToEntityToComponent.end())
		{
			it2->second.erase(entity);
		}
	}

	template <typename T>
	std::shared_ptr<T> getComponentOfType(Entity entity)
	{
		static ComponentType componentType = GetComponentType<T>();
		auto it = m_entityToComponentTypeToComponent.find(entity);
		if (it != m_entityToComponentTypeToComponent.end())
		{
			auto it2 = it->second.find(componentType);
			if (it2 != it->second.end())
			{
				return std::static_pointer_cast<T>(it2->second);
			}
		}
		return nullptr;
	}

	template <typename T>
	std::vector<Entity> getAllEntitiesWithComponentType()
	{
		static ComponentType componentType = GetComponentType<T>();
		std::vector<Entity> entities = {};

		for (auto const& entityToComponent : m_componentTypeToEntityToComponent[componentType])
		{
			entities.push_back(entityToComponent.first);
		}

		return entities;
	}

	template <typename T>
	std::vector<std::shared_ptr<T>> getAllComponentsOfType()
	{
		static ComponentType componentType = GetComponentType<T>();
		std::vector<std::shared_ptr<T>> components = {};

		for (auto const& entityToComponent : m_componentTypeToEntityToComponent[componentType])
		{
			components.push_back(std::static_pointer_cast<T>(entityToComponent.second));
		}

		return components;
	}

private:
	Entity m_nextEntity = 0;

	/*
	entity:
		componentType: component
		componentType: component
	entity:
		componentType: component
		componentType: component
	*/
	std::unordered_map<Entity, std::unordered_map<ComponentType, std::shared_ptr<Component>>> m_entityToComponentTypeToComponent;

	/*
	componentType:
		entity: component
		entity: component
	componentType:
		entity: component
		entity: component
	*/
	std::unordered_map<ComponentType, std::unordered_map<Entity, std::shared_ptr<Component>>> m_componentTypeToEntityToComponent;
};

