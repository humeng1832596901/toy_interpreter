/*******************************************************************
	> File Name: ExitObject.h
	> Author: 胡孟
	> Mail: 13535324513@163.com
	> Created Time: Fri 05 Aug 2022 02:00:54 AM CST
 ******************************************************************/

#ifndef _EXITOBJECT_H
#define _EXITOBJECT_H

#include "Object.h"
#include <memory>

struct ExitObject : Object {
    std::string Inspect() const override {
        return "exit";
    }
    std::string Type() const override {
        return ObjectType::EXIT_OBJ;
    }
};
auto exitObject = std::make_shared<ExitObject>();

#endif
