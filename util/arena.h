#ifndef STORAGE_LEVELDB_UTIL_ARENA_H_
#define STORAGE_LEVELDB_UTIL_ARENA_H_

#include <stddef.h> // standard definitions
#include <vector>
#include "port/atomic_pointer.h"
#include <assert.h>
#include <stdint.h>

namespace leveldb
{

    class Arena
    {
    public:
        Arena();
        ~Arena();

        char *Allocate(size_t bytes);        // return a pointer to a newly allocated memory block of "bytes" bytes.
        char *AllocateAligned(size_t bytes); // Allocate memory with the normal alignment guaratees provided by malloc.
        size_t MemoryUsage() const;          // Return an estimate of the total memory usage of data allocated by the arena.

    private:
        // Allocation state
        char *alloc_ptr_;
        size_t alloc_bytes_remaining_;
        // Array of new[] allocated memory blocks
        std::vector<char *> blocks_;
        // Total memory usage of the arena
        port::AtomicPointer memory_usage_;

        // No copying allowed
        Arena(const Arena &);
        void operator=(const Arena &);

        char *AllocateFallback(size_t bytes);
        char *AllocateNewBlock(size_t block_bytes);
    };

}

#endif