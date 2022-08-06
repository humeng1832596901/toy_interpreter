/*******************************************************************
	> File Name: main.cpp
	> Author: 胡孟
	> Mail: 13535324513@163.com
	> Created Time: Mon 25 Jul 2022 09:42:11 PM CST
 ******************************************************************/

#include "repl.h"

int main(int argc, char** argv){
    std::cout << "Hello! Try my toy interpreter!" << std::endl;
    Start(std::cin, std::cout);
    return 0;
}
