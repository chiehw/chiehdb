#include "util/arena.h"
#include "util/testharness.h"

using namespace leveldb;

class ArenaTest
{
};

TEST(ArenaTest, Empty)
{
    Arena arena;
}

TEST(ArenaTest, Allocate)
{
    Arena arena;

    assert(arena.MemoryUsage() == 0);
    arena.Allocate(500);
    assert(arena.MemoryUsage() == 4104);
    arena.Allocate(3500);
    assert(arena.MemoryUsage() == 4104);
    arena.Allocate(1500);
    assert(arena.MemoryUsage() == 5612);
}

int main(int argc, char **argv)
{
    printf("leveldb arena test\n");

    return leveldb::test::RunAllTests();
}