//
// Created by gemcu on 8/30/26.
//

#include "system.h"

System::System(const SYSTEM_ID id) : _id{id} {}

void System::apply(ENTITY_ID entity) {}

System::~System() = default;
