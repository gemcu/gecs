//
// Created by gemcu on 3/14/26.
//

#ifndef GECS_ENTITY_H
#define GECS_ENTITY_H

#include "types.h"

#include "component.h"

class GECS;

class Entity {
    friend class GECS;
    const ENTITY_ID _id;
    GECS* _gecs;
    explicit Entity(ENTITY_ID id, GECS* gecs);
public:
    ENTITY_ID getId() const;
    template <class T> bool has() const;
    template <class T> std::shared_ptr<T> get() const;
    template <class T> COMPONENT_ID attach(std::shared_ptr<T> component);
    template <class T> std::shared_ptr<T> detach();
};

inline Entity::Entity(const ENTITY_ID id, GECS* gecs) : _id{id}, _gecs(gecs) {}

inline ENTITY_ID Entity::getId() const {
    return _id;
}

#endif //GECS_ENTITY_H