//
// Created by gemcu on 8/30/26.
//

#ifndef GECS_SYSTEM_H
#define GECS_SYSTEM_H

#include <memory>

#include "component.h"
#include "types.h"

class GECS;

class System {
    friend class GECS;

    SYSTEM_ID _id   = -1;
    SYSTEM_TYPE _type;

    ENTITY_ID _entity;
    virtual void apply() = 0;
    void apply_for(ENTITY_ID entity);

    void set_id(SYSTEM_ID id);
    SYSTEM_ID get_id() const;

    GECS* _gecs;
    void set_gecs(GECS* gecs);
    GECS* get_gecs() const;

protected:
    explicit System(SYSTEM_TYPE type);
    std::shared_ptr<Component> getComponent(const COMPONENT_TYPE& type);
    bool hasComponent(const COMPONENT_TYPE& type);

public:
    virtual ~System() = default;
};
#endif //GECS_SYSTEM_H
