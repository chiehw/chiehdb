#include "db/db_impl.h"
#include "leveldb/db.h"
#include "db/filename.h"

namespace leveldb
{
    DB::~DB() {}

    struct DBImpl::Writer
    {
        Status status;

        bool sync;
        bool down;
        port::CondVar cv;

        explicit Writer(port::Mutex *mu) : cv(mu) {}
    };

    DBImpl::DBImpl(const Options &raw_options, const std::string &dbname)
        : dbname_(dbname), env_(raw_options.env)
    {
        versions_ = new VersionSet();
    }

    Status DB::Open(const Options &options, const std::string &dbname, DB **dbptr)
    {
        *dbptr = NULL;

        DBImpl *impl = new DBImpl(options, dbname);
        impl->mutex_.Lock();

        // 通过 log 文件恢复 MemTable
        impl->env_->CreateDir(dbname);
        // 创建 Log 文件
        WritableFile *lfile;
        uint64_t new_log_number = impl->versions_->NewFileNumber();
        Status s = options.env->NewWritableFile(LogFileName(dbname, new_log_number), &lfile);

        if (s.ok())
        {
            impl->logfile_number_ = new_log_number;
            impl->logfile_ = lfile;
            impl->log_ = new log::Writer(lfile);
        }

        impl->mutex_.Unlock();
        if (s.ok())
        {
            *dbptr = impl;
        }
        else
        {
            delete impl;
        }
        return s;
    }
    DBImpl::~DBImpl()
    {
    }
}