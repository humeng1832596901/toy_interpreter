/*******************************************************************
	> File Name: HashObject.h
	> Author: 胡孟
	> Mail: 13535324513@163.com
	> Created Time: Fri 05 Aug 2022 01:56:37 AM CST
 ******************************************************************/

#ifndef _HASHOBJECT_H
#define _HASHOBJECT_H

#include "Object.h"
#include "BooleanObject.h"
#include "IntegerObject.h"
#include "StringObject.h"
#include <unordered_map>
#include <memory>

struct HashKeyObject : Object {

    HashKeyObject(const std::shared_ptr<Object>& obj)
        : key(obj->Inspect())
    {
        if(BooleanObject* bobj = dynamic_cast<BooleanObject*>(obj.get())){
            keyType = ObjectType::BOOLEAN_OBJ;
            Value = bobj->Value ? 1 : 0;
        }else if(IntegerObject* iobj = dynamic_cast<IntegerObject*>(obj.get())){
            keyType = ObjectType::INTEGER_OBJ;
            Value = int_hash(iobj->Value);
        }else if(StringObject* sobj = dynamic_cast<StringObject*>(obj.get())){
            keyType = ObjectType::STRING_OBJ;
            Value = str_hash(sobj->Value);
        }
        keyType = "null";
    }

    std::string Type() const override {
        return ObjectType::HASHKEY_OBJ;
    }

    std::string Inspect() const override {
        return key;
    }

    std::hash<std::string> str_hash;
    std::hash<int> int_hash;
    std::string keyType;
    std::string key;
    size_t Value;  
};

struct pair_hash{
    std::size_t  operator()(const HashKeyObject& hkobj) const {
        return hkobj.Value;
    }
};

struct pair_equal{
    bool operator()(const HashKeyObject& a, const HashKeyObject& b) const {
        return a.keyType == b.keyType && a.Value == b.Value;
    }
};

struct HashObject : Object {

    HashObject() = default;

    std::string Type() const override {
        return ObjectType::HASH_OBJ;
    }

    std::string Inspect() const override {
        std::string str = "{";
        int size = Pairs.size();
        int idx = 0;
        for(auto& p : Pairs){
            str += (p.first.Inspect()+":"+p.second->Inspect());
            if(idx != size-1)
                str += ", ";
            idx++;
        }
        return str+"}";
    }
    
    std::shared_ptr<Object> find(const std::shared_ptr<Object>& key){
        HashKeyObject tmp(key);
        if(tmp.Type() == "null")
            return std::make_shared<ErrorObject>("unusable as hash key: "+key->Type());
        if(Pairs.find(tmp) == Pairs.end())
            return std::make_shared<ErrorObject>("key "+key->Inspect()+ " not found");
        else
            return Pairs[tmp];
        return std::make_shared<ErrorObject>("something error when find "+key->Inspect()+" from the hashtable");
    }

    std::unordered_map<HashKeyObject, std::shared_ptr<Object>, pair_hash, pair_equal> Pairs;  
};


#endif
