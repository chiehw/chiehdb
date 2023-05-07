#include "util/arena.h"

namespace leveldb
{
    static const int kBlockSize = 4096;

    Arena::Arena() : memory_usage_(0)
    {
        alloc_ptr_ = NULL;
        alloc_bytes_remaining_ = 0;
    }

    Arena::~Arena()
    {
        for (size_t i = 0; i < blocks_.size(); i++)
        {
            delete[] blocks_[i];
        }
    }

    inline char *Arena::Allocate(size_t bytes)
    {
        assert(bytes > 0);
        // 当前块内存可以分配
        if (bytes <= alloc_bytes_remaining_)
        {
            char *result = alloc_ptr_;
            alloc_ptr_ += bytes;
            alloc_bytes_remaining_ -= bytes;
            return result;
        }
        return AllocateFallback(bytes);
    }

    char *Arena::AllocateFallback(size_t bytes)
    {
        // 如果需要分配的空间大于 Block 的 1/4，则直接按照所需的空间分配。
        if (bytes > kBlockSize / 4)
        {
            char *result = AllocateNewBlock(bytes);
            return result;
        }

        // 否则分配一个 Block，并在新的 Block 上分配空间。
        alloc_ptr_ = AllocateNewBlock(kBlockSize);
        alloc_bytes_remaining_ = kBlockSize;

        char *result = alloc_ptr_;
        alloc_ptr_ += bytes;
        alloc_bytes_remaining_ -= bytes;
        return result;
    }

    char *Arena::AllocateNewBlock(size_t block_bytes)
    {
        char *result = new char[block_bytes];
        blocks_.push_back(result);
        memory_usage_.NoBarrier_Store(
            // 将大小转换为指针，存入原子指针。
            reinterpret_cast<void *>(MemoryUsage() + block_bytes + sizeof(char *)));
        return result;
    }

    char *Arena::AllocateAligned(size_t bytes)
    {
        // 如果指针的大小大于 8 字节，align 的值设置为指针大小，否则设置为 8 字节。
        const int align = (sizeof(void *) > 8) ? sizeof(void *) : 8;
        // 判断 align 是不是 2 的正整数次幂。
        assert((align & (align - 1)) == 0);
        // 计算内存的偏移量，以便确定下一个内存的初始地址应该对齐到那个地址。
        // 这里的与操作，相当于取模。
        size_t current_mod = reinterpret_cast<intptr_t>(alloc_ptr_) & (align - 1);
        size_t slop = (current_mod == 0) ? 0 : align - current_mod;
        size_t needed = bytes + slop;

        char *result;
        if (needed <= alloc_bytes_remaining_)
        {
            result = alloc_ptr_ + slop;
            alloc_ptr_ += needed;
            alloc_bytes_remaining_ -= needed;
        }
        else
        {
            result = AllocateFallback(bytes);
        }
        assert(reinterpret_cast<uintptr_t>(result) & (align - 1) == 0);
        return result;
    }
}