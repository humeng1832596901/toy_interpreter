/*******************************************************************
	> File Name: parser.hpp
	> Mail: 13535324513@163.com
	> Created Time: Tue 26 Jul 2022 02:22:02 PM CST
 ******************************************************************/

#ifndef _PARSER_H
#define _PARSER_H

#include "../lexer/lexer.hpp"
#include "../ast/ast.hpp"
#include <functional>
#include <iostream> //test

std::unordered_map<std::string, int> precedences = {
    {EQUAL, operators::EQUALS},
    {NOT_EQUAL, operators::EQUALS},
    {LT, operators::LESSGREATER},
    {GT, operators::LESSGREATER},
    {PLUS, operators::SUM},
    {MINUS, operators::SUM},
    {SLASH, operators::PRODUCT},
    {ASTERISK, operators::PRODUCT},
    {LPAREN, operators::CALL},
    {LBRACKET, operators::INDEX}
};

struct Parser{
    using prefixParseFn = std::function<Expression*(void)>;
    using infixParseFn = std::function<Expression*(Expression*)>; 
    
    Parser(const Lexer& lexer)
        : lex(lexer)
    {
        nextToken();
        nextToken();
        //语法糖，注册成员函数
        auto pIDENT = [&](){
            return parseIdentifier();  
        };
        auto pINT = [&](){
            return parseIntegerLiteral();
        };
        auto pDOUBLE = [&](){
            return parseDoubleLiteral();
        };
        auto pSTR = [&](){
            return parseStringLiteral();
        };
        auto pBOOL = [&](){
            return parseBooleanLiteral();
        };
        auto pPAREN = [&](){
            return parseGroupedExpression();
        };
        auto pARRAY = [&](){
            return parseArrayLiteral();
        };
        auto pHash = [&](){
            return parseHashLiteral();
        };
        auto pIF = [&](){
            return parseIfExpression();
        };
        auto pWHILE = [&](){
            return parseWhileExpression();
        };
        auto pFUNC = [&](){
            return parseFunctionLiteral();
        };
        auto pPRE = [&](){
            return parsePrefixExpression();
        };
        auto pINDEX = [&](Expression* left){
            return parseIndexExpression(left);
        };
        auto pINF = [&](Expression* left){
            return parseInfixExpression(left);
        };
        auto pCALL = [&](Expression* function){
            return parseCallExpression(function);
        };
        registerPrefix(IDENT, pIDENT);
        registerPrefix(INT, pINT);
        registerPrefix(DOUBLE, pDOUBLE);
        registerPrefix(STRING, pSTR);
        registerPrefix(TRUE, pBOOL);
        registerPrefix(FALSE, pBOOL);
        registerPrefix(LPAREN, pPAREN);
        registerPrefix(LBRACKET, pARRAY);
        registerPrefix(BANG, pPRE);
        registerPrefix(MINUS, pPRE);
        registerPrefix(IF, pIF);
        registerPrefix(WHILE, pWHILE);
        registerPrefix(FUNCTION, pFUNC);
        registerPrefix(LBRACE, pHash);
        registerInfix(PLUS, pINF);
        registerInfix(MINUS, pINF);
        registerInfix(SLASH, pINF);
        registerInfix(ASTERISK, pINF);
        registerInfix(EQUAL, pINF);
        registerInfix(NOT_EQUAL, pINF);
        registerInfix(LT, pINF);
        registerInfix(GT, pINF);
        registerInfix(LPAREN, pCALL);
        registerInfix(LBRACKET, pINDEX);
    }

    void registerPrefix(const std::string& TokenType, const prefixParseFn& func){
        prefixParseFns[TokenType] = func;
    }

    void registerInfix(const std::string& TokenType, const infixParseFn& func){
        infixParseFns[TokenType] = func;
    }

    const std::string Errors()const;

    void nextToken(){
        curToken = peekToken;
        peekToken = lex.nextToken();
    }

    bool peekTokenIs(std::string TokenType){
        return peekToken.TokenType == TokenType;
    }

    bool curTokenIs(std::string TokenType){
        return curToken.TokenType == TokenType;
    }

    void noPrefixParseFnError(std::string TokenType){
        std::string msg = "line " + std::to_string(lex.line) + ": no prefix parse function for " + TokenType + " found";
        errors.emplace_back(msg);
    }

