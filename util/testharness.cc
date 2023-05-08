#include "util/testharness.h"

namespace leveldb
{
    namespace test
    {

        struct Test
        {
            const char *base;
            const char *name;
            void (*func)();
        };
        std::vector<Test> *tests;

        int RunAllTests()
        {
            const char *matcher = getenv("LEVELDB_TESTS");

            int num = 0;
            if (tests != NULL)
            {
                for (size_t i = 0; i < tests->size(); i++)
                {
                    const Test &t = (*tests)[i];
                    if (matcher != NULL)
                    {
                        std::string name = t.base;
                        name.push_back('.');
                        name.append(t.name);
                        if (strstr(name.c_str(), matcher) == NULL)
                        {
                            continue;
                        }
                    }
                    fprintf(stderr, "==== Test %s.%s\n", t.base, t.name);
                    (*t.func)();
                    ++num;
                }
            }
            fprintf(stderr, "==== PASSED %d tests\n", num);
            return 0;
        }

        bool RegisterTest(const char *base, const char *name, void (*func)())
        {
            if (tests == NULL)
            {
                tests = new std::vector<Test>;
            }
            Test t;
            t.base = base;
            t.name = name;
            t.func = func;
            tests->push_back(t);
            return true;
        }

    }
}