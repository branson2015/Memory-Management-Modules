#include <cstdlib>
#include <iostream>
#include "mmm.hpp"

namespace mmm{

    void *TOP = nullptr;
    void *BOTTOM = nullptr;
    void *NONE = nullptr;

    //Base
    Mmm *Mmm::create(MmmType type, size sz, void *memory){
        switch(type){
            case MmmType::SingleStack:
            case MmmType::TopDownStack:
                return new(memory, sz, sizeof(size)) TopDownStack(sz);
            case MmmType::BottomUpStack:
                return new(memory, sz, sizeof(size)) BottomUpStack(sz);
            case MmmType::DoubleStack:
                return new(memory, sz, sizeof(size)*2) DoubleStack(sz);
            default: 
                return nullptr;
        }
    }

    //TopDownStack
    void * TopDownStack::_alloc(size sz){
        sz += sizeof(size);

        if(fsize < sz)  return nullptr;
        
        curr += sz;
        fsize -= sz;

        *reinterpret_cast<size*>(curr - sizeof(size)) = sz;
        
        return curr;
    }

    void TopDownStack::_free(void *&mem){
        (void)mem;  //silence unused variable compiler warning

        size gain = *reinterpret_cast<size*>(curr - sizeof(size));

        curr -= gain;
        fsize += gain;
    }

    //BottomUpStack
    void * BottomUpStack::_alloc(size sz){
        sz += sizeof(size);

        if(fsize < sz)  return nullptr;
        
        curr -= sz;
        fsize -= sz;

        *reinterpret_cast<size*>(curr) = sz;
        
        return reinterpret_cast<size*>(curr) + 1;
    }

    void BottomUpStack::_free(void *&mem){
        (void)mem;  //silence unused variable compiler warning

        size gain = *reinterpret_cast<size*>(curr);

        curr += gain;
        fsize += gain;
    }

    //doubleStack
    void * DoubleStack::_alloc(size sz){
        return (sz < ~sz) ? TopDownStack::_alloc(sz) : BottomUpStack::_alloc(~sz + 1);
    }

    void DoubleStack::_free(void *&mem){
        if(&mem == &TOP)            TopDownStack::_free(NONE);
        else if(&mem == &BOTTOM)    BottomUpStack::_free(NONE);
    }
}