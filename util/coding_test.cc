#include "util/coding.h"
#include "util/testharness.h"

namespace leveldb
{
    class Coding
    {
    };
    TEST(Coding, Fixed32)
    {
        std::string s;
        for (uint32_t v = 0; v < 10000; v++)
        {
            PutFixed32(&s, v);
        }

        const char *p = s.data();
        for (uint32_t v = 0; v < 10000; v++)
        {
            uint32_t actual = DecodeFixed32(p);
            assert(v == actual);
            p += sizeof(uint32_t);
        }
    }
}

int main(int argc, char **argv)
{
    return leveldb::test::RunAllTests();
}