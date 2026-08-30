//
// Created by gemcu on 3/14/26.
//

#ifndef GECS_COMPONENT_H
#define GECS_COMPONENT_H

#include "entity.h"

class Component {
    friend class GECS;

    COMPONENT_ID _id        = -1;
    COMPONENT_TYPE _type;

    ENTITY_ID _entity_id    = -1;

    void set_id(COMPONENT_ID id);
    void set_entity(ENTITY_ID entity_id);
    COMPONENT_ID get_id() const;
    ENTITY_ID get_entity() const;

protected:
    explicit Component(COMPONENT_TYPE type);

public:
    virtual ~Component() = default;
};

#endif //GECS_COMPONENT_H