/*******************************************************************
	> File Name: ArrayObject.h
	> Author: 胡孟
	> Mail: 13535324513@163.com
	> Created Time: Fri 05 Aug 2022 01:58:50 AM CST
 ******************************************************************/

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
