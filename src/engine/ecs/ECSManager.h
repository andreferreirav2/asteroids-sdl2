#pragma once
#include <vector>
#include <map>
#include "Entity.h"
#include "Component.h"
#include "System.h"

class ECSManager
{
public:
	//Entity management
	Entity createEntity();
	void destroyEntity(Entity);

	//Component management
	void addComponent(Entity, std::shared_ptr<Component> Component);
	void removeComponentOfType(Entity, ComponentType);
	std::shared_ptr<Component> getComponentOfType(Entity, ComponentType);
	std::vector<Entity> getAllEntitiesWithComponentType(ComponentType);
	std::vector<std::shared_ptr<Component>> getAllComponentsOfType(ComponentType);

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
	std::map<Entity, std::map<ComponentType, std::shared_ptr<Component>>> m_entityToComponentTypeToComponent;

	/*
	componentType:
		entity: component
		entity: component
	componentType:
		entity: component
		entity: component
	*/
	std::map<ComponentType, std::map<Entity, std::shared_ptr<Component>>> m_componentTypeToEntityToComponent;
};

