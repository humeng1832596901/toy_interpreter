/*******************************************************************
	> File Name: evaluator.h
	> Author: 胡孟
	> Mail: 13535324513@163.com
	> Created Time: Sat 30 Jul 2022 07:36:09 PM CST
 ******************************************************************/

#ifndef _EVALUATOR_H
#define _EVALUATOR_H

#include "../ast/ast.h"
#include "../object/object.h"
#include "../environment/environment.h"
#include "../builtin/builtin.h"
#include <vector>

std::shared_ptr<Object> Eval(Node*, Environment*);

bool isError(const std::shared_ptr<Object>& obj){
    return obj->Type() == ObjectType::ERROR_OBJ;
}

std::shared_ptr<Object> evalStatements(std::vector<Statement*>& stmts, Environment* env){
    std::shared_ptr<Object> res = nullObject;
    int size = stmts.size();
    for(int i=0; i<size; ++i){
        res = Eval(stmts[i], env);
    }
    return res;
}

std::shared_ptr<Object> evalBangOperatorExpression(std::shared_ptr<Object>& right){
    if(right == trueObject)
        return falseObject;
    else if(right == falseObject)
        return trueObject;
    else if(right == nullObject)
        return trueObject;
    //这里需要对几种情况进行判断，如整数中的0，空字符串，空数组等等
    if(right->Type() == ObjectType::INTEGER_OBJ && right->Inspect() == "0")
        //对0
        return trueObject;    
    
    return falseObject;
}

std::shared_ptr<Object> evalMinusPrefixOperatorExpression(std::shared_ptr<Object>& right){
    if(IntegerObject* iobj = dynamic_cast<IntegerObject*>(right.get()))
        return std::make_shared<IntegerObject>(-iobj->Value);
    else if(DoubleObject* dobj = dynamic_cast<DoubleObject*>(right.get()))
        return std::make_shared<DoubleObject>(-dobj->Value);

    return std::make_shared<ErrorObject>("unknow operator: -"+right->Type());
}

std::shared_ptr<Object> evalPrefixExpression(const std::string& Operator, std::shared_ptr<Object>& right){
    if(Operator == "!"){
        return evalBangOperatorExpression(right);    
    }else if(Operator == "-")
        return evalMinusPrefixOperatorExpression(right);
    return std::make_shared<ErrorObject>("unknow operator: "+Operator+right->Type());
}

/*
Object* evalIntegerInfixExpression(const std::string& Operator, IntegerObject* left, IntegerObject* right){
    if(Operator == "+")
        return new IntegerObject(left->Value + right->Value);
    else if(Operator == "-")
        return new IntegerObject(left->Value - right->Value);
    else if(Operator == "*")
        return new IntegerObject(left->Value * right->Value);
    else if(Operator == "/") 
        return new IntegerObject(left->Value / right->Value);
    else if(Operator == "<")
        return left->Value < right->Value ? &trueObject : &falseObject;
    else if(Operator == ">")
        return left->Value > right->Value ? &trueObject : &falseObject;
    else if(Operator == "==")
        return left->Value == right->Value ? &trueObject : &falseObject;
    else if(Operator == "!=")
        return left->Value != right->Value ? &trueObject : &falseObject;
    else
        return new ErrorObject("unknow operator: "+left->Type()+Operator+right->Type());
}
*/

template<typename obj>
std::shared_ptr<Object> evalInfixExpression(const std::string& Operator, 
                                            const std::shared_ptr<obj>& left, 
                                            const std::shared_ptr<obj>& right){
    if(Operator == "+")
        return std::make_shared<obj>(left->Value + right->Value);
    else if(Operator == "-")
        return std::make_shared<obj>(left->Value - right->Value);
    else if(Operator == "*")
        return std::make_shared<obj>(left->Value * right->Value);
    else if(Operator == "/") 
        return std::make_shared<obj>(left->Value / right->Value);
    else if(Operator == "<")
        return left->Value < right->Value ? trueObject : falseObject;
    else if(Operator == ">")
        return left->Value > right->Value ? trueObject : falseObject;
    else if(Operator == "==")
        return left->Value == right->Value ? trueObject : falseObject;
    else if(Operator == "!=")
        return left->Value != right->Value ? trueObject : falseObject;
    else
        return std::make_shared<ErrorObject>("unknow trivial operator: "+left->Type()+Operator+right->Type());
}

