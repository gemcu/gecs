//
// Created by gemcu on 8/30/26.
//

#ifndef GECS_GECS_H
#define GECS_GECS_H

#include <memory>
#include <unordered_map>
#include <vector>

#include "entity.h"
#include "component.h"
#include "system.h"

class GECS {

    const GECS_ID _id;
    std::unordered_map<ENTITY_ID, Entity> _entity_map;
    std::unordered_map<COMPONENT_ID, std::unique_ptr<Component>> _component_map;
    std::unordered_map<SYSTEM_ID, std::unique_ptr<System>> _system_map;

    std::vector<SYSTEM_ID> _pipeline;
public:
    explicit GECS(GECS_ID id);

    ENTITY_ID createEntity();
    ENTITY_ID destroyEntity(ENTITY_ID id);

    COMPONENT_ID attachComponent(std::unique_ptr<Component> component);
    std::unique_ptr<Component> detachComponent(COMPONENT_ID id);

    SYSTEM_ID attachSystem(std::unique_ptr<System> system);
    std::unique_ptr<System> detachSystem(SYSTEM_ID id);

    virtual ~GECS();
};

#endif //GECS_GECS_H
