#ifndef STORAGE_LEVELDB_INCLUDE_ENV_H_
#define STORAGE_LEVELDB_INCLUDE_ENV_H_

#include "leveldb/slice.h"
#include "leveldb/status.h"

namespace leveldb
{
    class WritableFile;

    class WritableFile
    {
    public:
        WritableFile() {}
        virtual ~WritableFile() {}
        // 纯虚函数
        virtual Status Append(const Slice &data) = 0;
        virtual Status Close() = 0;
        virtual Status Flush() = 0;
        virtual Status Sync() = 0;

    private:
        // 禁止拷贝构造和拷贝赋值
        WritableFile(const WritableFile &);
        void operator=(const WritableFile &);
    };

}

#endif