#include <cstdlib>
#include <cmath>
#include "mmm.hpp"

#include <iostream>

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
    void * TopDownStack::_alloc(size sz, size alignment){
        sz += sizeof(size);

        UCHAR *old = curr;
        curr = reinterpret_cast<UCHAR*>(align(reinterpret_cast<size>(curr), alignment));
        UCHAR offset = curr - old;


        if(fsize < (sz + offset)) return nullptr;
        if(offset){  
            *(reinterpret_cast<char*>(curr) - 1) = offset;
        }
        
        old = curr;
        curr += sz;
        fsize -= (sz + offset);

        *(reinterpret_cast<size*>(curr) - 1) = sz | (!!offset);

        return old;
    }

    void TopDownStack::_free(void *&mem){
        (void)mem;  //silence unused variable compiler warning

        size gain = *(reinterpret_cast<size*>(curr) - 1);

        if(gain & 1){        //have to typecast like this for little endian processors
            gain += static_cast<UCHAR>(*reinterpret_cast<size*>(curr - gain));
            gain &= ~1;
        }

        curr -= gain;
        fsize += gain;

    }

    //BottomUpStack
    void * BottomUpStack::_alloc(size sz, size alignment){
        sz = balign(sz, alignment) + sizeof(size);

        if(fsize < sz)  return nullptr;

        curr -= sz;
        fsize -= sz;

        *reinterpret_cast<size*>(curr) = sz;
        return (reinterpret_cast<size*>(curr)+1);
    }

    void BottomUpStack::_free(void *&mem){
        (void)mem;  //silence unused variable compiler warning

        size gain = *reinterpret_cast<size*>(curr);

        curr += gain;
        fsize += gain;
    }

    //doubleStack
    void * DoubleStack::_alloc(size sz, size alignment){
        return (sz < ~sz) ? TopDownStack::_alloc(sz, alignment) : BottomUpStack::_alloc(~sz + 1, alignment);
    }

    void DoubleStack::_free(void *&mem){
        if(&mem == &TOP)            TopDownStack::_free(NONE);
        else if(&mem == &BOTTOM)    BottomUpStack::_free(NONE);
    }

    //FixedPage
    FixedPage::FixedPage(size sz, size cs, size ps): 
    Mmm(sz, cs), 
    pageSize(ps), 
    pageTableSize(ceil((sz - sizeof(FixedPage))/(8.0*ps + 1.0))),
    numPages((sz - sizeof(FixedPage)/pageTableSize)/ps){}

    void *FixedPage::_alloc(size sz, size alignment){

    }
    void FixedPage::_free(void *&mem){

    }
}