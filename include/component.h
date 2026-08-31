//
// Created by gemcu on 3/14/26.
//

#ifndef GECS_COMPONENT_H
#define GECS_COMPONENT_H

class Component {
    friend class GECS;

    COMPONENT_ID _id        = -1;
    ENTITY_ID _entity_id    = -1;

    void set_id(COMPONENT_ID id);
    void set_entity(ENTITY_ID entity_id);
    COMPONENT_ID get_id() const;
    ENTITY_ID get_entity() const;

protected:
    explicit Component();

public:
    virtual ~Component() = default;
};


inline void Component::set_id(const COMPONENT_ID id) {
    this->_id = id;
}

inline void Component::set_entity(const ENTITY_ID entity_id) {
    this->_entity_id = entity_id;
}

inline COMPONENT_ID Component::get_id() const {
    return _id;
}

inline ENTITY_ID Component::get_entity() const {
    return _entity_id;
}

inline Component::Component() {}

#endif //GECS_COMPONENT_H