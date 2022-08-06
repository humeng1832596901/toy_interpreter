/*******************************************************************
	> File Name: StringObject.h
	> Author: 胡孟
	> Mail: 13535324513@163.com
	> Created Time: Fri 05 Aug 2022 01:51:33 AM CST
 ******************************************************************/

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
