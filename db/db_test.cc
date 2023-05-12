#include "leveldb/db.h"
#include "util/testharness.h"

namespace leveldb
{

    class DBTest
    {
    };
    TEST(DBTest, Open)
    {
        leveldb::DB *db;
        leveldb::Options options;
        // // options.create_if_missing = true;
        leveldb::Status status = leveldb::DB::Open(options, "test.db", &db);
        // assert(status == Status::OK());
        delete db;
    }
}

int main(int argc, char **argv)
{
    printf("leveldb arena test\n");

    return leveldb::test::RunAllTests();
}