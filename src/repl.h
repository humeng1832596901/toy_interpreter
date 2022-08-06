/*******************************************************************
	> File Name: repl.h
	> Mail: 13535324513@163.com
	> Created Time: Tue 26 Jul 2022 01:21:54 PM CST
 ******************************************************************/

#ifndef _REPL_H
#define _REPL_H

#include <iostream>
#include <memory>
#include "parser/parser.hpp"
#include "evaluator/evaluator.hpp"
#include "builtin/builtin.h"

const std::string PROMPT = ">> ";

const std::string MONKEY_FACE = "            __,__\n"
"   .--.  .-"     "-.  .--.\n"
"  / .. \\/  .-. .-.  \\/ .. \\\n"
" | |  '|  /   Y   \\  |'  | |\n"
" | \\   \\  \\ 0 | 0 /  /   / |\n"
"  \\ '- ,\\.-\"\"\"\"\"\"\"-./, -' /\n"
"''-' /_   ^ ^   _\\ '-''\n"
"       |  \\._   _./  |\n"
"       \\   \\ '~' /   /\n"
"        '._ '-=-' _.'\n"
"           '-----'\n";

void Start(std::istream& in, std::ostream& out){
    char buf[1024];
    Environment env;
    initBuiltin();  //初始化内置函数
    for(;;){
        out << PROMPT;
        in.getline(buf, 1024);
        Lexer lex(buf); 
        Parser p(lex);
        auto program = p.ParseProgram();
        std::string error = p.Errors();
        if(!error.empty()){
            std::cout << MONKEY_FACE 
                      << "Woops! We ran into some monkey business here!\nparser errors:\n" 
                      << error 
                      << std::endl;
        }else{
            std::shared_ptr<Object> evaluated = Eval(&program, &env);
            if(evaluated == exitObject){
                std::cout << "bye" << std::endl;
                break;
            }
            if(evaluated != nullObject)
                std::cout << evaluated->Inspect() << std::endl;
        }
    }
}

#endif
