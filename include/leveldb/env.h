#ifndef STORAGE_LEVELDB_INCLUDE_ENV_H_
#define STORAGE_LEVELDB_INCLUDE_ENV_H_

#include "leveldb/slice.h"
#include "leveldb/status.h"

namespace leveldb
{
    class WritableFile;

    class Env
    {
    public:
        Env() {}
        virtual ~Env() {}

        static Env *Default();
        virtual Status NewWritableFile(const std::string &fname, WritableFile **result) = 0;
        virtual Status CreateDir(const std::string &name) = 0;
    };

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

    class SequentialFile
    {
    public:
        SequentialFile() {}
        virtual ~SequentialFile() {}
        // 参考 Google 的 C++ 编程风格指南，一般输入的参数在前，输出参数在后。
        // result 指向 Slice 结构体将用于存储读取的数据。
        // scratch 返回读取到的 n 个字节
        virtual Status Read(size_t n, Slice *result, char *scratch) = 0;
        // 顺序读取时，某些信息不是用户需要的，该方法可以跳过这些信息。
        // 使用 Skip 跳过比 Read 跳过效率更高。
        virtual Status Skip(uint64_t n) = 0;

    private:
        SequentialFile(const SequentialFile &);
        void operator=(const SequentialFile &);
    };

}

#endif