#ifndef STORAGE_LEVELDB_UTIL_CRC32C_H_
#define STORAGE_LEVELDB_UTIL_CRC32C_H_

#include <stddef.h>
#include <stdint.h>

namespace leveldb
{
    namespace crc32c
    {
        // init_crc 表示初始的 CRC 校验值。在计算多个数据块的 CRC 校验值时，可以
        // 将上一个数据块的 CRC 校验值作为初始值传递到下一个数据块的 CRC 函数，
        // 从而实现连续计算
        extern uint32_t Extend(uint32_t init_crc, const char *data, size_t n);

        inline uint32_t Value(const char *data, size_t n)
        {
            // data = 0..kMaxRecordType(4)
            // n = 1
            return Extend(0, data, n);
        }

        static const uint32_t kMaskDelta = 0xa282ead8ul;

        inline uint32_t Mask(uint32_t crc)
        {
            return ((crc >> 15) | (crc << 17)) + kMaskDelta;
        }

    }
}

#endif