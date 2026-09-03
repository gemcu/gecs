//
// Created by gemcu on 8/30/26.
//

#include <iostream>

#include "gecs.h"

class Value : public Component {
    int _value = 0;
public:
    int getValue() const {return _value;}
    void setValue(const int value) {this->_value = value;}
};

class IncrementValue : public System {
    void pre() override {
        std::cout << "Begin increment";
    }
    void apply() override {
        if (hasComponent<Value>()) {
            const auto valueComp = getComponent<Value>();
            valueComp->setValue(valueComp->getValue() + 1);
            std::cout << "Incremented value: " << valueComp->getValue() << std::endl;
        }
    }
    void post() override {
        std::cout << "End increment";
    }
};

int main() {

    auto comp = std::make_shared<Value>();
    auto sys = std::make_shared<IncrementValue>();

    GECS gecs{0};
    const ENTITY_ID entity = gecs.createEntity();
    const COMPONENT_ID comp_id = gecs.addComponent(std::move(comp));
    const SYSTEM_ID system_id = gecs.addSystem(std::move(sys));
    gecs.attachComponentToEntity(comp_id, entity);
    gecs.setPipeline({system_id});

    for (int i=0; i<10; i++) gecs.run();
}