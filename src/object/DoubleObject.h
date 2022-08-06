/*******************************************************************
	> File Name: DoubleObject.h
	> Author: 胡孟
	> Mail: 13535324513@163.com
	> Created Time: Fri 05 Aug 2022 01:50:33 AM CST
 ******************************************************************/

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
