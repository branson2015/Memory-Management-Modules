#include <iostream>
#include "mmm.hpp"

//TODO: fix private/public used for testing

struct Test{
    int a, b;
    char c, d;

    inline void print(){ std::cout << a << " " << b << " " << +c << " " << +d << std::endl; }
};

inline void printStats(Mmm * m){
    std::cout << "buffer: " << (size_t)m->buffer << std::endl;
    std::cout << "curr:   " << (size_t)(dynamic_cast<MmmSingleStack*>(m)->curr) << std::endl;
    std::cout << "size: " << m->getSize() << std::endl;
    std::cout << "free: " << m->getFreeSize() << std::endl;
    std::cout << "used: " << m->getUsedSize() << std::endl << std::endl;
}

int main(int argc, char **argv){

    Mmm *m = Mmm::create(MmmType::singleStack, 1024);

    printStats(m);
    Test *t = m->alloc<Test>(4);
    t[0] = {1,2,3,4};
    t[1] = {5,6,7,8};

    t[0].print();
    t[1].print();

    printStats(m);

    Test *t2 = m->alloc<Test>();
    *t2 = {9, 10, 11, 12};
    t2->print();

    printStats(m);

    m->free();

    printStats(m);

    m->free();

    printStats(m);

    m->free();//error


    char b;
    std::cin >> b;

    return 0;
}

/*
    Mmm::create(type, location)
*/