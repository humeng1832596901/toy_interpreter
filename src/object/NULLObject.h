#ifndef _NULLOBJECT_H
#define _NULLOBJECT_H

#include "Object.h"

struct NULLObject : Object {
    std::string Inspect() const override {
        return "null";
    }  
    std::string Type() const override {
        return ObjectType::NULL_OBJ;
    }
};
auto nullObject = std::make_shared<NULLObject>();

#endif
