#ifndef _vm_
#define _vm_

#include "code.hpp"
#include "parser/parser.hpp"
#include "ast/ast.hpp"
#include "object/object.h"
#include <stack>
// #include "lexer/lexer.hpp"

// const size_t StackSize = 2048;

struct VM{
    std::vector<std::shared_ptr<Object>> constants;
    Instructions instructions;
    std::stack<std::shared_ptr<Object>> sta;

    void Run(){
        for(size_t ip=0; ip<instructions.size(); ++ip){
            unsigned char op = instructions[ip];
            switch(op){
                case (unsigned char)(Opcodes::OpAdd) : {
                    std::shared_ptr<Object> left = sta.top();
                    sta.pop();
                    std::shared_ptr<Object> right = sta.top();
                    sta.pop();
                    if(left->Type() != right->Type()){
                        //error
                    }else{
                        if(left->Type() == ObjectType::INTEGER_OBJ){
                            sta.push(std::make_shared<IntegerObject>(
                                std::dynamic_pointer_cast<IntegerObject>(left)->Value + std::dynamic_pointer_cast<IntegerObject>(right)->Value
                            ));
                        }
                    }
                    break;
                }
            }
        }
    }
    // int sp = 0;
};

Program parse(const std::string codes){
    Lexer lex(codes); 
    Parser p(lex);
    Program program = p.ParseProgram();
}

#endif