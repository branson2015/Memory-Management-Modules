#include <cstdlib>
#include <iostream>
#include "mmm.hpp"

Mmm *Mmm::create(MmmType type, void *memory, size_t sz){
    char *mem = reinterpret_cast<char*>(memory);
    
    if(mem == nullptr || sz <= 0){
        //put error here
    }
    
    Mmm *m;
    switch(type){
        case MmmType::singleStack:
            m = new(sz, 0) MmmSingleStack(sz);    //allocates different memory for memory manager, should be allocated in it's own pool?
    }

    return m;
}

void * MmmSingleStack::_alloc(size_t sz){
    sz += sizeof(size_t);
    sz = align(sz, ALIGN);

    if(fsize < sz){
        //error, not enough space, realloc?
        return nullptr;
    }
    
    
    curr = curr + *reinterpret_cast<size_t*>(curr);
    *reinterpret_cast<size_t*>(curr) = sz;
    
    fsize -= sz;
    
    return curr + sizeof(size_t);
}

void MmmSingleStack::_free(void *mem){
    //error here if mem is not nullptr?

    curr = curr - sizeof(size_t);

    if(curr == buffer){
        //error, everything already freed
        return;
    }

    size_t gain = *reinterpret_cast<size_t*>(curr);
    curr = curr - gain;
    fsize += gain;

    return;
}