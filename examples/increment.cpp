//
// Created by gemcu on 8/30/26.
//

#include <iostream>

#include "gecs.h"

namespace {
    class Value : public Component {
        int _value = 0;
    public:
        int getValue() const {return _value;}
        void setValue(const int value) {this->_value = value;}
    };

    class IncrementValue : public System {

        int iteration_count = 0;

        void pre() override {
            iteration_count++;
            std::cout << "iteration " << iteration_count << " begin...\n";
        }
        void apply() override {
            if (hasComponent<Value>()) {
                const auto valueComp = getComponent<Value>();
                valueComp->setValue(valueComp->getValue() + 1);
                std::cout << "Incremented value: " << valueComp->getValue() << "\n";
            }
        }
        void post() override {
            std::cout << "iteration " << iteration_count << " end.\n";
        }
    };

    void createIncrementable(GECS& gecs, const int value = 0) {
        auto comp = std::make_shared<Value>();
        comp->setValue(value);
        auto sys = std::make_shared<IncrementValue>();

        const ENTITY_ID entity = gecs.createEntity();
        gecs.createAndAttachComponent(comp, entity);
    }
}

int main() {

    GECS gecs{0};
    const auto sys = std::make_shared<IncrementValue>();
    const SYSTEM_ID system_id = gecs.addSystem(sys);
    gecs.setPipeline({system_id});

    for (int i=0; i<5; i++) createIncrementable(gecs, (i*5)-10);
    for (int i=0; i<10; i++)
        gecs.run();
}