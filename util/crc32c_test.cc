#include "crc32c.h"
#include "util/testharness.h"
#include <assert.h>

namespace leveldb
{
    namespace crc32c
    {
        class CRC
        {
        };
        TEST(CRC, StandardResults)
        {
            char buf[32];
            memset(buf, 0, sizeof(buf));
            assert(0x8a9136aa == Value(buf, sizeof(buf)));
        }
    }
}

int main(int argc, char **argv)
{
    return leveldb::test::RunAllTests();
}
