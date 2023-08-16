#ifndef _BOOLEANOBJECT_H
#define _BOOLEANOBJECT_H

#include "Object.h"

struct BooleanObject : Object {
    
    BooleanObject(const bool val)
        : Value(val)
    {}

    std::string Inspect() const override {
        return Value ? "true" : "false";
    }
    std::string Type() const override {
        return ObjectType::BOOLEAN_OBJ;
    }
    bool Value;
};
auto trueObject = std::make_shared<BooleanObject>(true);
auto falseObject = std::make_shared<BooleanObject>(false);


#endif
