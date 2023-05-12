#include "leveldb/env.h"
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>

namespace leveldb
{
    namespace
    {
        static Status IOError(const std::string &context, int err_number)
        {
            return Status::IOError(context, strerror(err_number));
        }

        class PosixWritableFile : public WritableFile
        {
        private:
            std::string filename_;
            FILE *file_;

        public:
            PosixWritableFile(const std::string &fname, FILE *f) : filename_(fname), file_(f) {}
            ~PosixWritableFile()
            {
                if (file_ != NULL)
                {
                    fclose(file_);
                }
            }

            virtual Status Append(const Slice &data)
            {
                // 第二个参数为数据块的大小，将每个字符都视为一个单独的数据块写入到文件。
                size_t r = fwrite_unlocked(data.data(), 1, data.size(), file_);
                if (r != data.size())
                {
                    return IOError(filename_, errno);
                }
                return Status::OK();
            }

            virtual Status Flush()
            {
                Status result;
                if (fflush_unlocked(file_) != 0)
                {
                    result = IOError(filename_, errno);
                }
                file_ = NULL;
                return result;
            }

            Status SyncDirManifest()
            {
                const char *f = filename_.c_str();
                const char *sep = strchr(f, '/');
                Slice basename;
                std::string dir;
                if (sep == NULL)
                {
                    dir = ".";
                    basename = f;
                }
                else
                {
                    dir = std::string(f, sep - f);
                    basename = sep + 1;
                }

                Status s;
                if (basename.startsWith("MANIFEST"))
                {
                    int fd = open(dir.c_str(), O_RDONLY);
                    if (fd < 0)
                    {
                        s = IOError(dir, errno);
                    }
                    else
                    {
                        if (fsync(fd) < 0)
                        {
                            s = IOError(dir, errno);
                        }
                        close(fd);
                    }
                }
                return s;
            }

            virtual Status Sync()
            {
                Status s = SyncDirManifest();
                if (!s.ok())
                {
                    return s;
                }
                if (fflush_unlocked(file_) != 0 || fdatasync(fileno(file_)) != 0)
                {
                    s = Status::IOError(filename_, strerror(errno));
                }
                return s;
            }

            virtual Status Close()
            {
                Status result;
                if (fclose(file_) != 0)
                {
                    result = IOError(filename_, errno);
                }
                file_ = NULL;
                return result;
            }
        };

        static int LockOrUnlock(int fd, bool lock)
        {
            errno = 0;
            struct flock f;
            memset(&f, 0, sizeof(f));
            f.l_type = lock ? F_WRLCK : F_UNLCK;
            f.l_whence = SEEK_SET;
            f.l_start = 0;
            f.l_len = 0;
            return fcntl(fd, F_SETLK, &f);
        }
    }

    class PosixEnv : public Env
    {
    public:
        PosixEnv();
        virtual ~PosixEnv()
        {
            char msg[] = "Destroying Env::Default()\n";
            fwrite(msg, 1, sizeof(msg), stderr);
        }

        virtual Status CreateDir(const std::string &name)
        {
            Status result;
            if (mkdir(name.c_str(), 0755) != 0)
            {
                result = IOError(name, errno);
            }
            return result;
        }

        virtual Status NewWritableFile(const std::string &fname, WritableFile **result)
        {
            Status s;
            FILE *f = fopen(fname.c_str(), "w");
            if (f == NULL)
            {
                *result = NULL;
                s = IOError(fname, errno);
            }
            else
            {
                *result = new PosixWritableFile(fname, f);
            }
            return s;
        }

    private:
        void PthreadCall(const char *label, int result)
        {
            if (result != 0)
            {
                fprintf(stderr, "pthread %s: %s\n", label, strerror(result));
                abort();
            }
        }

        pthread_mutex_t mu_;
    };

    PosixEnv::PosixEnv()
    {
        PthreadCall("mutex_init", pthread_mutex_init(&mu_, NULL));
    }

    static pthread_once_t once = PTHREAD_ONCE_INIT;
    static Env *default_env;
    static void InitDefaultEnv()
    {
        default_env = new PosixEnv;
    }

    Env *Env::Default()
    {
        pthread_once(&once, InitDefaultEnv);
        return default_env;
    }
}