#include "leveldb/env.h"
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

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
}