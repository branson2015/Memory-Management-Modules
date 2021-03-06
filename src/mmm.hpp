#ifndef MMM_HEADER
#define MMM_HEADER

#include <cstddef> 
#include <utility>

//TODO:
//impliment pool allocator
//impliment linear allocator
//implement rule of 5 or rule of 0
//make compatible with std::allocator

namespace mmm{
    using UCHAR = unsigned char;
    using size = size_t;    //can only be unsigned type

    namespace{
        UCHAR top = 1;
        UCHAR bottom = -1;
        UCHAR none = 0;
    }
    void * const TOP = reinterpret_cast<void*>(&top);
    void * const BOTTOM = reinterpret_cast<void*>(&bottom);
    void * const NONE = reinterpret_cast<void*>(&none);
    
    enum class MmmStackType{
        SingleStack,
        TopDownStack,
        BottomUpStack,
        DoubleStack
    };
    enum class MmmPoolType{
        FixedPage
    };
    
    class Mmm {
        public:
        
        static Mmm *createStack(MmmStackType, const size, void* = nullptr);
        static Mmm *createPool(MmmPoolType, const size, void* = nullptr);

        template<typename T> inline T *alloc(int copies = 1){ return reinterpret_cast<T*>(_alloc(sizeof(T)*copies, alignof(T))); }
        inline void *alloc(const size sz = 0, int copies = 1, size align = alignment){ return _alloc(sz*copies, align); }
        inline void free(void *mem = NONE){ _free(mem); }

        inline size getBufferSize(){ return bsize; }
        inline size getFreeSize(){ return fsize; }
        inline size getUsedSize(){ return bsize - fsize; }

        protected:
        Mmm(size sz, size cs): bsize(sz-cs), fsize(sz-cs){} //have to provide size cs because sizeof(*this) doesn't return correct size of inherited classes in construction phase
        
        //possibly put this in the create class instead? probably better suited there.
        inline void * operator new(size classSize, void *heap, size &heapSize, size extra = 0){
            if(heap == nullptr){
                heapSize = align(heapSize + classSize + extra);
                return reinterpret_cast<void*>(align(reinterpret_cast<size>(malloc(heapSize + sizeof(size)))));
            }else{
                void *rtn = reinterpret_cast<void*>(align((reinterpret_cast<size>(heap))));
                heapSize -= (reinterpret_cast<char*>(rtn) - reinterpret_cast<char*>(heap));
                return rtn;
            }
        }
        ~Mmm() = default;
        
        virtual void * _alloc(size, size) = 0;
        virtual void _free(void*) = 0;

        size bsize, fsize;
        const static size alignment = sizeof(size);
 
        inline static size balign(size addr, size a = alignment){ return addr & (-a); }
        inline static size align(size sz, size a = alignment){ return ((sz + a - 1) & (-a)); }
    };

    class TopDownStack : public virtual Mmm {
        public:
        TopDownStack(size sz, size cs = sizeof(TopDownStack)): 
            Mmm(sz-sizeof(size), cs), 
            curr(reinterpret_cast<UCHAR*>(reinterpret_cast<size>(this) + cs + sizeof(size))){
            *(reinterpret_cast<size*>(curr) - 1) = 0;
        }
        
        protected:
        virtual void* _alloc(size, size);
        virtual void _free(void*);

        UCHAR *curr;
    };

    class BottomUpStack : public virtual Mmm{
        public:
        BottomUpStack(size sz, size cs = sizeof(BottomUpStack)):
            Mmm(sz-sizeof(size), cs), 
            curr(reinterpret_cast<UCHAR*>(reinterpret_cast<size>(this) + cs + sz)){
            *reinterpret_cast<size*>(curr) = 0;
        }
        
        protected:
        virtual void *_alloc(size, size);
        virtual void _free(void*);

        UCHAR *curr;
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
        void _free(void*) final;
    };

    class FixedPage: public Mmm{
        public:
        FixedPage(size sz, size cs = sizeof(FixedPage), size ps = 512);

        protected:
        const size pageSize;
        const size numPages;

        size pagesInUse;

        UCHAR *freeHead;
        UCHAR *freeFoot;

        void *_alloc(size sz, size alignment) override;
        void _free(void*) override;
    };
}

#endif