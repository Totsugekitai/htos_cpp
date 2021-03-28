### Makefile_template ###

# don't use TAB
.RECIPEPREFIX = >
# change shell to bash
SHELL := bash
# shell flags
.SHELLFLAGS := -eu -o pipefail -c
# one shell for one target rule
.ONESHELL:
# warning undefined variables
MAKEFLAGS += --warn-undefined-variables
# delete intermediate files on error
.DELETE_ON_ERROR:
# delete implicit rules
MAKEFLAGS += -r

# MAKEFILE_DIR is directory Makefile located in
MAKEFILE_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

### Makefile_template end ###

TARGET := htos.elf
ENTRY_POINT := kernel_entry

LIBCXX_INCLUDE_PATH := $(HTOS_THIRD_PARTY)/out/x86_64-elf/include/c++/v1

CXX := clang++
CXXFLAGS += -O2 -Wall --target=x86_64-elf -ffreestanding -mno-red-zone -fno-exceptions -fno-rtti -std=c++17 -I$(LIBCXX_INCLUDE_PATH) -nostdlibinc -D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS

LD := ld.lld
LDFLAGS += -T kernel/linker.ld -z norelro -static

OBJS := kernel/main.o

.PHONY: $(TARGET)
$(TARGET): $(OBJS) Makefile
> $(LD) $(LDFLAGS) -o $@ $(OBJS)

%.o: %.cpp Makefile
> $(CXX) $(CXXFLAGS) -o $@ -c $<

.PHONY: kernel
kernel: Makefile
> make $(TARGET)

.PHONY: clean
clean: Makefile
> rm -rf kernel/*.o
