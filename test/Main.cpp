#include <iostream>
#include "mmm.hpp"
using namespace mmm;

#define Stack FixedPageStack  //for testing

int main(int argc, char **argv){


    void *mem = malloc(8192);
    Mmm *m = Mmm::createPool(MmmPoolType::FixedPage, 8192, mem);

    void *t1 = m->alloc();
    void *t2 = m->alloc();
    m->free(t1);
    m->free(t2);
    void *t3 = m->alloc();
    void *t4 = m->alloc();

    return 0;
}