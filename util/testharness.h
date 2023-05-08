#ifndef STORAGE_LEVELDB_UTIL_TESTHARNESS_H_
#define STORAGE_LEVELDB_UTIL_TESTHARNESS_H_

#include <vector>
#include <stdlib.h>
#include <string>
#include <string.h>

namespace leveldb
{
    namespace test
    {
        extern int RunAllTests();
        extern bool RegisterTest(const char *base, const char *name, void (*func)());

#define TCONCAT(a, b) TCONCAT1(a, b)
#define TCONCAT1(a, b) a##b

// 定义 Test 类，用于注册自定义的 Test 类。
#define TEST(base, name)                                                                                              \
    class TCONCAT(_Test_, name) : public base                                                                         \
    {                                                                                                                 \
    public:                                                                                                           \
        void _Run();                                                                                                  \
        static void _RunIt()                                                                                          \
        {                                                                                                             \
            TCONCAT(_Test_, name)                                                                                     \
            t;                                                                                                        \
            t._Run();                                                                                                 \
        }                                                                                                             \
    };                                                                                                                \
    bool TCONCAT(_Test_ignored_, name) = ::leveldb::test::RegisterTest(#base, #name, &TCONCAT(_Test_, name)::_RunIt); \
    void TCONCAT(_Test_, name)::_Run() // Run 会被 RunIt 调用，而 RunIt 由 RunAllTests 调用（因为该类会被注册到 tests）。
    };
}

#endif