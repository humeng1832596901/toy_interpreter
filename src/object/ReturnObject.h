#ifndef _RETURNOBJECT_H
#define _RETURNOBJECT_H

#include "Object.h"
#include <memory>

struct ReturnObject : Object {

    ReturnObject(std::shared_ptr<Object>&& obj)
        : Value(obj)
    {}

    ReturnObject(std::shared_ptr<Object>& obj)
        : Value(obj)
    {}

    std::string Inspect() const override {
        return Value->Inspect();
    }

    std::string Type() const override {
        return ObjectType::RETURN_VALUE_OBJ;
    }
    std::shared_ptr<Object> Value;  
};

#endif
