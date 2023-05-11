#include "port/port_posix.h"

namespace leveldb
{
    namespace port
    {
        static void PthreadCall(const char *label, int result)
        {
            if (result != 0)
            {
                fprintf(stderr, "pthread %s: %s\n", label, strerror(result));
                abort();
            }
        }

        Mutex::Mutex()
        {
            PthreadCall("init mutex", pthread_mutex_init(&mu_, NULL));
        }

        Mutex::~Mutex()
        {
            PthreadCall("destroy mutex", pthread_mutex_destroy(&mu_));
        }

        void Mutex::Lock()
        {
            PthreadCall("lock mutex", pthread_mutex_lock(&mu_));
        }

        void Mutex::Unlock()
        {
            PthreadCall("unlock mutex", pthread_mutex_unlock(&mu_));
        }
    }
}