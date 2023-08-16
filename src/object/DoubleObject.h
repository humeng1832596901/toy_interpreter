#ifndef _DOUBLEOBJECT_H
#define _DOUBLEOBJECT_H

#include "Object.h"

struct DoubleObject : Object {
    
    DoubleObject(const double val)
        : Value(val)
    {}

    std::string Inspect() const override {
        return std::to_string(Value);
    }
    
    std::string Type() const override {
        return ObjectType::DOUBLE_OBJ;
    }
    double Value;
};

#endif
