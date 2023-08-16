#ifndef _FUNCTIONOBJECT_H
#define _FUNCTIONOBJECT_H

#include "Object.h"
#include "../environment/environment.h"

struct FunctionObject : Object {

    ~FunctionObject(){
        if(Body){
            delete Body;
            Body = nullptr;
        }
        int size = Parameters.size();
        for(int i=0; i<size; ++i){
            delete Parameters[i];
            Parameters[i] = nullptr;
        }
        Parameters.clear();
    }

    FunctionObject(Statement* body, const std::vector<Identifier*>& parameters, Environment* env)
        : Env(env)
    {
        int size = parameters.size();
        for(int i=0; i<size; ++i){
            Parameters.push_back(new Identifier(*parameters[i]));
        }
        Body = new BlockStatement(*dynamic_cast<BlockStatement*>(body));
    }

    std::string Inspect() const override {
        std::string str = "fn(";
        int size = Parameters.size();
        for(int i=0; i<size; ++i){
            str += Parameters[i]->String();
            if(i != size-1)
                str += ", ";
        }
        str += ")\n"+Body->String()+"\n}";
        return str;
    }

    std::string Type() const override {
        return ObjectType::FUNCTION_OBJ;
    } 

    Statement* Body;   //BlockStatement
    std::vector<Identifier*> Parameters;
    Environment* Env;
};

#endif
