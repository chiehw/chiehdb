#ifndef STORAGE_LEVELDB_INCLUDE_SLICE_H_
#define STORAGE_LEVELDB_INCLUDE_SLICE_H_

#include <stddef.h>
#include <string.h>
#include <string>
#include <assert.h>

namespace leveldb
{
    class Slice
    {
    private:
        const char *data_;
        size_t size_;

    public:
        Slice() : data_(""), size_(0) {}
        Slice(const char *data, size_t size) : data_(data), size_(size) {}
        Slice(const std::string &s) : data_(s.data()), size_(s.size()) {}
        Slice(const char *data) : data_(data), size_(strlen(data)) {}

        const char *data() const { return data_; }
        const size_t size() const { return size_; }

        bool empty() const { return size_ == 0; }
        int compare(const Slice &b) const
        {
            // 先判断长度相同的部分
            const size_t min_len = (size_ < b.size_) ? size_ : b.size_;
            int r = memcmp(data_, b.data_, min_len);
            if (r == 0)
            {
                // 字符短的小
                if (size_ < b.size_)
                    r = -1;
                else if (size_ > b.size_)
                    r = 1;
            }
            return r;
        }
        bool startsWith(const Slice &b) const
        {
            return size_ >= b.size_ && memcmp(data_, b.data_, b.size_) == 0;
        }

        char operator[](size_t index) const
        {
            assert(index < size());
            return data_[index];
        }
        void clear()
        {
            data_ = "";
            size_ = 0;
        }
        void remove_prefix(size_t n)
        {
            assert(n <= size());
            data_ += n;
            size_ -= n;
        }

        std::string ToString() const
        {
            return std::string(data_, size_);
        }
    };
    inline bool operator==(const Slice &x, const Slice &y)
    {
        return ((x.size() == y.size()) && memcmp(x.data(), y.data(), x.size()) == 0);
    }
    inline bool operator!=(const Slice &x, const Slice &y)
    {
        return !(x == y);
    }
}

#endif