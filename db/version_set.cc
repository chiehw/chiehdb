#include "db/version_set.h"

namespace leveldb
{
    VersionSet::VersionSet() : next_file_number_(2)
    {
    }

    VersionSet::~VersionSet()
    {
    }

}