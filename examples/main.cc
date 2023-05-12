#include <cassert>
#include "leveldb/db.h"

int main(int argc, char **argv)
{
    leveldb::DB *db;
    leveldb::Options options;
    leveldb::Status status = leveldb::DB::Open(options, "test.db", &db);

    delete db;
}