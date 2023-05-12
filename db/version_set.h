#ifndef STORAGE_LEVELDB_DB_VERSION_SET_H_
#define STORAGE_LEVELDB_DB_VERSION_SET_H_

#include <stdint.h>

namespace leveldb
{
    class VersionSet
    {
    public:
        VersionSet();
        ~VersionSet();

        uint64_t NewFileNumber()
        {
            return next_file_number_++;
        }

    private:
        uint64_t next_file_number_;
    };
}

#endif
