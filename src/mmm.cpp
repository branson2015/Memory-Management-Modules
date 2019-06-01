#include <cstdlib>
#include <iostream>
#include "mmm.hpp"

Mmm * Mmm::create(MmmType type, size_t sz){
    Mmm *m;
    switch(type){
        case MmmType::singleStack:
            m =  new MmmSingleStack;
            break;
        default: 
            return nullptr;
    }
    
    m->buffer= malloc(sz);
    return m;
}