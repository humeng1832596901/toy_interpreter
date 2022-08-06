/*******************************************************************
	> File Name: ast.h
	> Author: 胡孟
	> Mail: 13535324513@163.com
	> Created Time: Tue 26 Jul 2022 02:09:42 PM CST
 ******************************************************************/

#ifndef _AST_H
#define _AST_H

#include <vector>
#include <memory>
#include <unordered_set>
#include "../token.h"

struct Node{
    virtual std::string TokenLiteral() const = 0;
    virtual std::string String() const = 0;
    virtual ~Node(){}
};

struct Statement : Node{
    Statement() = default;
    virtual std::string String() const override {
        return "";
    }
    virtual std::string TokenLiteral() const override {
        return "";
    }
    virtual ~Statement(){}
};

struct Expression : Node{
    virtual std::string TokenLiteral()const override{return "";}  
    virtual std::string String()const override{return ""; }
};

Statement* traitsAndNewStatement(Statement*);
Expression* traitsAndNewExpression(Expression*);

struct Identifier : Expression{

    Identifier() = default;

    Identifier(const Token& tok, const std::string& val)
        : token(tok),
          Value(val)
    {}

    Identifier(const Identifier& other){
        token = other.token;
        Value = other.Value;
    }

    std::string String() const override {
        return Value;
    }

    std::string TokenLiteral() const override {
        return token.Literal;
    }

    Token token;
    std::string Value;
};

struct IntegerLiteral : Expression{

    IntegerLiteral() = default;

    IntegerLiteral(const Token& tok)
        :token(tok)
    {}

    IntegerLiteral(const IntegerLiteral& other){
        token = other.token;
        Value = other.Value;
    }

    std::string String() const override {
        return token.Literal;
    }

    std::string TokenLiteral() const override {
        return token.Literal;
    }

    Token token;
    int64_t Value;
};

struct DoubleLiteral : Expression{

    DoubleLiteral() = default;

    DoubleLiteral(const Token& tok)
        :token(tok)
    {}

    DoubleLiteral(const DoubleLiteral& other){
        token = other.token;
        Value = other.Value;
    }

    std::string String() const override {
        return token.Literal;
    }

    std::string TokenLiteral() const override {
        return token.Literal;
    }

    Token token;
    double Value;
};

struct StringLiteral : Expression {

    StringLiteral() = default;

    StringLiteral(const Token& tok, const std::string& str)
        : token(tok),
          Value(str)
    {}

    StringLiteral(const StringLiteral& other){
        token = other.token;
        Value = other.Value;
    }

    std::string String() const override {
        return token.Literal;
    }

    std::string TokenLiteral() const override {
        return token.Literal;
    }

    Token token;
    std::string Value;
};

struct BooleanLiteral : Expression{

    BooleanLiteral() = default;

    BooleanLiteral(const Token& tok, bool val)
        : token(tok),
          Value(val)
    {}

    BooleanLiteral(const BooleanLiteral& other){
        token = other.token;
        Value = other.Value;
    }

    std::string String() const override {
        return token.Literal;
    }

    std::string TokenLiteral() const override {
        return token.Literal;
    }

    Token token;
    bool Value;
};

//块语句
struct BlockStatement : Statement{

    BlockStatement(const Token& tok)
        : token(tok)
    {}

    BlockStatement(const BlockStatement& other){
        token = other.token;
        int size = other.Statements.size();
        for(int i=0; i<size; ++i){
            //注意这里的statement*可能是派生类, 有点难搞
            Statement* stmt = traitsAndNewStatement(other.Statements[i]);
            Statements.push_back(stmt);
        }
    }

    ~BlockStatement(){
        int size = Statements.size();
        for(int i=0; i<size; ++i){
            if(Statements[i]){
                delete Statements[i];
                Statements[i] = nullptr;
            }
        }
        Statements.clear();
    }

    std::string TokenLiteral() const override {
        return token.Literal;
    }

    std::string String() const override {
        std::string str;
        int size = Statements.size();
        for(int i=0; i<size; ++i){
            str += Statements[i]->String();
        }
        return str;
    }

    Token token;
    std::vector<Statement*> Statements;
};

struct FunctionLiteral : Expression {

    FunctionLiteral(const Token& tok)
        : token(tok),
          Body(nullptr)
    {}

    FunctionLiteral(const FunctionLiteral& other){
        token = other.token;
        Body = traitsAndNewStatement(other.Body);
        int size = other.Parameters.size();
        for(int i=0; i<size; ++i)
            Parameters.push_back(new Identifier(*other.Parameters[i]));
    }

    ~FunctionLiteral(){
        if(Body){
            delete Body;
            Body = nullptr;
        }
        int size = Parameters.size();
        for(int i=0; i<size; ++i){
            if(Parameters[i]){
                delete Parameters[i];
                Parameters[i] = nullptr;
            }
        }
        Parameters.clear();
    }

