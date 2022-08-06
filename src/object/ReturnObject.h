/*******************************************************************
	> File Name: ReturnObject.h
	> Author: 胡孟
	> Mail: 13535324513@163.com
	> Created Time: Fri 05 Aug 2022 01:54:52 AM CST
 ******************************************************************/

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
