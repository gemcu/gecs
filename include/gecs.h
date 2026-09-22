//
// Created by gemcu on 8/30/26.
//

#ifndef GECS_GECS_H
#define GECS_GECS_H

#include <algorithm>
#include <memory>
#include <unordered_map>
#include <vector>
#include <stdexcept>

#include "types.h"
#include "entity.h"
#include "component.h"
#include "system.h"

class GECS {

    friend class Entity;
    friend class System;

    const GECS_ID _id;
    long _tickCount = 0;
    std::unordered_map<ENTITY_ID, Entity> _entity_map;
    std::unordered_map<COMPONENT_ID, std::shared_ptr<Component>> _component_map;
    std::unordered_map<SYSTEM_ID, std::shared_ptr<System>> _system_map;

    std::vector<SYSTEM_ID> _pipeline;

    ENTITY_ID _next_entity_id() const;
    COMPONENT_ID _next_component_id() const;
    SYSTEM_ID _next_system_id() const;

    template <class T> std::shared_ptr<T> get_component(ENTITY_ID id);
    template <class T> bool has_component(ENTITY_ID id) const;




public:
    explicit GECS(GECS_ID id);

    Entity createEntity();
    Entity getEntity(ENTITY_ID id);
    std::vector<Entity> getEntities() const;
    void destroyEntity(ENTITY_ID id);

    COMPONENT_ID addComponent(const std::shared_ptr<Component>& component);
    std::vector<COMPONENT_ID> addComponents(const std::vector<std::shared_ptr<Component>>& components);

    std::shared_ptr<Component> removeComponent(COMPONENT_ID id);
    std::vector<std::shared_ptr<Component>> removeComponents(const std::vector<COMPONENT_ID>& ids);

    SYSTEM_ID addSystem(std::shared_ptr<System> system);
    std::shared_ptr<System> removeSystem(SYSTEM_ID id);
    void setPipeline(const std::vector<SYSTEM_ID>& pipeline);

    void attachComponentToEntity(COMPONENT_ID compId, ENTITY_ID entityId) const;
    void attachComponentsToEntity(const std::vector<COMPONENT_ID>& compIds, ENTITY_ID entityId) const;
    void detachComponentFromEntity(COMPONENT_ID compId) const;
    void detachComponentsFromEntity(const std::vector<COMPONENT_ID> &compIds) const;

    COMPONENT_ID createAndAttachComponent(const std::shared_ptr<Component> &component, ENTITY_ID entity_id);
    std::vector<COMPONENT_ID> createAndAttachComponents(const std::vector<std::shared_ptr<Component>>& components, ENTITY_ID entityId);

    void tick();
};

inline ENTITY_ID GECS::_next_entity_id() const {
    ENTITY_ID id{0};
    for (const auto e : _entity_map)
        if (id <= e.first) id = e.first;
    return id + 1;
}

inline COMPONENT_ID GECS::_next_component_id() const {
    COMPONENT_ID id{0};
    for (const auto& c : _component_map)
        if (id <= c.first) id = c.first;
    return id + 1;
}

inline SYSTEM_ID GECS::_next_system_id() const {
    SYSTEM_ID id{0};
    for (const auto& s : _system_map)
        if (id <= s.first) id = s.first;
    return id + 1;
}

inline GECS::GECS(const GECS_ID id) : _id{id} {
}

inline Entity GECS::createEntity() {
    ENTITY_ID id = _next_entity_id();
    Entity entity = Entity{id, this};
    _entity_map.insert({id, entity});
    return entity;
}

inline Entity GECS::getEntity(ENTITY_ID id) {
    return _entity_map.at(id);
}

inline std::vector<Entity> GECS::getEntities() const {
    auto entities = std::vector<Entity>();
    for (auto it = _entity_map.cbegin(); it != _entity_map.cend(); ++it) {
        entities.push_back(it->second);
    }
    return entities;
}

inline void GECS::destroyEntity(const ENTITY_ID id) {
    _entity_map.erase(id);
}

inline COMPONENT_ID GECS::addComponent(const std::shared_ptr<Component>& component) {
    COMPONENT_ID id = _next_component_id();
    _component_map.insert({id, component});
    return id;
}

inline std::vector<COMPONENT_ID> GECS::addComponents(const std::vector<std::shared_ptr<Component>>& components) {
    std::vector<COMPONENT_ID> ids;
    for (const auto& comp: components) {
        ids.push_back(addComponent(comp));
    }
    return ids;
}

