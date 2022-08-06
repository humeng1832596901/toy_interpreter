/*******************************************************************
	> File Name: NULLObject.h
	> Author: 胡孟
	> Mail: 13535324513@163.com
	> Created Time: Fri 05 Aug 2022 02:03:58 AM CST
 ******************************************************************/

#ifndef _NULLOBJECT_H
#define _NULLOBJECT_H

#include "Object.h"

struct NULLObject : Object {
    std::string Inspect() const override {
        return "null";
    }  
    std::string Type() const override {
        return ObjectType::NULL_OBJ;
    }
};
auto nullObject = std::make_shared<NULLObject>();

#endif
