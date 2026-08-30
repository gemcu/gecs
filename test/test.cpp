//
// Created by gemcu on 8/30/26.
//

#include "gecs.h"

class Value : public Component {
    int _value = 0;
public:
    Value() : Component("VALUE_COMPONENT") {}
    int getValue() const {return _value;}
    void setValue(const int value) {this->_value = value;}
};

class IncrementValue : public System {
    void apply() override {
        if (hasComponent("VALUE_COMPONENT")) {
            const auto valueComp = std::dynamic_pointer_cast<Value>(getComponent("VALUE_COMPONENT"));
            valueComp->setValue(valueComp->getValue() + 1);
        }
    }
public:
    IncrementValue() : System("INCREMENT_VALUE_SYSTEM") {}
};

int main() {

    auto comp = std::make_unique<Value>();

    GECS gecs{0};
    gecs.createEntity();
    gecs.attachComponent(std::move(comp));
    gecs.run();
}