inline std::vector<std::shared_ptr<Component>> GECS::removeComponents(const std::vector<COMPONENT_ID>& ids) {
    std::vector<std::shared_ptr<Component>> components;
    for (const auto id : ids) {
        components.push_back(removeComponent(id));
    }
    return components;
}

inline void GECS::attachComponentsToEntity(const std::vector<COMPONENT_ID>& compIds, const ENTITY_ID entityId) const {
    for (const auto compId: compIds) {
        attachComponentToEntity(compId, entityId);
    }
}

inline void GECS::detachComponentsFromEntity(const std::vector<COMPONENT_ID> &compIds) const {
    for (const auto compId: compIds) {
        detachComponentFromEntity(compId);
    }
}

inline COMPONENT_ID GECS::createAndAttachComponent(const std::shared_ptr<Component> &component, const ENTITY_ID entity_id) {
    const auto compId = addComponent(component);
    attachComponentToEntity(compId, entity_id);
    return compId;
}

inline std::vector<COMPONENT_ID> GECS::createAndAttachComponents(
    const std::vector<std::shared_ptr<Component>>& components,
    const ENTITY_ID entityId
    ) {
    auto compIds = addComponents(components);
    attachComponentsToEntity(compIds, entityId);
    return compIds;
}

inline std::shared_ptr<Component> GECS::removeComponent(const COMPONENT_ID id) {
    auto component = std::move(_component_map.at(id));
    _component_map.erase(id);
    return component;
}

inline void GECS::attachComponentToEntity(const COMPONENT_ID compId, const ENTITY_ID entityId) const {
    try {
        const auto& comp = _component_map.at(compId);
        const auto& entity = _entity_map.at(entityId);
        comp->set_entity(entity.getId());
    } catch (const std::out_of_range&) {
        throw std::runtime_error("Component or entity not found");
    }
}

inline void GECS::detachComponentFromEntity(const COMPONENT_ID compId) const {
    try {
        const auto& comp = _component_map.at(compId);
        comp->set_entity(-1);
    } catch (const std::out_of_range&) {
        throw std::runtime_error("Component or entity not found");
    }
}

inline SYSTEM_ID GECS::addSystem(std::shared_ptr<System> system) {
    SYSTEM_ID id{1};
    for (const auto& s : _system_map)
        if (id <= s.first) id = s.first + 1;
    system->set_id(id);
    system->set_gecs(this);
    _system_map.insert({id, std::move(system)});
    return id;
}

inline std::shared_ptr<System> GECS::removeSystem(const SYSTEM_ID id) {
    auto s = _system_map.at(id);
    _system_map.erase(id);
    s->set_gecs(nullptr);
    return s;
}

inline void GECS::setPipeline(const std::vector<SYSTEM_ID>& pipeline) {
    try {

        for (auto sys : pipeline) {
            _system_map.at(sys);
        }
    } catch (const std::out_of_range&) {
        throw std::runtime_error("System not found");
    }
    _pipeline = pipeline;
}

inline void GECS::tick(){
    for (const auto s : _pipeline) {
        const auto& system = _system_map.at(s);
        system->start(getEntities());
        for (const auto e : _entity_map) {
            if (system->requirements(e.second))
                system->forEach(e.second);
        }
        system->finish(getEntities());
    }
    _tickCount++;
}

template<class T>
std::shared_ptr<T> GECS::get_component(const ENTITY_ID id) {
    for (const auto& c : _component_map) {
        if (c.second->_entity_id == id) {
            auto casted = std::dynamic_pointer_cast<T>(c.second);
            if (casted) return casted;
        }
    }
    throw std::runtime_error("No component for entity " + std::to_string(id));
}

template<class T>
bool GECS::has_component(const ENTITY_ID id) const {
    for (const auto& c : _component_map) {
        if (c.second->_entity_id == id && std::dynamic_pointer_cast<T>(c.second)) return true;
    }
    return false;
}

template<class T>
bool Entity::has() const {
    return _gecs->has_component<T>(this->_id);
}

template<class T>
std::shared_ptr<T> Entity::get() const {
    return _gecs->get_component<T>(this->_id);
}

template<class T>
COMPONENT_ID Entity::attach(std::shared_ptr<T> component) {
    if (has<T>()) {
        return -1;
    }
    return _gecs->createAndAttachComponent(component, _id);
}

template<class T>
std::shared_ptr<T> Entity::detach() {
    auto comp = get<T>();
    _gecs->detachComponentFromEntity(comp->getId());
    return _gecs->removeComponent(comp->getId());
}

#endif //GECS_GECS_H
