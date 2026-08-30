//
// Created by gemcu on 8/30/26.
//

#ifndef GECS_GECS_H
#define GECS_GECS_H

#include <memory>
#include <unordered_map>
#include <vector>

#include "types.h"
#include "entity.h"
#include "component.h"
#include "system.h"

class GECS {
    friend class System;

    const GECS_ID _id;
    std::unordered_map<ENTITY_ID, Entity> _entity_map;
    std::unordered_map<COMPONENT_ID, std::shared_ptr<Component>> _component_map;
    std::unordered_map<SYSTEM_ID, std::shared_ptr<System>> _system_map;

    std::vector<SYSTEM_ID> _pipeline;

    std::shared_ptr<Component> get_component_for_entity(const COMPONENT_TYPE& type, ENTITY_ID id);
    bool has_component_for_entity(const COMPONENT_TYPE& type, const ENTITY_ID id) const;
    template <class T> std::shared_ptr<T> get_component(ENTITY_ID id);
    template <class T> bool has_component(ENTITY_ID id) const;

public:
    explicit GECS(GECS_ID id);

    ENTITY_ID createEntity();
    ENTITY_ID destroyEntity(ENTITY_ID id);

    COMPONENT_ID attachComponent(std::shared_ptr<Component> component);
    std::shared_ptr<Component> detachComponent(COMPONENT_ID id);

    SYSTEM_ID attachSystem(std::shared_ptr<System> system);
    std::shared_ptr<System> detachSystem(SYSTEM_ID id);

    void run();

    virtual ~GECS();
};
#endif //GECS_GECS_H
