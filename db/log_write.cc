
#include "db/log_write.h"
#include "db/log_format.h"
#include "util/crc32c.h"
#include "leveldb/env.h"
#include "util/coding.h"

namespace leveldb
{
    namespace log
    {
        static void InitTypeCrc(uint32_t *type_crc)
        {
            for (int i = 0; i <= kMaxRecordType; i++)
            {
                char t = static_cast<char>(i);
                type_crc[i] = crc32c::Value(&t, 1);
            }
        }

        Writer::Writer(WritableFile *dest)
            : dest_(dest),
              block_offset_(0)
        {
            InitTypeCrc(type_crc_);
        }

        Writer::~Writer()
        {
        }

        Status Writer::AddRecord(const Slice &slice)
        {
            const char *ptr = slice.data();
            size_t left = slice.size();

            Status s;
            bool begin = true;
            do
            {
                // 如果偏移小于 kBlockSize，则没有越界，leftover 大于 0
                const int leftover = kBlockSize - block_offset_;
                assert(leftover >= 0);
                // 如果不足以防止 head（7 bytes），那就切换到新的块
                if (leftover < kHeaderSize)
                {
                    // 如果有剩余，那就用 \x00 填充
                    if (leftover > 0)
                    {
                        assert(kHeaderSize == 7);
                        dest_->Append(Slice("\x00\x00\x00\x00\x00\x00", leftover));
                    }
                    // block_offset 重置，剩余空间已经存放到 leftover 中。
                    block_offset_ = 0;
                }

                assert(kBlockSize - block_offset_ - kHeaderSize >= 0);

                // 全放完还是只能放一部分
                const size_t avail = kBlockSize - block_offset_ - kHeaderSize;
                const size_t fragment_length = (left < avail) ? left : avail;
                RecordType type;
                const bool end = (left == fragment_length);
                if (begin && end)
                {
                    type = kFullType;
                }
                else if (begin)
                {
                    type = kFirstType;
                }
                else if (end)
                {
                    type = kLastType;
                }
                else
                {
                    type = kMiddleType;
                }

                // 写入磁盘
                s = EmitPhysicalRecord(type, ptr, fragment_length);

                ptr += fragment_length;
                left -= fragment_length;
                begin = false;
            } while (s.ok() && left > 0);
            return s;
        }

        Status Writer::EmitPhysicalRecord(RecordType type, const char *ptr, size_t n)
        {
            // n 必须小于 64 KB，Block 的大小是 32 KB
            assert(n <= 0xffff);
            assert(block_offset_ + kHeaderSize + n <= kBlockSize);

            // 0-3 校验值，4-5 长度，6 类型
            char buf[kHeaderSize];
            // 小端序
            buf[4] = static_cast<char>(n & 0xff); // 长度的低位
            buf[5] = static_cast<char>(n >> 8);   // 长度的高位
            buf[6] = static_cast<char>(type);

            uint32_t crc = crc32c::Extend(type_crc_[type], ptr, n);
            // 避免 CRC 冲突
            crc = crc32c::Mask(crc);
            EncodeFixed32(buf, crc);

            Status s = dest_->Append(Slice(buf, kHeaderSize));
            if (s.ok())
            {
                s = dest_->Append(Slice(ptr, n));
                if (s.ok())
                {
                    s = dest_->Flush();
                }
            }
            block_offset_ += kHeaderSize + n;
            return s;
        }

    }
}