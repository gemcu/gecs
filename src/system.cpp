//
// Created by gemcu on 8/30/26.
//

#include "system.h"
#include "gecs.h"

void System::apply_for(ENTITY_ID entity) {
    _entity = entity;
    apply();
}

void System::set_id(const SYSTEM_ID id) {
    this->_id = id;
}

SYSTEM_ID System::get_id() const {
    return this->_id;
}

void System::set_gecs(GECS* gecs) {
    this->_gecs = gecs;
}

GECS* System::get_gecs() const {
    return _gecs;
}

System::System(SYSTEM_TYPE type) : _type(std::move(type)) {}

bool System::hasComponent(const COMPONENT_TYPE& type) {
    return _gecs->has_component_for_entity(type, _entity);
}

std::shared_ptr<Component> System::getComponent(const COMPONENT_TYPE& type) {
    return _gecs->get_component_for_entity(type, _entity);
}
