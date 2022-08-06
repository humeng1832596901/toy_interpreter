/*******************************************************************
	> File Name: IntegerObject.h
	> Author: 胡孟
	> Mail: 13535324513@163.com
	> Created Time: Fri 05 Aug 2022 01:47:17 AM CST
 ******************************************************************/

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