    void peekError(std::string TokenType){
        std::string msg = "line " + std::to_string(lex.line) + ": expected next token to be " + TokenType + ", got " + peekToken.TokenType + " instead";
        errors.emplace_back(msg);
    }

    bool expectPeek(std::string TokenType){
        if(peekTokenIs(TokenType)){
            nextToken();
            return true;
        }
        peekError(TokenType);
        return false;
    }

    int curPrecedence(){
        if(precedences.find(curToken.TokenType) != precedences.end())
            return precedences[curToken.TokenType];
        return operators::LOWEST;
    }

    int peekPrecedence(){
        if(precedences.find(peekToken.TokenType) != precedences.end())
            return precedences[peekToken.TokenType];
        return operators::LOWEST;
    }

    Expression* parseBooleanLiteral(){
        return new BooleanLiteral(curToken, curTokenIs(TRUE));
    }

    Expression* parseIntegerLiteral(){
        IntegerLiteral* intLitPtr = new IntegerLiteral(curToken);
        int64_t val = atoi(curToken.Literal.c_str());
        intLitPtr->Value = val;
        if(errno != 0){
            std::string msg = "line " + std::to_string(lex.line) + ": could not parse " + curToken.Literal + " as integer";
            errors.emplace_back(msg);
            delete intLitPtr;
            return nullptr;
        }
        return intLitPtr;
    }

    Expression* parseDoubleLiteral(){
        DoubleLiteral* doubleLitPtr = new DoubleLiteral(curToken);
        double val = atof(curToken.Literal.c_str());
        doubleLitPtr->Value = val;
        if(errno != 0){
            std::string msg = "line " + std::to_string(lex.line) + ": could not parse " + curToken.Literal + " as double";
            errors.emplace_back(msg);
            delete doubleLitPtr;
            return nullptr;
        }
        return doubleLitPtr;
    }

    Expression* parseStringLiteral(){
        return new StringLiteral(curToken, curToken.Literal);
    }

    Expression* parseArrayLiteral(){
        ArrayLiteral* alit = new ArrayLiteral(curToken);
        if(not parseExpressionList(alit->Elements, RBRACKET)){
            delete alit;
            return nullptr;
        }
        return alit;
    }

    Expression* parseIndexExpression(Expression* Left){
        IndexExpression* idxExp = new IndexExpression(curToken, Left);
        nextToken();
        idxExp->Index = parseExpression(operators::LOWEST);
        if(not expectPeek(RBRACKET)){
            delete idxExp;
            return nullptr;
        }
        return idxExp;
    }

    Expression* parseHashLiteral(){
        HashLiteral* hExp = new HashLiteral(curToken);  //{
        Expression *key=nullptr, *value=nullptr;
        for(; not peekTokenIs(RBRACE); ){
            nextToken();
            key = parseExpression(operators::LOWEST);
            if(!key || not expectPeek(COLON)){   //如果下一个token不是冒号
                delete hExp;
                return nullptr;
            }
            nextToken();
            value = parseExpression(operators::LOWEST);
            if(!value){
                delete hExp;
                return nullptr;
            }
            hExp->Pairs.insert({key, value});
            if(not peekTokenIs(RBRACE) && not expectPeek(COMMA)){  //如果下一个token既不是}也不是,
                delete hExp;
                return nullptr;
            }
        }
        if(not expectPeek(RBRACE)){
            delete hExp;
            return nullptr;
        }
        return hExp;
    }

    Expression* parseIdentifier(){
        Expression* iPtr = new Identifier(curToken, curToken.Literal);
        return iPtr;
    }

    Expression* parseWhileExpression(){
        WhileExpression* exp = new WhileExpression(curToken);
        if(not expectPeek(LPAREN)){
            delete exp;
            return nullptr;
        }
        nextToken();
        exp->Condition = parseExpression(operators::LOWEST);
        if(not expectPeek(RPAREN)){
            delete exp;
            return nullptr;
        }
        if(not expectPeek(LBRACE)){
            delete exp;
            return nullptr;
        }
        exp->Consequence = parseBlockStatement();
        return exp;
    }

