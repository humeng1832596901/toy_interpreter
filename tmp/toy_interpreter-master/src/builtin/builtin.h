#ifndef _BUILITIN_H
#define _BUILITIN_H

//内置函数或内置变量
#include <ctime>
#include <string>
#include <chrono>
#include <memory>
#include "../object/Object.h"
#include "../object/BuiltinObject.h"

std::unordered_map<std::string, std::shared_ptr<BuiltinObject>> builtins;

std::shared_ptr<Object> sumElements(const std::vector<std::shared_ptr<Object>>& Elements){
    if(Elements.size() == 0)
        return std::make_shared<IntegerObject>(0);
    double sum = 0;
    int size = Elements.size();
    std::string elemType;
    bool hasDouble = false;
    for(int i=0; i<size; ++i){
        elemType = Elements[i]->Type();
        if(elemType != ObjectType::INTEGER_OBJ && elemType != ObjectType::DOUBLE_OBJ)
            return std::make_shared<ErrorObject>("sum function can't supporte type "+ elemType);
        if(!hasDouble && elemType == ObjectType::DOUBLE_OBJ)
            hasDouble = true;
        sum += atof(Elements[i]->Inspect().c_str());
    }
    if(hasDouble)
        return std::make_shared<DoubleObject>(sum);
    return std::make_shared<IntegerObject>(sum);
}

void initBuiltin(){
    //内置函数
    //len函数
    builtins["len"] = std::make_shared<BuiltinObject>([&](const std::vector<std::shared_ptr<Object>>& paras)->std::shared_ptr<Object>{
        if(paras.size() != 1)
        return std::make_shared<ErrorObject>("wrong number of arguments. got "
                                   +std::to_string(paras.size())+", want 1.");
        std::string type = paras[0]->Type();
        if(type == ObjectType::STRING_OBJ)
            return std::make_shared<IntegerObject>(dynamic_cast<StringObject*>(paras[0].get())->Value.size());
        else if(type == ObjectType::ARRAY_OBJ)
            return std::make_shared<IntegerObject>(dynamic_cast<ArrayObject*>(paras[0].get())->Elements.size());
        else if(type == ObjectType::HASH_OBJ)
            return std::make_shared<IntegerObject>(dynamic_cast<HashObject*>(paras[0].get())->Pairs.size());
        return std::make_shared<ErrorObject>("argument to `len` not supported, got "+type);
    });

    //type函数, 为了省事就不定义TypeObject了，直接返回StringObject
    builtins["type"] = std::make_shared<BuiltinObject>([&](const std::vector<std::shared_ptr<Object>>& paras)->std::shared_ptr<Object>{
        if(paras.size() != 1)
            return std::make_shared<ErrorObject>("wrong number of arguments. got "
                                   +std::to_string(paras.size())+", want 1.");
        return std::make_shared<StringObject>(paras[0]->Type());
    });

    //sum函数
    builtins["sum"] = std::make_shared<BuiltinObject>([&](const std::vector<std::shared_ptr<Object>>& paras)->std::shared_ptr<Object>{
        if(paras.size() == 0)
            return std::make_shared<IntegerObject>(0);
        std::string type = paras[0]->Type();
        if(type == ObjectType::ARRAY_OBJ && paras.size() == 1){
            return sumElements(dynamic_cast<ArrayObject*>(paras[0].get())->Elements);
        }
        return sumElements(paras); 
    });

    //print函数
    builtins["print"] = std::make_shared<BuiltinObject>([&](const std::vector<std::shared_ptr<Object>>& paras)->std::shared_ptr<Object>{
        int size = paras.size();
        for(int i=0; i<size; ++i)
            std::cout << paras[i]->Inspect() << std::endl;
        return nullObject;
    });

    //abs函数
    builtins["abs"] = std::make_shared<BuiltinObject>([&](const std::vector<std::shared_ptr<Object>>& paras)->std::shared_ptr<Object>{
        if(paras.size() != 1)
            return std::make_shared<ErrorObject>("wrong number of arguments. got "
                                   +std::to_string(paras.size())+", want 1.");
        if(IntegerObject* iobj = dynamic_cast<IntegerObject*>(paras[0].get()))
            return std::make_shared<IntegerObject>(iobj->Value >= 0 ? iobj->Value : -iobj->Value);
        else if(DoubleObject* dobj = dynamic_cast<DoubleObject*>(paras[0].get()))
            return std::make_shared<DoubleObject>(dobj->Value >= 0 ? dobj->Value : -dobj->Value);
        return std::make_shared<ErrorObject>("abs function can't support type: "+paras[0]->Type());
    });

    //time函数
    builtins["time"] = std::make_shared<BuiltinObject>([&](const std::vector<std::shared_ptr<Object>>& paras)->std::shared_ptr<Object>{
        if(paras.size() != 0)
            return std::make_shared<ErrorObject>("wrong number of arguments. got "
                                   +std::to_string(paras.size())+", want 0.");
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
	    std::tm* now_tm = std::localtime(&now_time_t);
	    char buffer[128];
	    strftime(buffer, sizeof(buffer), "%F %T", now_tm);
        return std::make_shared<StringObject>(buffer);
    });

    //exit函数
    builtins["exit"] = std::make_shared<BuiltinObject>([&](const std::vector<std::shared_ptr<Object>>& paras)->std::shared_ptr<Object>{
        if(paras.size() != 0)
            return std::make_shared<ErrorObject>("wrong number of arguments. got "
                                   +std::to_string(paras.size())+", want 0.");

        return exitObject;
    });

    //强制类型转换
    builtins["int"] = std::make_shared<BuiltinObject>([&](const std::vector<std::shared_ptr<Object>>& paras)->std::shared_ptr<Object>{
        if(paras.size() != 1)
            return std::make_shared<ErrorObject>("wrong number of arguments. got "
                                   +std::to_string(paras.size())+", want 1.");
        std::string type = paras[0]->Type();
        if(type == ObjectType::INTEGER_OBJ || 
           type == ObjectType::DOUBLE_OBJ){
               return std::make_shared<IntegerObject>(stoi(paras[0]->Inspect()));
           }else if(StringObject* sobj = dynamic_cast<StringObject*>(paras[0].get()))
               return std::make_shared<IntegerObject>(stoi(sobj->Value));
                
        return std::make_shared<ErrorObject>("call int error");
    });

    builtins["double"] = std::make_shared<BuiltinObject>([&](const std::vector<std::shared_ptr<Object>>& paras)->std::shared_ptr<Object>{
        if(paras.size() != 1)
            return std::make_shared<ErrorObject>("wrong number of arguments. got "
                                   +std::to_string(paras.size())+", want 1.");
        std::string type = paras[0]->Type();
        if(type == ObjectType::INTEGER_OBJ || 
           type == ObjectType::DOUBLE_OBJ){
               return std::make_shared<DoubleObject>(atof(paras[0]->Inspect().c_str()));
           }else if(StringObject* sobj = dynamic_cast<StringObject*>(paras[0].get()))
               return std::make_shared<DoubleObject>(stof(sobj->Value));
                
        return std::make_shared<ErrorObject>("call double error");
    });

    builtins["str"] = std::make_shared<BuiltinObject>([&](const std::vector<std::shared_ptr<Object>>& paras)->std::shared_ptr<Object>{
        if(paras.size() != 1)
            return std::make_shared<ErrorObject>("wrong number of arguments. got "
                                   +std::to_string(paras.size())+", want 1.");
        return std::make_shared<StringObject>(paras[0]->Inspect());
    });

}

#endif
