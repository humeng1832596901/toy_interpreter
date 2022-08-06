/*******************************************************************
	> File Name: token.h
	> Author: 胡孟
	> Mail: 13535324513@163.com
	> Created Time: Mon 25 Jul 2022 05:33:37 PM CST
 ******************************************************************/

#ifndef _TOKEN_H
#define _TOKEN_H

#include <string>
#include <unordered_map>

#define ILLEGAL "ILLEGAL"
#define END "EOF"

//标识符+字面量
#define IDENT "IDENT"
#define INT "INT"
#define DOUBLE "DOUBLE"
#define STRING "STRING"

//运算符
#define ASSIGN "="
#define PLUS  "+"
#define MINUS "-"
#define BANG "!"
#define ASTERISK "*"
#define SLASH "/"
#define LT "<"
#define GT ">"
#define EQUAL "=="
#define NOT_EQUAL "!="

//分隔符
#define COMMA ","
#define SEMICOLON ";"
#define LPAREN "("
#define RPAREN ")"
#define LBRACE "{"
#define RBRACE "}"
#define LBRACKET "["
#define RBRACKET "]"
#define COLON ":"

//关键字
#define FUNCTION "FUNCTION"
#define LET "LET"
#define IDENT "IDENT"
#define TRUE "true"
#define FALSE "false"
#define RETURN "return"
#define IF "if"
#define ELSE "else"
#define WHILE "while"

//运算符
namespace operators{
    const int _ = 0;
    const int LOWEST = 1;
    const int EQUALS = 2; //==
    const int LESSGREATER = 3; //> or <
    const int SUM = 4; //+
    const int PRODUCT = 5; //*
    const int PREFIX = 6; //-X or !X
    const int CALL = 7; //func(X)
    const int INDEX = 8;
}

struct Token{
    Token() = default;
    Token(const std::string& type)
        : TokenType(type)
    {}

    Token(const std::string& type, const std::string& lit)
        : TokenType(type),
          Literal(lit)
    {}

    std::string TokenType;
    std::string Literal;
};

//关键词表
std::unordered_map<std::string,std::string> keywords = {
    {"fn", FUNCTION},
    {"let", LET},
    {"true", TRUE},
    {"false", FALSE},
    {"return", RETURN},
    {"if", IF},
    {"else", ELSE},
    {"while", WHILE}
};

std::string lookUpIdent(std::string ident){
    if(keywords.find(ident) != keywords.end())
        return keywords[ident];
    return IDENT;
}

#endif
