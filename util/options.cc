#include "leveldb/options.h"
#include "leveldb/env.h"

namespace leveldb
{
    Options::Options()
        : env(Env::Default())
    {
    }
}