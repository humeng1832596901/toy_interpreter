/*******************************************************************
	> File Name: obj.h
	> Author: 胡孟
	> Mail: 13535324513@163.com
	> Created Time: Sat 30 Jul 2022 07:15:45 PM CST
 ******************************************************************/

#ifndef _OBJ_H
#define _OBJ_H

#include <string>

namespace ObjectType{
    const std::string INTEGER_OBJ = "INTEGER";
    const std::string DOUBLE_OBJ = "DOUBLE";
    const std::string RETURN_VALUE_OBJ = "RETURN_VALUE";
    const std::string BOOLEAN_OBJ = "BOOLEAN";
    const std::string NULL_OBJ = "NULL";
    const std::string ERROR_OBJ = "ERROR";
    const std::string FUNCTION_OBJ = "FUNCTION";
    const std::string STRING_OBJ = "STRING";
    const std::string BUILTIN_OBJ = "BUILTIN";
    const std::string EXIT_OBJ = "EXIT";
    const std::string ARRAY_OBJ = "ARRAY";
    const std::string HASHKEY_OBJ = "HASHKEY";
    const std::string HASH_OBJ = "HASH";
    const std::string BREAK_OBJ = "BREAK";
}

struct Object{
    virtual std::string Type() const = 0;  
    virtual std::string Inspect() const = 0;
};

#endif
