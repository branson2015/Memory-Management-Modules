#ifndef MMM_HEADER
#define MMM_HEADER

#include <cstddef> 
#include <utility>

//TODO: change all char*'s to void*'s and use as little typecasting as possible
//implement rule of 5 or rule of 0

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

        template<typename T> inline T *alloc(int copies = 1){ return reinterpret_cast<T*>(_alloc(sizeof(T)*copies, alignof(T))); }
        inline void *alloc(const size sz, int copies = 1, size align = alignment){ return _alloc(sz*copies, align); }
        inline void free(void *& mem = NONE){ _free(mem); }

        inline size getBufferSize(){ return bsize; }
        inline size getFreeSize(){ return fsize; }
        inline size getUsedSize(){ return bsize - fsize; }

        protected:
        //have to provide size cs because sizeof(*this) doesn't return correct size of inherited classes in construction phase
        Mmm(size sz, size cs): bsize(sz-cs), fsize(sz-cs){}
            
        inline void * operator new(size classSize, void *heap, size &heapSize, size extra = 0){
            if(heap == nullptr){
                heapSize = align(heapSize + classSize + extra);
                return reinterpret_cast<void*>(align(reinterpret_cast<size>(malloc(heapSize + sizeof(void*)))));    //maybe don't need to align this part?
            }else{
                void *rtn = reinterpret_cast<void*>(align((reinterpret_cast<size>(heap))));
                heapSize -= (reinterpret_cast<char*>(rtn) - reinterpret_cast<char*>(heap));
                return rtn;
            }
        }
        
        virtual void * _alloc(size, size) = 0;
        virtual void _free(void*&) = 0;

        size bsize, fsize;
        const static size alignment = sizeof(void*);
 
        inline static size balign(size addr, size a = alignment){ return addr & (-a); }
        inline static size align(size sz, size a = alignment){ return ((sz + a - 1) & (-a)); }
    };

    class TopDownStack : public virtual Mmm {
        public:

        TopDownStack(size sz, size cs = sizeof(TopDownStack)): 
            Mmm(sz-sizeof(size), cs), curr(reinterpret_cast<char*>(align(reinterpret_cast<size>(reinterpret_cast<char*>(this) + cs)))){
            *reinterpret_cast<size*>(curr) = 0;
        }
        
        protected:

        virtual void* _alloc(size, size);
        virtual void _free(void*&);

        char *curr;
    };

    class BottomUpStack : public virtual Mmm{
        public:

        BottomUpStack(size sz, size cs = sizeof(BottomUpStack)):
            Mmm(sz-sizeof(size), cs), curr(reinterpret_cast<char*>(balign(reinterpret_cast<size>(this) + cs + sz))){
            *reinterpret_cast<size*>(curr) = 0;
        }
        
        protected:
        virtual void *_alloc(size, size);
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

        void *_alloc(size, size) final;
        void _free(void*&) final;
    };
}

#endif