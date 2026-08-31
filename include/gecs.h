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
    friend class System;

    const GECS_ID _id;
    std::unordered_map<ENTITY_ID, Entity> _entity_map;
    std::unordered_map<COMPONENT_ID, std::shared_ptr<Component>> _component_map;
    std::unordered_map<SYSTEM_ID, std::shared_ptr<System>> _system_map;

    std::vector<SYSTEM_ID> _pipeline;

    template <class T> std::shared_ptr<T> get_component(ENTITY_ID id);
    template <class T> bool has_component(ENTITY_ID id) const;

public:
    explicit GECS(GECS_ID id);

    ENTITY_ID createEntity();
    ENTITY_ID destroyEntity(ENTITY_ID id);

    COMPONENT_ID addComponent(std::shared_ptr<Component> component);
    std::shared_ptr<Component> removeComponent(COMPONENT_ID id);
    SYSTEM_ID addSystem(std::shared_ptr<System> system);
    std::shared_ptr<System> removeSystem(SYSTEM_ID id);
    void setPipeline(const std::vector<SYSTEM_ID>& pipeline);

    void attachComponentToEntity(COMPONENT_ID comp_id, ENTITY_ID entity_id) const;
    void detachComponentFromEntity(COMPONENT_ID comp_id) const;

    void run() const;

    virtual ~GECS();
};

inline GECS::GECS(const GECS_ID id) : _id{id} {
}

inline ENTITY_ID GECS::createEntity() {
    ENTITY_ID id{1};
    for (const auto e : _entity_map)
        if (id <= e.first) id = e.first + 1;
    _entity_map.insert({id, Entity{id}});
    return id;
}

inline ENTITY_ID GECS::destroyEntity(const ENTITY_ID id) {
    _entity_map.erase(id);
    return id;
}

inline COMPONENT_ID GECS::addComponent(std::shared_ptr<Component> component) {
    COMPONENT_ID id{1};
    for (const auto& c : _component_map)
        if (id <= c.first) id = c.first + 1;
    component->set_id(id);
    _component_map.insert({id, std::move(component)});
    return id;
}

inline std::shared_ptr<Component> GECS::removeComponent(const COMPONENT_ID id) {
    auto component = std::move(_component_map.at(id));
    _component_map.erase(id);
    return component;
}

inline void GECS::attachComponentToEntity(const COMPONENT_ID comp_id, const ENTITY_ID entity_id) const {
    try {
        const auto& comp = _component_map.at(comp_id);
        const auto& entity = _entity_map.at(entity_id);
        comp->set_entity(entity.getId());
    } catch (const std::out_of_range&) {
        throw std::runtime_error("Component or entity not found");
    }
}

inline void GECS::detachComponentFromEntity(const COMPONENT_ID comp_id) const {
    try {
        const auto& comp = _component_map.at(comp_id);
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

inline void GECS::run() const{
    for (const auto s : _pipeline) {
        const auto& system = _system_map.at(s);
        for (const auto e : _entity_map) {
            system->apply_for(e.first);
        }
    }
}

inline GECS::~GECS() = default;

template<class T>
std::shared_ptr<T> GECS::get_component(const ENTITY_ID id) {
    for (const auto& c : _component_map) {
        if (c.second->get_entity() == id) {
            auto casted = std::dynamic_pointer_cast<T>(c.second);
            if (casted) return casted;
        }
    }
    throw std::runtime_error("No component for entity " + std::to_string(id));
}

template<class T>
bool GECS::has_component(const ENTITY_ID id) const {
    for (const auto& c : _component_map) {
        if (c.second->get_entity() == id && std::dynamic_pointer_cast<T>(c.second)) return true;
    }
    return false;
}

template<class T>
bool System::hasComponent() const {
    return _gecs->has_component<T>(_entity);
}

template<class T>
std::shared_ptr<T> System::getComponent() {
    return _gecs->get_component<T>(_entity);
}
#endif //GECS_GECS_H
