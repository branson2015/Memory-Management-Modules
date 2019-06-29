#include <cstdlib>
#include <cmath>
#include "mmm.hpp"

#include <iostream>

namespace mmm{

    //Base
    Mmm *Mmm::createStack(MmmStackType type, size sz, void *memory){
        switch(type){
            case MmmStackType::SingleStack:
            case MmmStackType::TopDownStack:
                return new(memory, sz, sizeof(size)) TopDownStack(sz);
            case MmmStackType::BottomUpStack:
                return new(memory, sz, sizeof(size)) BottomUpStack(sz);
            case MmmStackType::DoubleStack:
                return new(memory, sz, sizeof(size)*2) DoubleStack(sz);
            default: 
                return nullptr;
        }
    }


    Mmm *Mmm::createPool(MmmPoolType type, size sz, void *memory){
        switch(type){
            case MmmPoolType::FixedPage:
                return new(memory, sz, 0) FixedPage(sz);
            default:
                return nullptr;
        }
    }

    //TopDownStack
    void * TopDownStack::_alloc(size sz, size alignment){
        sz += sizeof(size);

        size psz = *(reinterpret_cast<size*>(curr) - 1);
        size offset = reinterpret_cast<UCHAR*>(align(reinterpret_cast<size>(curr), alignment)) - curr;
        
        sz += offset;
        if(fsize < sz) return nullptr;        
        
        UCHAR *rtn = curr + offset;
        *(reinterpret_cast<size*>(rtn) - 1) = psz + offset;
        
        curr += sz;
        fsize -= sz;

        *(reinterpret_cast<size*>(curr) - 1) = sz - offset;

        return rtn;
    }

    void TopDownStack::_free(void *mem){
        (void)mem;

        size gain = *(reinterpret_cast<size*>(curr) - 1);

        curr -= gain;
        fsize += gain;
    }

    //BottomUpStack
    void * BottomUpStack::_alloc(size sz, size alignment){
        sz = curr - reinterpret_cast<UCHAR*>(balign(reinterpret_cast<size>(curr) - sz, alignment)) + sizeof(size);

        if(fsize < sz)  return nullptr;

        curr -= sz;
        fsize -= sz;

        *reinterpret_cast<size*>(curr) = sz;
        return (reinterpret_cast<size*>(curr)+1);
    }

    void BottomUpStack::_free(void *mem){
        (void)mem;

        size gain = *reinterpret_cast<size*>(curr);

        curr += gain;
        fsize += gain;
    }

    //doubleStack
    void * DoubleStack::_alloc(size sz, size alignment){
        return (sz < ~sz) ? TopDownStack::_alloc(sz, alignment) : BottomUpStack::_alloc(~sz + 1, alignment);
    }

    void DoubleStack::_free(void *mem){
        if(*reinterpret_cast<const char*>(mem) == *reinterpret_cast<const char*>(TOP))            TopDownStack::_free(NONE);
        else if(*reinterpret_cast<const char*>(mem) == *reinterpret_cast<const char*>(BOTTOM))    BottomUpStack::_free(NONE);
    }

    //fixedPage
    FixedPage::FixedPage(size sz, size cs, size ps): 
    Mmm(sz, cs), 
    pageSize(align(ps)),
    numPages(balign(sz-cs, alignment)/ps), //balign here? or align off of actual mem locations?
    pagesInUse(0){
        freeHead = (reinterpret_cast<UCHAR*>(align(reinterpret_cast<size>(this) + cs, alignment)));
        freeFoot = freeHead;
    }

    //should sz refer to number of pages or object size?
    void *FixedPage::_alloc(size sz, size alignment){
        (void)sz;
        (void)alignment;

        if(pagesInUse == numPages)  return nullptr;
        ++pagesInUse;

        void *rtn = freeHead;

        size bitmask = -static_cast<size>(freeHead == freeFoot);
        freeFoot += pageSize & bitmask;
        freeHead = reinterpret_cast<UCHAR*>((*reinterpret_cast<size*>(freeHead) & ~bitmask) | (reinterpret_cast<size>(freeFoot) & bitmask));

        return rtn;
    }

    void FixedPage::_free(void *mem){
        --pagesInUse;
        *reinterpret_cast<size*>(mem) = reinterpret_cast<size>(freeHead);
        freeHead = reinterpret_cast<UCHAR*>(mem);
    }
}