    Expression* parseIfExpression(){
        IfExpression* exp = new IfExpression(curToken);
        if(not expectPeek(LPAREN)){
            delete exp;
            return nullptr;
        }
        nextToken();
        exp->Condition = parseExpression(operators::LOWEST);
        if(not expectPeek(RPAREN)){
            delete exp;
            return nullptr;
        }
        if(not expectPeek(LBRACE)){
            delete exp;
            return nullptr;
        }
        exp->Consequence = parseBlockStatement();
        //检查else子结构是否存在
        if(peekTokenIs(ELSE)){
            nextToken();
            if(not expectPeek(LBRACE)){
                delete exp;
                return nullptr;
            }
            exp->Alternative = parseBlockStatement();
        }

        return exp;
    }

    Expression* parseFunctionLiteral(){
        FunctionLiteral* lit = new FunctionLiteral(curToken);
        if(not expectPeek(LPAREN)){
            delete lit;
            return nullptr;
        }
        if(not parseFunctionParameters(lit->Parameters)){
            delete lit;
            return  nullptr;
        }
        if(not expectPeek(LBRACE)){
            delete lit;
            return nullptr;
        }
        lit->Body = parseBlockStatement();
        return lit;
    }

    Expression* parseCallExpression(Expression* function){
        CallExpression* exp = new CallExpression(curToken, function);
        if(not parseExpressionList(exp->Arguments, RPAREN)){
            delete exp;
            return nullptr;
        }
        return exp;
    }

    bool parseExpressionList(std::vector<Expression*>& Arguments, const std::string& end){
        if(peekTokenIs(end)){
            nextToken();
            return true;
        }
        nextToken();
        Expression* exp = parseExpression(operators::LOWEST);
        Arguments.push_back(exp);
        for(; peekTokenIs(COMMA);){
            nextToken();
            nextToken();
            exp = parseExpression(operators::LOWEST);
            Arguments.push_back(exp);
        }
        if(not expectPeek(end))
            return false;
        return true;
    }

    
    bool parseFunctionParameters(std::vector<Identifier*>& Parameters){
        if(peekTokenIs(RPAREN)){   //无参函数
            nextToken();
            return true;
        }
        nextToken();
        Identifier* identPtr = new Identifier(curToken, curToken.Literal);
        Parameters.push_back(identPtr);
        for(; peekTokenIs(COMMA);){
            nextToken();
            nextToken();
            identPtr = new Identifier(curToken, curToken.Literal);
            Parameters.push_back(identPtr);
        }
        if(not expectPeek(RPAREN))
            return false;
        return true;
    }

    Expression* parsePrefixExpression(){
        PrefixExpression* pExpPtr = new PrefixExpression(curToken, curToken.Literal);
        nextToken();
        Expression* right = parseExpression(operators::PREFIX);
        if(right)
            pExpPtr->Right = right;
        return pExpPtr;
    }

    Expression* parseInfixExpression(Expression* left){
        InfixExpression* iExpPtr = new InfixExpression(curToken, curToken.Literal, left);
        int precedence = curPrecedence();
        nextToken();
        Expression* right = parseExpression(precedence);
        if(right)
            iExpPtr->Right = right;
            //这边如果失败再写错误信息
        return iExpPtr;
    }

    Expression* parseGroupedExpression(){   //对表达式中的括号进行优先级提升
        nextToken();
        Expression* exp = parseExpression(operators::LOWEST);
        if(not expectPeek(RPAREN))
            return nullptr;
        return exp;
    }

    Expression* parseExpression(const int precedence){
        if(prefixParseFns.find(curToken.TokenType) == prefixParseFns.end()){
            noPrefixParseFnError(curToken.TokenType);
            return nullptr;
        }
        Expression* expPtr = prefixParseFns[curToken.TokenType]();
        for(; not peekTokenIs(SEMICOLON) && precedence < peekPrecedence(); ){
            if(infixParseFns.find(peekToken.TokenType) == infixParseFns.end())
                return expPtr;
            nextToken();
            expPtr = infixParseFns[curToken.TokenType](expPtr);
        }
        return expPtr;
    }

    Statement* parseLetStatement();
    Statement* parseReturnStatement();
    Statement* parseBlockStatement();
    Statement* parseAssignStatement();
    Statement* parseExpressionStatement();
    Statement* parseIndexAssignStatement(Expression* exp);
    Statement* parseAssignStatementRight(AssignStatement* stmt);

    Statement* parseStatement(){
        if(curToken.TokenType == LET)
            return parseLetStatement();
        else if(curToken.TokenType == RETURN)
            return parseReturnStatement();
        return parseExpressionStatement();
    }

