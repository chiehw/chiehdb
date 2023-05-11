
# 外部定义可以覆盖 OPT
OPT ?= -O2 -DNDEBUG

CC=cc
CXX=g++
CXXFLAGS += -I. -I./include $(OPT)
TARGET_OS=`uname -s`
LDFLAGS=-pthread

# 编译环境检测
$(shell CC="$(CC)" CXX="$(CXX)" TARGET_OS="$(TARGET_OS)" ./build_detect_platform build_config.mk ./)
include build_config.mk

# 输出目录
STATIC_OUTDIR=out-static
STATIC_LIBOBJECTS := $(addprefix $(STATIC_OUTDIR)/, $(SOURCES:.cc=.o))


TESTHARNESS := $(STATIC_OUTDIR)/util/testharness.o


# 静态编译输出文件夹生成
$(STATIC_OUTDIR):
	mkdir $@
$(STATIC_OUTDIR)/db: | $(SIMULATOR_OUTDIR)
	mkdir $@
$(STATIC_OUTDIR)/util: | $(STATIC_OUTDIR)
	mkdir $@
$(STATIC_OUTDIR)/helpers/memenv: | $(STATIC_OUTDIR)
	mkdir -p $@
$(STATIC_OUTDIR)/port: | $(STATIC_OUTDIR)
	mkdir $@
$(STATIC_OUTDIR)/table: | $(STATIC_OUTDIR)
	mkdir $@
.PHONY: STATIC_OBJDIRS
STATIC_OBJDIRS: \
	$(STATIC_OUTDIR)/db \
	$(STATIC_OUTDIR)/port \
	$(STATIC_OUTDIR)/table \
	$(STATIC_OUTDIR)/util \
	$(STATIC_OUTDIR)/helpers/memenv
# 生成静态编译文件
$(STATIC_OUTDIR)/%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@
$(STATIC_OUTDIR)/libleveldb.a:$(STATIC_LIBOBJECTS)
	rm -f $@
	$(AR) -rs $@ $(STATIC_LIBOBJECTS)

# 生成可执行程序
$(STATIC_OUTDIR)/arena_test:util/arena_test.cc $(STATIC_LIBOBJECTS) $(TESTHARNESS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) util/arena_test.cc $(STATIC_LIBOBJECTS) $(TESTHARNESS) -o $@ $(LIBS)
$(STATIC_OUTDIR)/coding_test:util/coding_test.cc $(STATIC_LIBOBJECTS) $(TESTHARNESS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) util/coding_test.cc $(STATIC_LIBOBJECTS) $(TESTHARNESS) -o $@ $(LIBS)
$(STATIC_OUTDIR)/crc32c_test:util/crc32c_test.cc $(STATIC_LIBOBJECTS) $(TESTHARNESS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) util/crc32c_test.cc $(STATIC_LIBOBJECTS) $(TESTHARNESS) -o $@ $(LIBS)
$(STATIC_OUTDIR)/db_test:db/db_test.cc $(STATIC_LIBOBJECTS) $(TESTHARNESS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) db/db_test.cc $(STATIC_LIBOBJECTS) $(TESTHARNESS) -o $@ $(LIBS)
	
TESTS = \
	util/arena_test \
	util/coding_test \
	util/crc32c_test \
	db/db_test
PROGNAMES := $(notdir $(TESTS))
STATIC_PROGRAMS := $(addprefix $(STATIC_OUTDIR)/, $(PROGNAMES))
all: $(STATIC_OBJDIRS) $(STATIC_OUTDIR)/libleveldb.a $(STATIC_PROGRAMS)

run:
	make all
	cd /home/chieh/Documents/chiehdb/out-static && echo "==============\n" && ./db_test

# g++ -pthread -I. -I./include -O2 -DNDEBUG util/arena_test.cc out-static/db/db_impl.o out-static/db/log_write.o out-static/util/arena.o out-static/util/coding.o out-static/util/crc32c.o out-static/util/env_posix.o out-static/util/status.o out-static/util/testharness.o -o out-static/arena_test 