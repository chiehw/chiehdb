#ifndef STORAGE_LEVELDB_DB_DB_IMPL_H_
#define STORAGE_LEVELDB_DB_DB_IMPL_H_

#include "leveldb/options.h"
#include "leveldb/db.h"
#include "port/port.h"
#include "leveldb/status.h"
#include "leveldb/env.h"
#include "version_set.h"
#include "db/log_write.h"
#include <deque>

namespace leveldb
{
    class DBImpl : public DB
    {
    public:
        DBImpl(const Options &options, const std::string &dbname);
        virtual ~DBImpl();

    private:
        friend class DB;
        struct Writer;

        const std::string dbname_;
        Env *const env_;

        uint64_t logfile_number_;
        WritableFile *logfile_;
        log::Writer *log_;

        port::Mutex mutex_;
        VersionSet *versions_;

        std::deque<Writer *> writers_;

        DBImpl(const DBImpl &);
        void operator=(const DBImpl &);
    };
}

#endif