template<>
std::shared_ptr<Object> evalInfixExpression(const std::string& Operator, 
                                            const std::shared_ptr<StringObject>& left, 
                                            const std::shared_ptr<StringObject>& right){
    if(Operator == "+")
        return std::make_shared<StringObject>(left->Value + right->Value);
    else if(Operator == "<")
        return left->Value < right->Value ? trueObject : falseObject;
    else if(Operator == ">")
        return left->Value > right->Value ? trueObject : falseObject;
    else if(Operator == "==")
        return left->Value == right->Value ? trueObject : falseObject;
    else if(Operator == "!=")
        return left->Value != right->Value ? trueObject : falseObject;
    else
        return std::make_shared<ErrorObject>("unknow StringObject operator: "+left->Type()+Operator+right->Type());
}

std::shared_ptr<Object> evalInfixExpression(const std::string& Operator, 
                            const std::shared_ptr<Object>& left, 
                            const std::shared_ptr<Object>& right){
    if(left->Type() != right->Type())
        return std::make_shared<ErrorObject>("type mismatch: "+left->Type()+Operator+right->Type());
    if(left->Type() == ObjectType::INTEGER_OBJ)
        return evalInfixExpression<IntegerObject>(Operator, 
                                                  std::dynamic_pointer_cast<IntegerObject>(left),
                                                  std::dynamic_pointer_cast<IntegerObject>(right));
    else if(left->Type() == ObjectType::DOUBLE_OBJ)
        return evalInfixExpression<DoubleObject>(Operator, 
                                                 std::dynamic_pointer_cast<DoubleObject>(left),
                                                 std::dynamic_pointer_cast<DoubleObject>(right));
    else if(left->Type() == ObjectType::STRING_OBJ)
        return evalInfixExpression<StringObject>(Operator, 
                                                 std::dynamic_pointer_cast<StringObject>(left),
                                                 std::dynamic_pointer_cast<StringObject>(right));
        
    return std::make_shared<ErrorObject>("unknow operator: "+left->Type()+Operator+right->Type());
}

bool isTruthy(const std::shared_ptr<Object>& condition){
    if(condition == nullObject || condition == falseObject)
        return false;
    return true;
}

std::shared_ptr<Object> evalIfExpression(IfExpression* iExp, Environment* env){
    std::shared_ptr<Object> condition = Eval(iExp->Condition, env);
    if(isError(condition))
        return condition;
    if(isTruthy(condition)){
        return Eval(iExp->Consequence, env);
    }else if(iExp->Alternative)
        return Eval(iExp->Alternative, env);
    return nullObject;
}

std::shared_ptr<Object> evalWhileExpression(WhileExpression* wExp, Environment* env){
    std::shared_ptr<Object> condition = Eval(wExp->Condition, env);
    if(isError(condition))
        return condition;
    while(isTruthy(condition)){
        std::shared_ptr<Object> res = Eval(wExp->Consequence, env);
        condition = Eval(wExp->Condition, env);
    }
    return nullObject;
}

std::shared_ptr<Object> evalProgram(Program* program, Environment* env){
    std::shared_ptr<Object> res = nullObject;
    int size = program->Statements.size();
    for(int i=0; i<size; ++i){
        res = Eval(program->Statements[i], env);
        if(ReturnObject* robj = dynamic_cast<ReturnObject*>(res.get())){
            return robj->Value;
        }else if(ErrorObject* eobj = dynamic_cast<ErrorObject*>(res.get()))
            return res;
    }
    return res;
}

std::shared_ptr<Object> evalBlockStatement(BlockStatement* bstmt, Environment* env){
    std::shared_ptr<Object> res = nullObject;
    int size = bstmt->Statements.size();
    for(int i=0; i<size; ++i){
        res = Eval(bstmt->Statements[i], env);
        if(res != nullObject && 
           res->Type() == ObjectType::RETURN_VALUE_OBJ || res->Type() == ObjectType::ERROR_OBJ)
            return res;
    }
    return res;
}

std::shared_ptr<Object> evalIdentifier(Identifier* ident, Environment* env){
    std::shared_ptr<Object> res = env->Get(ident->Value);
    if(isError(res)){
        if(builtins.find(ident->Value) == builtins.end())
            return std::make_shared<ErrorObject>("identifier not found: "+ident->Value);
        return builtins[ident->Value];
    }
    return res;
}

bool evalExpressions(std::vector<std::shared_ptr<Object>>& args, 
                     std::vector<Expression*>& Arguments, 
                     Environment* env){
    for(auto arg : Arguments){
        std::shared_ptr<Object> evaluated = Eval(arg, env);
        args.push_back(evaluated);
        if(isError(evaluated))
            return false;
    }
    return true;
}

Environment* extendFunctionEnv(FunctionObject* fn, 
                               std::vector<std::shared_ptr<Object>>& args){
    Environment* env = NewEnclosedEnvironment(fn->Env);
    if(fn->Parameters.size() != args.size()){
        //Todo
    }
    int size = args.size();
    for(int i=0; i<size; ++i){
        env->Set(fn->Parameters[i]->Value, std::move(args[i]));
    }
    return env;
}

