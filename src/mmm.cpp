#include <cstdlib>
#include <iostream>
#include "mmm.hpp"

namespace mmm{

    void *TOP = nullptr;
    void *BOTTOM = nullptr;
    void *NONE = nullptr;

    Mmm::size Mmm::getClassSize(MmmType type){
        switch(type){
            case MmmType::SingleStack:
            case MmmType::TopDownStack:
                return sizeof(TopDownStack) + sizeof(size);
            case MmmType::BottomUpStack:
                return sizeof(BottomUpStack) + sizeof(size);
            case MmmType::DoubleStack:
                return sizeof(DoubleStack) + 2*sizeof(size);
            default: return 0;
        }
    }

    //Base
    Mmm *Mmm::create(MmmType type, size sz, void *memory){
        if(memory == nullptr){
            sz += Mmm::getClassSize(type);
            memory = malloc(sz);
            //memcpy to 0 for debug so debugging is easier
        }
        std::cout << (size)memory << std::endl;
        switch(type){
            case MmmType::SingleStack:
            case MmmType::TopDownStack:
                return new(memory) TopDownStack(sz);
            case MmmType::BottomUpStack:
                return new(memory) BottomUpStack(sz);
            case MmmType::DoubleStack:
                return new(memory) DoubleStack(sz);
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
        //works for both signed and unsigned types
        return ((sz > ~sz) || (sz < 0)) ? BottomUpStack::_alloc(sz) : TopDownStack::_alloc(sz);
    }

    void DoubleStack::_free(void *&mem){
        if(&mem == &TOP){
            TopDownStack::_free(NONE);
        }else if(&mem == &BOTTOM){
            BottomUpStack::_free(NONE);
        }
    }
}