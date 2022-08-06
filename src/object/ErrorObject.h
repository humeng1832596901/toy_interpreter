/*******************************************************************
	> File Name: ErrorObject.h
	> Author: 胡孟
	> Mail: 13535324513@163.com
	> Created Time: Fri 05 Aug 2022 01:44:59 AM CST
 ******************************************************************/

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
