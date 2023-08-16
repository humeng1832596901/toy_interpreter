#ifndef _STRINGOBJECT_H
#define _STRINGOBJECT_H

#include "Object.h"

struct StringObject : Object {
    
    StringObject(const std::string str)
        : Value(str)
    {}

    std::string Inspect() const override {
        return "\""+Value+"\"";
    }

    std::string Type() const override {
        return ObjectType::STRING_OBJ;
    }

    std::string Value;
};

#endif
