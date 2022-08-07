#ifndef _ARRAYOBJECT_H
#define _ARRAYOBJECT_H

#include "Object.h"
#include <vector>
#include <memory>

struct ArrayObject : Object {

    ArrayObject(const std::vector<std::shared_ptr<Object>>& elements)
        : Elements(elements)
    {}

    ArrayObject(const std::vector<std::shared_ptr<Object>>&& elements)
        : Elements(elements)
    {}

    std::string Type() const override {
        return ObjectType::ARRAY_OBJ;
    }

    std::string Inspect() const override {
        std::string str = "[";
        int size = Elements.size();
        for(int i=0; i<size; ++i){
            str += Elements[i]->Inspect();
            if(i != size-1)
                str += ", ";
        }
        return str+"]";
    }

    std::vector<std::shared_ptr<Object>> Elements;  
};

#endif
