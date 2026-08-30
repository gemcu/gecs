//
// Created by gemcu on 8/30/26.
//

#ifndef GECS_SYSTEM_H
#define GECS_SYSTEM_H

#include "entity.h"
#include "types.h"

class System {
    friend class GECS;
    const SYSTEM_ID _id;
    virtual void apply(ENTITY_ID entity);
public:
    explicit System(SYSTEM_ID id);
    virtual ~System();
};
#endif //GECS_SYSTEM_H
