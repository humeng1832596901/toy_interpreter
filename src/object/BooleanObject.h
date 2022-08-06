/*******************************************************************
	> File Name: BooleanObject.h
	> Author: 胡孟
	> Mail: 13535324513@163.com
	> Created Time: Fri 05 Aug 2022 02:02:01 AM CST
 ******************************************************************/

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
