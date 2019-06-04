#ifndef MMM_HEADER
#define MMM_HEADER

#include <cstddef> 

enum class MmmType{
    singleStack,
    doubleStack
};

class Mmm {
    public:
    
    static Mmm *create(MmmType, void*, const size_t);
    inline static Mmm *create(MmmType type, const size_t sz){ return Mmm::create(type, malloc(sz), sz); }

    template<typename T> inline T *alloc(const int copies = 1){ return static_cast<T*>(_alloc(sizeof(T)*copies)); }
    inline void *alloc(const size_t sz, const int copies = 1){ return _alloc(sz*copies); }
    inline void free(void *m = nullptr){ _free(m); }


    inline size_t getSize(){ return bsize; }
    inline size_t getFreeSize(){ return fsize; }
    inline size_t getUsedSize(){ return bsize - fsize; }

    protected:
    public:
    
    inline Mmm(size_t bsize, int classSize): buffer(reinterpret_cast<char*>(this) + classSize), bsize(bsize), fsize(bsize){}
    inline void *operator new(size_t obj, size_t size, char){ return malloc(obj + size); }

    const size_t ALIGN = 0xF;   //can only align by powers of 2
    inline size_t align(size_t sz, size_t align){ return (sz + align-1) & ~(align); }

    virtual void * _alloc(size_t) = 0;
    virtual void _free(void*) = 0;

    char *buffer;
    size_t bsize, fsize;
};



class MmmSingleStack : public Mmm {
    public:

    inline MmmSingleStack(size_t sz): Mmm(sz, sizeof(*this)), curr(buffer+sizeof(size_t)){
        *reinterpret_cast<size_t*>(buffer) = 0;
    }

    void* _alloc(size_t) final;
    void _free(void*) final;

    //private:
    char *curr;
};

#endif