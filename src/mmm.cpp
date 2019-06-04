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

    if(fsize < sz)  return nullptr; //error, not enough space, realloc?
    
    curr = curr + sz;
    *reinterpret_cast<size_t*>(curr - sizeof(size_t)) = sz;
    
    fsize -= sz;
    
    return curr;
}

void MmmSingleStack::_free(void *mem){

    size_t gain = *reinterpret_cast<size_t*>(curr - sizeof(size_t));

    if(gain == 0)   return; //error, everything already freed

    curr = curr - gain;
    fsize += gain;

    return;
}