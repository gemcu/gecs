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

#endif //GECS_ENTITY_H