    std::string TokenLiteral() const override {
        return token.Literal;
    }

    std::string String() const override {
        std::string paras = "fn(";
        int size = Parameters.size();
        for(int i=0; i<size; ++i){
            paras += Parameters[i]->String();
            if(i != size-1)
                paras += ",";
        }
        paras += ")";
        if(!Body)
            return paras+"{}";
        return paras +"{" + Body->String() + "}";
    }

    Token token;
    std::vector<Identifier*> Parameters;
    Statement* Body;
};

struct CallExpression : Expression {
   
    CallExpression(const Token& tok, Expression* function)
        : token(tok),
          Function(function)
    {}

    CallExpression(const CallExpression& other){
        token = other.token;
        Function = traitsAndNewExpression(other.Function);
        int size = other.Arguments.size();
        for(int i=0; i<size; ++i){
            Arguments.push_back(traitsAndNewExpression(other.Arguments[i]));
        }
    }

    ~CallExpression(){
        if(Function){
            delete Function;
            Function = nullptr;
        }
        int size = Arguments.size();
        for(int i=0; i<size; ++i){
            delete Arguments[i];
            Arguments[i] = nullptr;
        }
        Arguments.clear();
    }

    std::string TokenLiteral() const override {
        return token.Literal;
    }

    std::string String() const override {
        if(!Function)
            return "";
        std::string str = Function->String() + "(";
        int size = Arguments.size();
        for(int i=0; i<size; ++i){
            str += Arguments[i]->String();
            if(i != size-1)
                str += ", "; 
        }
        str += ")";
        return str;
    }

    Token token;
    Expression* Function;
    std::vector<Expression*> Arguments; 
};

//let语句
struct LetStatement : Statement{

    LetStatement() = default;

    LetStatement(const LetStatement& other){
        token = other.token;
        Name = other.Name;
    }

    ~LetStatement(){
        if(Value){
            delete Value;
            Value = nullptr;
        }
    }

    LetStatement(const Token& tok)
        : token(tok),
          Value(nullptr)
    {}

    std::string TokenLiteral() const override {
        return token.Literal;
    }
    
    std::string String() const override {
        if(Value)
            return token.Literal + " " + Name.String() + " = " + Value->String() + ";";
        return "";
    }

    Token token;
    Identifier Name;
    Expression* Value;
};

//赋值语句
struct AssignStatement : Statement{

    AssignStatement() = default;

    AssignStatement(const AssignStatement& other){
        token = other.token;
        Name = traitsAndNewExpression(other.Name);
        Value = traitsAndNewExpression(other.Value);
    }

    ~AssignStatement(){
        if(Name){
            delete Name;
            Name = nullptr;
        }
        if(Value){
            delete Value;
            Value = nullptr;
        }
    }

    AssignStatement(const Token& tok)
        : token(tok),
          Name(nullptr),
          Value(nullptr)
    {}

    std::string TokenLiteral() const override {
        return token.Literal;
    }
    
    std::string String() const override {
        if(Value)
            return token.Literal + " " + Name->String() + " = " + Value->String() + ";";
        return "";
    }

    Token token;
    Expression* Name; //Identifier HashLiteral ArrayLiteral
    //Identifier Name;
    Expression* Value;
};

//return语句
struct ReturnStatement : Statement{
   
    ~ReturnStatement(){
        if(Value){
            delete Value;
            Value = nullptr;
        }
    }

    ReturnStatement(const Token& tok)
        : token(tok)
    {}

    ReturnStatement(const ReturnStatement& other)
        : token(other.token)
    {
        Value = traitsAndNewExpression(other.Value);
    }

    std::string TokenLiteral() const override {
        return token.Literal;
    }

    std::string String() const override {
        if(Value)
            return token.Literal + " " + Value->String() + ";";
        return "";
    }

    Token token;
    Expression* Value;
};

//表达式语句
struct ExpressionStatement : Statement{
      
    ~ExpressionStatement(){
        if(Exp){
            delete Exp;
            Exp = nullptr;
        }
    }

    ExpressionStatement(const Token& tok)
        : token(tok),
          Exp(nullptr)
    {}

    ExpressionStatement(const ExpressionStatement& other){
        token = other.token;
        Exp = traitsAndNewExpression(other.Exp);
    }

    std::string TokenLiteral() const override {
        return token.Literal;
    }

    std::string String() const override {
        if(Exp)
            return Exp->String();
        return "";
    }

    Token token;
    Expression* Exp;
};

//if语句
struct IfExpression : Expression{

    IfExpression(const Token& tok)
        : token(tok),
          Condition(nullptr),
          Consequence(nullptr),
          Alternative(nullptr)
    {}

