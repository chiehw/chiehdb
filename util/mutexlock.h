
#ifndef STORAGE_LEVELDB_UTIL_MUTEXLOCK_H_
#define STORAGE_LEVELDB_UTIL_MUTEXLOCK_H_

#include "port/port.h"

namespace leveldb
{
    class MutexLock
    {
    public:
        explicit MutexLock(port::Mutex *mu) : mu_(mu)
        {
            this->mu_->Lock();
        }
        ~MutexLock()
        {
            this->mu_->Unlock();
        }

    private:
        port::Mutex *const mu_;

        MutexLock(const MutexLock &);
        void operator=(const MutexLock &);
    };
}
#endif