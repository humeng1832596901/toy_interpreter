#ifndef _BUILTINOBJECT_H
#define _BUILTINOBJECT_H

#include "Object.h"
#include <functional>

struct BuiltinObject : Object {
    using BuiltinFunction = std::function<std::shared_ptr<Object>(const std::vector<std::shared_ptr<Object>>&)>;
    
    BuiltinObject(const BuiltinFunction& func)
        : fn(func)
    {}

    std::string Inspect() const override {
        return "builtin function";
    }

    std::string Type() const override {
        return ObjectType::BUILTIN_OBJ;
    }
    
    BuiltinFunction fn;
};


#endif
