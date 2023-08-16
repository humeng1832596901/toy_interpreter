#include "repl.h"

int main(int argc, char** argv){
    std::cout << "Hello! Try my toy interpreter!" << std::endl;
    Start(std::cin, std::cout);
    return 0;
}