    IfExpression(const IfExpression& other){
        token = other.token;
        Condition = traitsAndNewExpression(other.Condition);
        Consequence = traitsAndNewStatement(other.Consequence);
        Alternative = traitsAndNewStatement(other.Alternative);
    }

    ~IfExpression(){
        if(Condition){
            delete Condition;
            Condition = nullptr;
        }
        if(Consequence){
            delete Consequence;
            Consequence = nullptr;
        }
        if(Alternative){
            delete Alternative;
            Alternative = nullptr;
        }
    }

    std::string TokenLiteral() const override {
        return token.Literal;
    }

    std::string String() const override {
        std::string str = "if" + Condition->String() + "{" + Consequence->String() + "}";
        if(Alternative)
            str += ("else{" + Alternative->String()+"}");
        return str;
    }

    Token token;
    Expression* Condition;
    Statement* Consequence;
    Statement* Alternative;
};

//while语句
struct WhileExpression : Expression{

    WhileExpression(const Token& tok)
        : token(tok),
          Condition(nullptr),
          Consequence(nullptr)
    {}

    WhileExpression(const WhileExpression& other){
        token = other.token;
        Condition = traitsAndNewExpression(other.Condition);
        Consequence = traitsAndNewStatement(other.Consequence);
    }

    ~WhileExpression(){
        if(Condition){
            delete Condition;
            Condition = nullptr;
        }
        if(Consequence){
            delete Consequence;
            Consequence = nullptr;
        }
    }

    std::string TokenLiteral() const override {
        return token.Literal;
    }

    std::string String() const override {
        return "while" + Condition->String() + "{" + Consequence->String() + "}";
    }

    Token token;
    Expression* Condition;
    Statement* Consequence;
};

struct PrefixExpression : Expression{

    ~PrefixExpression(){
        if(Right){
            delete Right;
            Right = nullptr;
        }
    }

    PrefixExpression(const Token& tok, const std::string& lit)
        : token(tok),
          Operator(lit),
          Right(nullptr)
    {}

    PrefixExpression(const PrefixExpression& other){
        token = other.token;
        Operator = other.Operator;
        Right = traitsAndNewExpression(other.Right);
    }

    std::string TokenLiteral() const override {
        return token.Literal;
    }

    std::string String() const override {
        return "(" + Operator + Right->String() + ")";
    }

    Token token; //前缀词法单元
    std::string Operator; //前缀符
    Expression* Right; //表达式
};

struct InfixExpression : Expression{
    
    ~InfixExpression(){
        if(Left){
            delete Left;
            Left = nullptr;
        }
        if(Right){
            delete Right;
            Right = nullptr;
        }
    }

    InfixExpression(const Token& tok, std::string lit, Expression* left)
        : token(tok),
          Operator(lit),
          Left(left),
          Right(nullptr)
    {}

    InfixExpression(const InfixExpression& other){
        token = other.token;
        Operator = other.Operator;
        Left = traitsAndNewExpression(other.Left);
        Right = traitsAndNewExpression(other.Right);
    }

    std::string TokenLiteral() const override {
        return token.Literal;
    }

    std::string String() const override {
        return "(" + Left->String() + " " + Operator + " " + Right->String() + ")";
    }

    Token token;  //二元运算符词法单元
    Expression* Left;
    std::string Operator;
    Expression* Right;
};

struct ArrayLiteral : Expression {

    ArrayLiteral() = default;

    ArrayLiteral(const Token& tok)
        :token(tok)
    {}

    ArrayLiteral(const ArrayLiteral& other){
        token = other.token;
        int size = 0;
        for(int i=0; i<other.Elements.size(); ++i)
            Elements.push_back(traitsAndNewExpression(other.Elements[i]));
    }

    ~ArrayLiteral(){
        int size = Elements.size();
        for(int i=0; i<size; ++i){
            delete Elements[i];
            Elements[i] = nullptr;
        }
        Elements.clear();
    }

    std::string TokenLiteral() const override {
        return token.Literal;
    }

    std::string String() const override {
        std::string str = "[";
        int size = Elements.size();
        for(int i=0; i<size; ++i){
            str += Elements[i]->String();
            if(i != size-1)
                str += ",";
        }
        return str+"]";
    }

    Token token;
    std::vector<Expression*> Elements;
};

struct IndexExpression : Expression {

    IndexExpression() = default;

    IndexExpression(const Token& tok, Expression* left)
        : token(tok),
          Left(left)
    {}

    IndexExpression(const IndexExpression& other){
        token = other.token;
        Left = traitsAndNewExpression(other.Left);
        Index = traitsAndNewExpression(other.Index);
    }

