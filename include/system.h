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
    ENTITY_ID _entity = -1;
    std::shared_ptr<GECS> _gecs;

    virtual void pre();
    virtual void post();
    virtual void apply();
    void apply_for(ENTITY_ID entity);

    void set_id(SYSTEM_ID id);
    void set_gecs(const std::shared_ptr<GECS> &gecs);

    SYSTEM_ID get_id() const;
    std::shared_ptr<GECS> get_gecs() const;

protected:
    explicit System();
    template <class T> bool hasComponent() const;
    template <class T> std::shared_ptr<T> getComponent();

public:
    virtual ~System() = default;
};

inline void System::pre() {}

inline void System::post() {}

inline void System::apply() {}

inline void System::apply_for(const ENTITY_ID entity) {
    _entity = entity;
    apply();
}

inline void System::set_id(const SYSTEM_ID id) {
    this->_id = id;
}

inline SYSTEM_ID System::get_id() const {
    return this->_id;
}

inline void System::set_gecs(const std::shared_ptr<GECS> &gecs) {
    this->_gecs = gecs;
}

inline std::shared_ptr<GECS> System::get_gecs() const {
    return _gecs;
}

inline System::System() : _gecs{nullptr} {}
#endif //GECS_SYSTEM_H
