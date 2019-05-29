#include <iostream>
#include "mmm.hpp"

int main(int argc, char **argv){

    std::cout << "hello world!" << std::endl;

    std::string t;
    std::cin >> t;
    std::cout << "this is a test!" << std::endl;

    for(int i = 0; i < 1000; ++i){
        std::cout << i << std::endl;
    }

    std::cout << "done!" << std::endl;

    return 0;
}