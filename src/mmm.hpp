#ifndef MMM_HEADER
#define MMM_HEADER

#include <cstddef> 

enum class MmmType{
    singleStack
};

class Mmm {

    friend Mmm * createMmm(MmmType, size_t);

    static Mmm* create(MmmType, size_t);

    template<typename T> inline T* alloc(){ return static_cast<T*>(_alloc(sizeof(T))); }
    inline void* alloc(size_t sz){ return _alloc(sz); }
    
    virtual void * _alloc(size_t) = 0;

    void * buffer;

};

class MmmSingleStack : public Mmm {
    void* _alloc(size_t) final;
};


//client
/*class MmmEngine{
    public:
    inline void operator()(MmmType type){M = Mmm::create(type);}

    private:
    Mmm *M;
};*/

inline Mmm * createMmm(MmmType type, size_t sz = 0){
    return Mmm::create(type, sz);
}


#endif