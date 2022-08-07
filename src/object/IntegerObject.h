#ifndef _INTEGEROBJECT_H
#define _INTEGEROBJECT_H

#include "Object.h"

struct IntegerObject : Object {
    
    IntegerObject(const int val)
        : Value(val)
    {}

    std::string Inspect() const override {
        return std::to_string(Value);
    }
    
    std::string Type() const override {
        return ObjectType::INTEGER_OBJ;
    }
    int Value;
};

#endif
