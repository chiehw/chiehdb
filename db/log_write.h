#ifndef STORAGE_LEVELDB_DB_LOG_WRITER_H_
#define STORAGE_LEVELDB_DB_LOG_WRITER_H_

#include <stdint.h>
#include "leveldb/status.h"
#include "leveldb/slice.h"
#include "db/log_format.h"

namespace leveldb
{
    // 声明 WritableFile 存在
    class WritableFile;
    namespace log
    {
        class Writer
        {
        public:
            explicit Writer(WritableFile *dest);
            // 添加数据到 dest，dest 的长度必须是 dest_length
            Writer(WritableFile *dest, uint64_t dest_length);
            ~Writer();

            Status AddRecord(const Slice &slice);

        private:
            WritableFile *dest_;
            // 当前块的偏移
            int block_offset_;

            uint32_t type_crc_[kMaxRecordType + 1];
            Status EmitPhysicalRecord(RecordType type, const char *ptr, size_t length);

            // 禁止拷贝构造和拷贝赋值
            Writer(const Writer &);
            void operator=(const Writer &);
        };

    }
}

#endif