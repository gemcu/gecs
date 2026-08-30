//
// Created by gemcu on 8/30/26.
//

#include "gecs.h"
#include <stdexcept>

GECS::GECS(const GECS_ID id) : _id{id} {
}

ENTITY_ID GECS::createEntity() {
    ENTITY_ID id{1};
    for (const auto e : _entity_map)
        if (id <= e.first) id = e.first + 1;
    _entity_map.insert({id, Entity{id}});
    return id;
}

ENTITY_ID GECS::destroyEntity(const ENTITY_ID id) {
    _entity_map.erase(id);
    return id;
}

COMPONENT_ID GECS::attachComponent(std::shared_ptr<Component> component) {
    COMPONENT_ID id{1};
    for (const auto& c : _component_map)
        if (id <= c.first) id = c.first + 1;
    component->set_id(id);
    _component_map.insert({id, std::move(component)});
    return id;
}

std::shared_ptr<Component> GECS::detachComponent(const COMPONENT_ID id) {
    auto component = std::move(_component_map.at(id));
    _component_map.erase(id);
    return component;
}

SYSTEM_ID GECS::attachSystem(std::shared_ptr<System> system) {
    SYSTEM_ID id{1};
    for (const auto& s : _system_map)
        if (id <= s.first) id = s.first + 1;
    system->set_id(id);
    _system_map.insert({id, std::move(system)});
    return id;
}

std::shared_ptr<System> GECS::detachSystem(const SYSTEM_ID id) {
    auto s = _system_map.at(id);
    _system_map.erase(id);
    return s;
}

void GECS::run() {
    for (const auto s : _pipeline) {
        const auto& system = _system_map.at(s);
        for (const auto e : _entity_map) {
            system->apply_for(e.first);
        }
    }
}

std::shared_ptr<Component> GECS::get_component_for_entity(const COMPONENT_TYPE& type, const ENTITY_ID id) {
    for (const auto& c : _component_map) {
        if (c.second->_type == type && c.second->get_entity() == id) return c.second;
    }
    throw std::runtime_error("No component " + type + " for entity " + std::to_string(id));
}

bool GECS::has_component_for_entity(const COMPONENT_TYPE& type, const ENTITY_ID id) const {
    for (const auto& c : _component_map) {
        if (c.second->_type == type && c.second->get_entity() == id) return true;
    }
    return false;
}


template<class T>
std::shared_ptr<T> GECS::get_component(ENTITY_ID id) {
    for (const auto& c : _component_map) {
        if (std::is_same<std::shared_ptr<T>, decltype(c.second)>::value && c.second->get_entity() == id)
            return std::dynamic_pointer_cast<T>(c.second);
    }
    throw std::runtime_error("No component for entity " + std::to_string(id));
}

template<class T>
bool GECS::has_component(ENTITY_ID id) const {
    for (const auto& c : _component_map) {
        if (std::is_same<std::shared_ptr<T>, decltype(c.second)>::value && c.second->get_entity() == id)
            return true;
    }
    return false;
}

GECS::~GECS() = default;
