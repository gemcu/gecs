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
    SYSTEM_ID _id = -1;
    GECS* _gecs;
    System(SYSTEM_ID id, GECS* gecs);
    SYSTEM_ID get_id() const;
    GECS* get_gecs() const;
    void set_id(SYSTEM_ID id);
    void set_gecs(GECS* gecs);
    ENTITY_ID _entity = -1;

    virtual void start(const std::vector<Entity>& entities);
    virtual void finish(const std::vector<Entity>& entities);
    virtual void forEach(const Entity& entity);

    virtual bool requirements(const Entity& entity);

    void apply_for(ENTITY_ID entity);
    bool requirements_for(ENTITY_ID entity);

protected:
    System();
public:
    virtual ~System() = default;
    SYSTEM_ID getId() const;
};

inline System::System(const SYSTEM_ID id, GECS* gecs) : _id{id}, _gecs{gecs} {}

inline void System::start(const std::vector<Entity>& entities) {}

inline void System::finish(const std::vector<Entity>& entities) {}

inline void System::forEach(const Entity& entity) {}

inline void System::set_gecs(GECS *gecs) {
    this->_gecs = gecs;
}

inline void System::set_id(const SYSTEM_ID id) {
    this->_id = id;
}

inline bool System::requirements(const Entity& entity) {
    return true;
}

inline SYSTEM_ID System::getId() const {
    return this->_id;
}

inline System::System() : _gecs{nullptr} {}
#endif //GECS_SYSTEM_H
