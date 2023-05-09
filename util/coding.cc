#include "util/coding.h"

namespace leveldb
{
    // 根据系统是大端还是小端，将数据存入 buf
    void EncodeFixed32(char *buf, uint32_t value)
    {
        if (port::kLittleEndian)
        {
            mempcpy(buf, &value, sizeof(value));
        }
        else
        {
            buf[0] = value & 0xff;
            buf[1] = value >> 8 & 0xff;
            buf[2] = value >> 16 & 0xff;
            buf[3] = value >> 24 & 0xff;
        }
    }
}