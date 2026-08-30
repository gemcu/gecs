//
// Created by gemcu on 8/30/26.
//
#include "component.h"

#include <utility>

void Component::set_id(const COMPONENT_ID id) {
    this->_id = id;
}

void Component::set_entity(const ENTITY_ID entity_id) {
    this->_entity_id = entity_id;
}

COMPONENT_ID Component::get_id() const {
    return _id;
}

ENTITY_ID Component::get_entity() const {
    return _entity_id;
}

Component::Component(COMPONENT_TYPE type) : _type(std::move(type)) {}