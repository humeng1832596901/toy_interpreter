#ifndef _ERROROBJECT_H
#define _ERROROBJECT_H

#include "Object.h"

struct ErrorObject : Object {

    ErrorObject(const std::string& err)
        : msg(err)
    {}

    std::string Inspect() const override {
        return "Error: " + msg;
    }

    std::string Type() const override {
        return ObjectType::ERROR_OBJ;
    }

    std::string msg;
};

#endif
