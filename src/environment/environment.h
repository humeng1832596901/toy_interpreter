/*******************************************************************
	> File Name: environment.h
	> Author: 胡孟
	> Mail: 13535324513@163.com
	> Created Time: Sun 31 Jul 2022 09:15:57 PM CST
 ******************************************************************/

#ifndef _ENVIRONMENT_H
#define _ENVIRONMENT_H

#include "../object/ErrorObject.h"
#include <vector>
#include <unordered_map>
#include <memory>

struct Environment{

    Environment(Environment* env=nullptr)
        :outer(env)
    {}

    std::shared_ptr<Object> Get(const std::string& key){
        if(store.find(key) == store.end()){
            if(outer)
                return outer->Get(key);
            return std::make_shared<ErrorObject>("unknow indent "+key);
        }
        return store[key];
    }

    std::shared_ptr<Object> Set(const std::string& key, std::shared_ptr<Object>&& val){
        store[key] = val;
        return val;
    }

    std::unordered_map<std::string, std::shared_ptr<Object>> store;  
    Environment* outer = nullptr;  //外层环境
};

Environment* NewEnclosedEnvironment(Environment* outer){
    return new Environment(outer);
}

#endif