    Program ParseProgram(){  
        Program program; 
        for(;curToken.TokenType != END; nextToken()){
	    Statement* res = parseStatement();
	    if(!res)
		break;
            program.Statements.emplace_back(parseStatement());
        }
        return program;
    }

    Lexer lex;
    Token curToken;
    Token peekToken;
    std::vector<std::string> errors;

    std::unordered_map<std::string, prefixParseFn> prefixParseFns;
    std::unordered_map<std::string, infixParseFn> infixParseFns;
};

const std::string Parser::Errors() const {
        if(errors.empty())
            return "";
        std::string err;
        int size = errors.size();
        for(int i=0; i<size; ++i){
            if(i==size-1)
                err += errors[i];
            else
                err += errors[i]+"\n";
        }
        return err;

}

//解析let语句
Statement* Parser::parseLetStatement(){
    LetStatement* stmtPtr = new LetStatement(curToken);
    if(not expectPeek(IDENT)){   //expectPeek函数有对token进行移位
        //报错
        delete stmtPtr;
        return nullptr;
    }
    stmtPtr->Name = Identifier(curToken, curToken.Literal);
    if(not expectPeek(ASSIGN)){
        delete stmtPtr;
        return nullptr;
    }
    nextToken();  //跳过ASSIGN
    Expression* val = parseExpression(operators::LOWEST);
    if(val)
        stmtPtr->Value = val;
    else{
        delete stmtPtr;
        return nullptr;
    }
    if(peekTokenIs(SEMICOLON))
        nextToken();
    return stmtPtr;
}

//解析return语句
Statement* Parser::parseReturnStatement(){
    ReturnStatement* stmtPtr = new ReturnStatement(curToken);
    nextToken();
    Expression* val = parseExpression(operators::LOWEST);
    if(val)
        stmtPtr->Value = val;
    else{
        delete stmtPtr;
        return nullptr;
    }
    if(peekTokenIs(SEMICOLON))
        nextToken();
    return stmtPtr;
}

//解析块语句
Statement* Parser::parseBlockStatement(){
    BlockStatement* blockPtr = new BlockStatement(curToken);
    nextToken();
    for(;not curTokenIs(RBRACE) && not curTokenIs(END); nextToken()){
        Statement* stmtPtr = parseStatement();
        if(stmtPtr)
            blockPtr->Statements.push_back(stmtPtr);
        else{
            //Todo解析失败要报错
            return nullptr;
        }
    }
    return blockPtr;
}

//解析赋值语句中的右值
Statement* Parser::parseAssignStatementRight(AssignStatement* stmtPtr){
    if(not expectPeek(ASSIGN)){
        delete stmtPtr;
        return nullptr;
    }
    nextToken();  //跳过ASSIGN
    Expression* val = parseExpression(operators::LOWEST);
    if(val)
        stmtPtr->Value = val;
    else{
        delete stmtPtr;
        return nullptr;
    }
    if(peekTokenIs(SEMICOLON))
        nextToken();
    return stmtPtr;
}

//解析普通赋值语句
Statement* Parser::parseAssignStatement(){
    AssignStatement* stmtPtr = new AssignStatement(curToken);
    if(not curTokenIs(IDENT)){   
        //报错
        delete stmtPtr;
        return nullptr;
    }
    stmtPtr->Name = new Identifier(curToken, curToken.Literal);
    return parseAssignStatementRight(stmtPtr);
}

//解析hash/array赋值语句
Statement* Parser::parseIndexAssignStatement(Expression* exp){
    AssignStatement* stmtPtr = new AssignStatement(curToken);
    stmtPtr->Name = exp;
    return parseAssignStatementRight(stmtPtr);
}

//解析表达式语句
Statement* Parser::parseExpressionStatement(){
    if(peekTokenIs(ASSIGN))
        return parseAssignStatement();
    Expression* exp = parseExpression(operators::LOWEST);
    if(exp){
        if(dynamic_cast<IndexExpression*>(exp) && peekTokenIs(ASSIGN)){
            return parseIndexAssignStatement(exp);
        }
        ExpressionStatement* stmtPtr = new ExpressionStatement(curToken);
        stmtPtr->Exp = exp;
        if(peekTokenIs(SEMICOLON))
            nextToken();  
        return stmtPtr;
    }
    return nullptr;
}

#endif
