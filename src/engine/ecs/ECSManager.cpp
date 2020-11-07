#include "ECSManager.h"

using namespace std;

Entity ECSManager::createEntity()
{
	return m_nextEntity++;
}

void ECSManager::destroyEntity(Entity entity)
{
	m_entityToComponentTypeToComponent.erase(entity);

	for (auto& compTypeToMapEntityComponent : m_componentTypeToEntityToComponent)
	{
		compTypeToMapEntityComponent.second.erase(entity);
	}
}

void ECSManager::addComponent(Entity entity, shared_ptr<Component> component)
{
	if (!component)
	{
		return;
	}

	ComponentType componentType = typeid(*component).name();
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

void ECSManager::removeComponentOfType(Entity entity, ComponentType componentType)
{
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

shared_ptr<Component> ECSManager::getComponentOfType(Entity entity, ComponentType componentType)
{
	auto it = m_entityToComponentTypeToComponent.find(entity);
	if (it != m_entityToComponentTypeToComponent.end())
	{
		auto it2 = it->second.find(componentType);
		if (it2 != it->second.end())
		{
			return it2->second;
		}
	}
	return nullptr;
}

vector<Entity> ECSManager::getAllEntitiesWithComponentType(ComponentType componentType)
{
	vector<Entity> entities = {};

	for (auto const& entityToComponent : m_componentTypeToEntityToComponent[componentType])
	{
		entities.push_back(entityToComponent.first);
	}

	return entities;
}

vector<shared_ptr<Component>> ECSManager::getAllComponentsOfType(ComponentType componentType)
{
	vector<shared_ptr<Component>> components = {};

	for (auto const& entityToComponent : m_componentTypeToEntityToComponent[componentType])
	{
		components.push_back(entityToComponent.second);
	}

	return components;
}
