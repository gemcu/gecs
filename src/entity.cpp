//
// Created by gemcu on 3/14/26.
//
#include "../include/entity.h"

Entity::Entity(const ENTITY_ID id) : id{id} {}

ENTITY_ID Entity::getId() const {
    return id;
}
