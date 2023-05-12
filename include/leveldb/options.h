#ifndef STORAGE_LEVELDB_INCLUDE_OPTIONS_H_
#define STORAGE_LEVELDB_INCLUDE_OPTIONS_H_

#include <stddef.h>
#include "leveldb/env.h"

namespace leveldb
{
    struct Options
    {
        // 使用自定义的接口和操作系统交互。
        Env *env;

        Options();
    };

    struct WriteOptions
    {
        bool sync;
        WriteOptions() : sync(false) {}
    };
}

#endif