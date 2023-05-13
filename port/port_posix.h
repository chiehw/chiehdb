#ifndef STORAGE_LEVELDB_PORT_PORT_POSIX_H_
#define STORAGE_LEVELDB_PORT_PORT_POSIX_H_

// 在 x86 的 Linux 已经被定义了。
#ifndef PLATFORM_IS_LITTLE_ENDIAN
#define PLATFORM_IS_LITTLE_ENDIAN (__BYTE_ORDER == __LITTLE_ENDIAN)
#endif

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

namespace leveldb
{
    namespace port
    {

        static const bool kLittleEndian = PLATFORM_IS_LITTLE_ENDIAN;
#undef PLATFORM_IS_LITTLE_ENDIAN

        class Mutex
        {
        public:
            Mutex();
            ~Mutex();

            void Lock();
            void Unlock();
            void AssertHeld() {}

        private:
            friend class CondVar;
            pthread_mutex_t mu_;

            Mutex(const Mutex &);
            void operator=(const Mutex &);
        };

        class CondVar
        {
        public:
            explicit CondVar(Mutex *mu);
            ~CondVar();

            void Wait();
            void Signal();
            void SignalAll();

        private:
            pthread_cond_t cv_;
            Mutex *mu_;
        };

    }
}

#endif