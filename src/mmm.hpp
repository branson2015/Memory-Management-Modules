#ifndef MMM_HEADER
#define MMM_HEADER

#include <cstddef> 
#include <utility>

//TODO: fix private/public used for testing
//TODO: allocate more space for holding object and first size pointer

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
        
        using size = size_t;
        static Mmm *create(MmmType, const size, void* = nullptr);

        template<typename T> inline T *alloc(const int copies = 1){ return static_cast<T*>(_alloc(sizeof(T)*copies)); }
        inline void *alloc(const size sz, const int copies = 1){ return _alloc(sz*copies); }
        inline void free(void *& mem = NONE){ _free(mem); }

        inline size getBufferSize(){ return bsize; }
        inline size getFreeSize(){ return fsize; }
        inline size getUsedSize(){ return bsize - fsize; }

        protected:
        public:
        
        inline Mmm(size sz, size cs): bsize(sz-cs), fsize(sz-cs){}

        virtual void * _alloc(size) = 0;
        virtual void _free(void*&) = 0;

        size bsize, fsize;

        private:
        static size getClassSize(MmmType);
    };

    class TopDownStack : public virtual Mmm {
        public:
        inline TopDownStack(size sz, size cs = sizeof(TopDownStack)): 
            Mmm(sz-sizeof(size), cs), curr(reinterpret_cast<char*>(this) + cs + sizeof(size)){
            *(reinterpret_cast<size*>(curr) - 1) = 0;
        }

        virtual void* _alloc(size);
        virtual void _free(void*&);

        //protected:
        char *curr;
    };

    class BottomUpStack : public virtual Mmm{
        public:
        inline BottomUpStack(size sz, size cs = sizeof(BottomUpStack)):
            Mmm(sz-sizeof(size), cs), curr(reinterpret_cast<char*>(this) + sz - sizeof(size)){
            *(reinterpret_cast<size*>(curr) + 1) = 0;
        }

        virtual void *_alloc(size);
        virtual void _free(void*&);

        //protected:
        char *curr;
    };

    class DoubleStack : public TopDownStack, public BottomUpStack{
        public:

        inline DoubleStack(size sz, size cs = sizeof(DoubleStack)):
            Mmm(sz - 2*sizeof(size), cs), TopDownStack(sz, cs), BottomUpStack(sz, cs){}
        
        void *_alloc(size) final;
        void _free(void*&) final;

        inline std::pair<TopDownStack*, BottomUpStack*> split(){
            return std::make_pair(dynamic_cast<TopDownStack*>(this), dynamic_cast<BottomUpStack*>(this));
        }
    };
}

#endif