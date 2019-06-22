#include <iostream>
#include "mmm.hpp"
using namespace mmm;

#define Stack DoubleStack  //for testing

inline void printStats(Mmm * m){
    std::cout << "free: " << m->getFreeSize() << std::endl;
    std::cout << "used: " << m->getUsedSize() << std::endl << std::endl;
}

int main(int argc, char **argv){

    std::cout << "init:" << std::endl;
    Mmm *m = Mmm::create(MmmType::Stack, 1024);
    printStats(m);

    std::cout << "alloc: " << std::endl;
    m->alloc<int>(1); 
    m->alloc<int*>(1); 
    printStats(m);

    std::cout << "alloc: " << std::endl;
    m->alloc<char>(-504);
    printStats(m);

    std::cout << "free: " << std::endl;
    m->free(TOP);
    m->free(TOP);
    printStats(m);

    std::cout << "free: " << std::endl;
    m->free(BOTTOM);
    printStats(m);

    return 0;
}