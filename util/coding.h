#ifndef STORAGE_LEVELDB_UTIL_CODING_H_
#define STORAGE_LEVELDB_UTIL_CODING_H_

#include <stdint.h>
#include <string>
#include "leveldb/slice.h"
#include "port/port.h"

namespace leveldb
{
    extern void EncodeFixed32(char *buf, uint32_t value);

    inline uint32_t DecodeFixed32(const char *ptr)
    {
        // 小端序：低字节存储在低地址，高字节存储在高地址。
        if (port::kLittleEndian)
        {
            // Load the raw bytes
            uint32_t result;
            memcpy(&result, ptr, sizeof(result)); // gcc optimizes this to a plain load
            return result;
        }
        else
        {
            // 将大端序转换为小端序
            // char 类型有时会被视为有符号整数类型，而 unsigned char 类型则始终被视为无符号整数类型。
            return ((static_cast<uint32_t>(static_cast<unsigned char>(ptr[0]))) |
                    (static_cast<uint32_t>(static_cast<unsigned char>(ptr[1])) << 8) |
                    (static_cast<uint32_t>(static_cast<unsigned char>(ptr[2])) << 16) |
                    (static_cast<uint32_t>(static_cast<unsigned char>(ptr[3])) << 24));
        }
    }
}

#endif