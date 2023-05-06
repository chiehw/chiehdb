#ifndef PORT_ATOMIC_POINTER_H_
#define PORT_ATOMIC_POINTER_H_

#if defined(_M_X64) || defined(__x86_64__)
#define ARCH_CPU_X86_FAMILY 1
#endif

namespace leveldb
{
    namespace port
    {
#if defined(ARCH_CPU_X86_FAMILY) && defined(__GNUC__)
        inline void MemoryBarrier()
        {
            __asm__ __volatile__(""
                                 :
                                 :
                                 : "memory");
        }
#define LEVELDB_HAVE_MEMORY_BARRIER
#endif

        class AtomicPointer
        {
        private:
            void *rep_;

        public:
            AtomicPointer() {}
            explicit AtomicPointer(void *p) : rep_(p) {}
            inline void *NoBarrier_Load() const { return rep_; }
            inline void NoBarrier_Store(void *p) { rep_ = p; }
            inline void *Acquire_Load() const
            {
                void *result = rep_;
                MemoryBarrier();
                return result;
            }
            inline void Release_Store(void *v)
            {
                MemoryBarrier();
                rep_ = v;
            }
        };

    }
}

#endif