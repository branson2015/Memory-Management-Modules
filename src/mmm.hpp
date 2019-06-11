#ifndef MMM_HEADER
#define MMM_HEADER

#include <cstddef> 
#include <utility>

namespace mmm{
    extern void *TOP, *BOTTOM, *NONE;
    
    enum class MmmType{
        SingleStack,
        TopDownStack,
        BottomUpStack,
        DoubleStack
    };

    class Mmm {
        public:
        
        using size = size_t;    //can only be unsigned type
        static Mmm *create(MmmType, const size, void* = nullptr);

        template<typename T> inline T *alloc(const int copies = 1){ return reinterpret_cast<T*>(_alloc(sizeof(T)*copies)); }
        inline void *alloc(const size sz, const int copies = 1){ return _alloc(sz*copies); }
        inline void free(void *& mem = NONE){ _free(mem); }

        inline size getBufferSize(){ return bsize; }
        inline size getFreeSize(){ return fsize; }
        inline size getUsedSize(){ return bsize - fsize; }

        protected:
        //have to provide size cs because sizeof(*this) doesn't return correct size of inherited classes in construction phase
        Mmm(size sz, size cs): bsize(sz-cs), fsize(sz-cs){}
        inline void * operator new(size_t classSize, void *heap, size &heapSize, size extra = 0){
            return (heap == nullptr) ? malloc(heapSize += classSize + extra) : heap; 
        }

        virtual void * _alloc(size) = 0;
        virtual void _free(void*&) = 0;

        size bsize, fsize;
    };

    class TopDownStack : public virtual Mmm {
        public:

        TopDownStack(size sz, size cs = sizeof(TopDownStack)): 
            Mmm(sz-sizeof(size), cs), curr(reinterpret_cast<char*>(this) + cs + sizeof(size)){
            *(reinterpret_cast<size*>(curr) - 1) = 0;
        }
        
        protected:

        virtual void* _alloc(size);
        virtual void _free(void*&);

        char *curr;
    };

    class BottomUpStack : public virtual Mmm{
        public:

        BottomUpStack(size sz, size cs = sizeof(BottomUpStack)):
            Mmm(sz-sizeof(size), cs), curr(reinterpret_cast<char*>(this) + sz - sizeof(size)){
            *(reinterpret_cast<size*>(curr) + 1) = 0;
        }
        
        protected:
        virtual void *_alloc(size);
        virtual void _free(void*&);

        char *curr;
    };

    class DoubleStack : public TopDownStack, public BottomUpStack{
        public:

        DoubleStack(size sz, size cs = sizeof(DoubleStack)):
            Mmm(sz - 2*sizeof(size), cs), TopDownStack(sz, cs), BottomUpStack(sz, cs){}
        
        inline std::pair<TopDownStack*, BottomUpStack*> split(){
            return std::make_pair(dynamic_cast<TopDownStack*>(this), dynamic_cast<BottomUpStack*>(this));
        }

        protected:

        void *_alloc(size) final;
        void _free(void*&) final;
    };
}

#endif