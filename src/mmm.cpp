#include <cstdlib>
#include <iostream>
#include "mmm.hpp"

//Base
Mmm *Mmm::create(MmmType type, void *memory, size_t sz){
    char *mem = reinterpret_cast<char*>(memory);
    
    if(mem == nullptr || sz <= 0){
        //put error here
    }

    //do something with placement new here

    switch(type){
        case MmmType::singleStack:
            return new(sz, 0) BottomUpStack(sz);
        case MmmType::doubleStack:
            return new(sz, 0) MmmDoubleStack(sz);
        default: 
            return nullptr;
    }
}

//TopDownStack
void * TopDownStack::_alloc(size_t sz){
    sz = align(sz + sizeof(size_t), ALIGN);

    if(fsize < sz)  return nullptr; //error, not enough space, realloc?
    
    curr = curr + sz;
    *reinterpret_cast<size_t*>(curr - sizeof(size_t)) = sz;
    
    fsize -= sz;
    
    return curr;
}

void TopDownStack::_free(void *mem){

    size_t gain = *reinterpret_cast<size_t*>(curr - sizeof(size_t));

    curr = curr - gain;
    fsize += gain;
}

//BottomUpStack
void * BottomUpStack::_alloc(size_t sz){
    /*  REDO THIS CODE
    sz = align(sz + sizeof(size_t), ALIGN);

    if(fsize < sz)  return nullptr; //error, not enough space, realloc?
    
    curr = curr + sz;
    *reinterpret_cast<size_t*>(curr - sizeof(size_t)) = sz;
    
    fsize -= sz;
    
    return curr;*/
}

void BottomUpStack::_free(void *mem){
    /*  REDO THIS CODE
    size_t gain = *reinterpret_cast<size_t*>(curr - sizeof(size_t));

    curr = curr - gain;
    fsize += gain;*/
}

//doubleStack
void * MmmDoubleStack::_alloc(size_t sz){
    //do bound checking here for the two currs and make sure they don't cross
    //OR: have one class change the other class' bsize
    if(sz > 0)  return TopDownStack::_alloc(sz);
    else        return BottomUpStack::_alloc(sz);
    
}

void MmmDoubleStack::_free(void *mem){
    //if mem == TOP?
    //if mem == BOTTOM?
    //take advantage of type system??
    //actually pass pointer to obj you want to free?
    //why not both?
}