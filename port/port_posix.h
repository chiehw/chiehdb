#ifndef STORAGE_LEVELDB_PORT_PORT_POSIX_H_
#define STORAGE_LEVELDB_PORT_PORT_POSIX_H_

// 在 x86 的 Linux 已经被定义了。
#ifndef PLATFORM_IS_LITTLE_ENDIAN
#define PLATFORM_IS_LITTLE_ENDIAN (__BYTE_ORDER == __LITTLE_ENDIAN)
#endif

namespace leveldb
{
    namespace port
    {

        static const bool kLittleEndian = PLATFORM_IS_LITTLE_ENDIAN;
#undef PLATFORM_IS_LITTLE_ENDIAN

    }
}

#endif