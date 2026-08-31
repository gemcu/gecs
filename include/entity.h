//
// Created by gemcu on 3/14/26.
//

#ifndef GECS_ENTITY_H
#define GECS_ENTITY_H

#include "types.h"

class Entity {
    const ENTITY_ID id  = -1;
public:
    explicit Entity(ENTITY_ID id);
    ENTITY_ID getId() const;
};

inline Entity::Entity(const ENTITY_ID id) : id{id} {}

inline ENTITY_ID Entity::getId() const {
    return id;
}

#endif //GECS_ENTITY_H