std::shared_ptr<Object> unwrapReturnValue(std::shared_ptr<Object>& evaluated){
    if(ReturnObject* rObj = dynamic_cast<ReturnObject*>(evaluated.get()))
        return rObj->Value;
    return evaluated;
}

std::shared_ptr<Object> applyFunction(std::shared_ptr<Object>& function, 
                                      std::vector<std::shared_ptr<Object>>& args){
    if(FunctionObject* fn = dynamic_cast<FunctionObject*>(function.get())){
        Environment* extendedEnv = extendFunctionEnv(fn, args);
        std::shared_ptr<Object> evaluated = Eval(fn->Body, extendedEnv);
        return unwrapReturnValue(evaluated);
    }else if(BuiltinObject* bfn = dynamic_cast<BuiltinObject*>(function.get())){
        return bfn->fn(args);
    }else{
        return std::make_shared<ErrorObject>("type conversion from " + 
                                            function->Type() + 
                                            " to FunctionObject error");
    }
}

std::shared_ptr<Object> evalArrayIndexExpression(const std::shared_ptr<ArrayObject>& left, 
                                                 const std::shared_ptr<IntegerObject>& index){
    int size = left->Elements.size()-1;
    if(index->Value<0 || index->Value>size)
        return std::make_shared<ErrorObject>("index out of range");
    return left->Elements[index->Value];
}

std::shared_ptr<Object> evalHashIndexExpression(const std::shared_ptr<HashObject>& hobj, 
                                                const std::shared_ptr<Object>& index){
    return hobj->find(index);
}

std::shared_ptr<Object> evalIndexExpression(const std::shared_ptr<Object>& left, 
                                            const std::shared_ptr<Object>& index){
    if(left->Type() == ObjectType::ARRAY_OBJ && index->Type() == ObjectType::INTEGER_OBJ)
        return evalArrayIndexExpression(std::dynamic_pointer_cast<ArrayObject>(left)
                                        , std::dynamic_pointer_cast<IntegerObject>(index));
    else if(left->Type() == ObjectType::HASH_OBJ)
        return evalHashIndexExpression(std::dynamic_pointer_cast<HashObject>(left),index);

    return std::make_shared<ErrorObject>("index operator not supported: " + left->Type());
}

std::shared_ptr<Object> evalHashLiteral(HashLiteral* hlit, Environment* env){
    std::shared_ptr<HashObject> hobj = std::make_shared<HashObject>();
    for(auto& p : hlit->Pairs){
        std::shared_ptr<Object> key = Eval(p.first, env);
        if(isError(key)){
            return key; 
        }
        
        std::string type = key->Type();
        if(type != ObjectType::BOOLEAN_OBJ &&
           type != ObjectType::INTEGER_OBJ &&
           type != ObjectType::STRING_OBJ){
               return std::make_shared<ErrorObject>("unusable as hash key: "+type);
        }
        HashKeyObject hkobj(key);
        std::shared_ptr<Object> val = Eval(p.second, env);
        if(isError(val)){
            return val;
        }
        hobj->Pairs.insert({std::move(hkobj), std::move(val)}); 
    }
    return hobj;
}

std::shared_ptr<Object> applyNormalAssignment(Identifier* ident, Expression* Value, Environment* env){
    std::shared_ptr<Object> oldVal = env->Get(ident->Value);
    if(isError(oldVal))
        return oldVal;
    std::shared_ptr<Object> val = Eval(Value, env);
    if(isError(val))
        return val;
    if(val->Type() != oldVal->Type())
        return std::make_shared<ErrorObject>("can't convert "+val->Type()+" to "+oldVal->Type());
    env->Set(ident->Value, std::move(val));
    return nullObject;
}

std::shared_ptr<Object> applyIndexAssignment(IndexExpression* index, Expression* Value, Environment* env){
    std::shared_ptr<Object> left = Eval(index->Left, env);
    if(isError(left))
        return left;
    std::shared_ptr<Object> idx = Eval(index->Index, env);
    if(isError(idx))
        return idx;
    std::shared_ptr<Object> val = Eval(Value, env);
    if(isError(val))
        return val;
    if(std::shared_ptr<HashObject> hobj = std::dynamic_pointer_cast<HashObject>(left))
        hobj->Pairs.insert({std::move(HashKeyObject(idx)), 
                            std::move(val)});
    else if(std::shared_ptr<ArrayObject> aobj = std::dynamic_pointer_cast<ArrayObject>(left)){
        if(idx->Type() != ObjectType::INTEGER_OBJ)
            return std::make_shared<ErrorObject>("index operator not supported: " + left->Type());
        int i = stoi(idx->Inspect());
        if(i<0 || i>=aobj->Elements.size())
            return std::make_shared<ErrorObject>("index out of range");
        aobj->Elements[i] = val;
    }
    return nullObject;
}

