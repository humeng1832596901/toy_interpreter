#ifndef _EXITOBJECT_H
#define _EXITOBJECT_H

#include "Object.h"
#include <memory>

struct ExitObject : Object {
    std::string Inspect() const override {
        return "exit";
    }
    std::string Type() const override {
        return ObjectType::EXIT_OBJ;
    }
};
auto exitObject = std::make_shared<ExitObject>();

#endif
