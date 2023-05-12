#include "db/filename.h"
#include <assert.h>

namespace leveldb
{
    static std::string MakeFileName(const std::string name, uint64_t number, const char *suffix)
    {
        char buf[100];
        snprintf(buf, sizeof(buf), "/%06llu.%s", static_cast<unsigned long long>(number), suffix);
        return name + buf;
    }
    std::string LogFileName(const std::string &dbname, uint64_t log_number)
    {
        assert(log_number > 0);
        return MakeFileName(dbname, log_number, "log");
    }
}