//
// Created by gemcu on 3/14/26.
//

#ifndef GECS_COMPONENT_H
#define GECS_COMPONENT_H

#include "types.h"

class GECS;

class Component {
    friend class GECS;
    COMPONENT_ID _id;
    GECS* _gecs;
    explicit Component(COMPONENT_ID id, GECS* gecs);

    ENTITY_ID _entity_id = -1;
    void set_entity(ENTITY_ID entity_id);
    COMPONENT_ID get_id() const;
    ENTITY_ID get_entity() const;
protected:
public:
    Component();
    [[nodiscard]] COMPONENT_ID getId() const;
    virtual ~Component() = default;
};

inline Component::Component() {}

inline Component::Component(const COMPONENT_ID id, GECS* gecs) : _id{id}, _gecs{gecs} {}

inline COMPONENT_ID Component::get_id() const {
    return _id;
}

inline void Component::set_entity(const ENTITY_ID entity_id) {
    this->_entity_id = entity_id;
}

#endif //GECS_COMPONENT_H