std::shared_ptr<Object> Eval(Node* node, Environment* env){
    if(Program* p = dynamic_cast<Program*>(node)){
        return evalProgram(p, env);
    }else if(ExpressionStatement* exp = dynamic_cast<ExpressionStatement*>(node)){
        return Eval(exp->Exp, env);
    }else if(IntegerLiteral* ilit = dynamic_cast<IntegerLiteral*>(node)){
        return std::make_shared<IntegerObject>(ilit->Value);
    }else if(DoubleLiteral* dlit = dynamic_cast<DoubleLiteral*>(node)){
        return std::make_shared<DoubleObject>(dlit->Value);
    }else if(BooleanLiteral* blit = dynamic_cast<BooleanLiteral*>(node)){
        return blit->Value ? trueObject : falseObject;
    }else if(PrefixExpression* pExp = dynamic_cast<PrefixExpression*>(node)){
        std::shared_ptr<Object> right = Eval(pExp->Right, env);
        if(isError(right))
            return right;
        return evalPrefixExpression(pExp->Operator, right);
    }else if(InfixExpression* iExp = dynamic_cast<InfixExpression*>(node)){
        std::shared_ptr<Object> left = Eval(iExp->Left, env);
        if(isError(left))
            return left;
        std::shared_ptr<Object> right = Eval(iExp->Right, env);
        if(isError(right))
            return right;
        return evalInfixExpression(iExp->Operator, left, right);
    }else if(IfExpression* iExp = dynamic_cast<IfExpression*>(node)){
        return evalIfExpression(iExp, env);
    }else if(BlockStatement* bstmt = dynamic_cast<BlockStatement*>(node)){
        return evalBlockStatement(bstmt, env);
    }else if(ReturnStatement* rstmt = dynamic_cast<ReturnStatement*>(node)){
        std::shared_ptr<Object> obj = Eval(rstmt->Value, env);
        if(isError(obj))
            return obj;
        return std::make_shared<ReturnObject>(obj);
    }else if(LetStatement* lstmt = dynamic_cast<LetStatement*>(node)){
        std::shared_ptr<Object> val = Eval(lstmt->Value, env);
        if(isError(val))
            return val;
        env->Set(lstmt->Name.Value, std::move(val));
    }else if(Identifier* ident = dynamic_cast<Identifier*>(node)){
        return evalIdentifier(ident, env);
    }else if(FunctionLiteral* flit = dynamic_cast<FunctionLiteral*>(node)){
        return std::make_shared<FunctionObject>(flit->Body, flit->Parameters, env); 
    }else if(CallExpression* cExp = dynamic_cast<CallExpression*>(node)){
        std::shared_ptr<Object> function = Eval(cExp->Function, env);
        if(isError(function))
            return function;
        std::vector<std::shared_ptr<Object>> args;
        if(not evalExpressions(args, cExp->Arguments, env))
            return *args.rbegin();
        return applyFunction(function, args);
    }else if(StringLiteral* slit = dynamic_cast<StringLiteral*>(node)){
        return std::make_shared<StringObject>(slit->Value);
    }else if(ArrayLiteral* alit = dynamic_cast<ArrayLiteral*>(node)){
        std::vector<std::shared_ptr<Object>> Elements;
        if(not evalExpressions(Elements, alit->Elements, env))
            return *Elements.rbegin();
        return std::make_shared<ArrayObject>(Elements);
    }else if(IndexExpression* idxExp = dynamic_cast<IndexExpression*>(node)){
        std::shared_ptr<Object> left = Eval(idxExp->Left, env);
        if(isError(left))
            return left;
        std::shared_ptr<Object> index = Eval(idxExp->Index, env);
        if(isError(index))
            return index;
        return evalIndexExpression(left, index);
    }else if(HashLiteral* hlit = dynamic_cast<HashLiteral*>(node)){
        return evalHashLiteral(hlit, env); 
    }else if(AssignStatement* astmt = dynamic_cast<AssignStatement*>(node)){
        if(Identifier* ident = dynamic_cast<Identifier*>(astmt->Name))
            return applyNormalAssignment(ident, astmt->Value, env);
        else if(IndexExpression* index = dynamic_cast<IndexExpression*>(astmt->Name))
            return applyIndexAssignment(index, astmt->Value, env);
    }else if(WhileExpression* wExp = dynamic_cast<WhileExpression*>(node))
        return evalWhileExpression(wExp, env);
    return nullObject;
}

#endif