    ~IndexExpression(){
        if(Left){
            delete Left;
            Left = nullptr;
        }
        if(Index){
            delete Index;
            Index = nullptr;
        }
    }

    std::string TokenLiteral() const override {
        return token.Literal;
    }

    std::string String() const override {
        return "(" + Left->String() + "[" + Index->String() + "]";
    }

    Token token;
    Expression* Left;
    Expression* Index;
};

struct HashLiteral : Expression {

    HashLiteral() = default;

    HashLiteral(const Token& tok)
        : token(tok)
    {}

    HashLiteral(const HashLiteral& other){
        token = other.token;
        Expression *key, *value;
        for(auto& p : other.Pairs){
            key = traitsAndNewExpression(p.first);
            value = traitsAndNewExpression(p.second);
            Pairs.insert({key, value});
        }
    }

    ~HashLiteral(){
        for(auto& p : Pairs){
            delete p.first;
            delete p.second;
        }
        Pairs.clear();
    }

    std::string TokenLiteral() const override {
        return token.Literal;
    }

    std::string String() const override {
        std::string str = "{";
        int size = Pairs.size();
        int idx = 0;
        for(auto& p : Pairs){
            str += (p.first->String() + ": " + p.second->String());
            if(idx != size-1)
                str += ",";
            idx++;
        }
        return str+"}";
    }

    Token token;
    std::unordered_map<Expression*, Expression*> Pairs;
};

struct Program : Node{
   
    ~Program(){
        for(int i=0; i<Statements.size(); ++i){
            if(Statements[i]){
                delete Statements[i];
                Statements[i] = nullptr;
            }
        }
        Statements.clear();
    }
    
    std::string TokenLiteral() const override{
        if(Statements.size() > 0){
            return Statements[0]->TokenLiteral();        
        }
        return "";
    }

    std::string String() const override{
        std::string str;
        for(auto& sta : Statements)
            str += sta->String();
        return str;
    }

    std::vector<Statement*> Statements; 
};

Statement* traitsAndNewStatement(Statement* stmt){
    if(BlockStatement* bstmt = dynamic_cast<BlockStatement*>(stmt)){
        return new BlockStatement(*bstmt);
    }else if(ExpressionStatement* estmt = dynamic_cast<ExpressionStatement*>(stmt)){
        return new ExpressionStatement(*estmt);
    }else if(LetStatement* lstmt = dynamic_cast<LetStatement*>(stmt))
        return new LetStatement(*lstmt);
    else if(ReturnStatement* rstmt = dynamic_cast<ReturnStatement*>(stmt))
        return new ReturnStatement(*rstmt);
    else if(AssignStatement* astmt = dynamic_cast<AssignStatement*>(stmt))
        return new AssignStatement(*astmt);
    return nullptr;
}

Expression* traitsAndNewExpression(Expression* exp){
    if(IfExpression* iexp = dynamic_cast<IfExpression*>(exp)){
        return new IfExpression(*iexp);
    }else if(PrefixExpression* pexp = dynamic_cast<PrefixExpression*>(exp)){
        return new PrefixExpression(*pexp);
    }else if(InfixExpression* iexp = dynamic_cast<InfixExpression*>(exp)){
        return new InfixExpression(*iexp);
    }else if(CallExpression* cexp = dynamic_cast<CallExpression*>(exp)){
        return new CallExpression(*cexp);
    }else if(FunctionLiteral* flit = dynamic_cast<FunctionLiteral*>(exp)){
        return new FunctionLiteral(*flit);
    }else if(IntegerLiteral* ilit = dynamic_cast<IntegerLiteral*>(exp)){
        return new IntegerLiteral(*ilit);
    }else if(DoubleLiteral* dlit = dynamic_cast<DoubleLiteral*>(exp)){
        return new DoubleLiteral(*dlit);
    }else if(BooleanLiteral* blit = dynamic_cast<BooleanLiteral*>(exp)){
        return new BooleanLiteral(*blit);
    }else if(StringLiteral* slit = dynamic_cast<StringLiteral*>(exp)){
        return new StringLiteral(*slit);
    }else if(Identifier* ident = dynamic_cast<Identifier*>(exp)){
        return new Identifier(*ident);
    }else if(ArrayLiteral* alit = dynamic_cast<ArrayLiteral*>(exp)){
        return new ArrayLiteral(*alit);
    }else if(IndexExpression* idxlit = dynamic_cast<IndexExpression*>(exp)){
        return new IndexExpression(*idxlit);
    }else if(HashLiteral* hlit = dynamic_cast<HashLiteral*>(exp)){
        return new HashLiteral(*hlit);
    }else if(WhileExpression* wExp = dynamic_cast<WhileExpression*>(exp))
        return new WhileExpression(*wExp);
    return nullptr;
}

#endif
