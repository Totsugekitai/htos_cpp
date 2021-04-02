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

LIBCXX_INCLUDE_PATH := $(HTOS_ROOT)/third_party/x86_64-elf/include/c++/v1
NEWLIB_INCLUDE_PATH := $(HTOS_ROOT)/third_party/x86_64-elf/include
LIBPATH := $(HTOS_ROOT)/third_party/x86_64-elf/lib

CC := clang
CFLAGS += -O2 -Wall --target=x86_64-elf -ffreestanding -mno-red-zone -std=c11 -I$(LIBCXX_INCLUDE_PATH) -I$(NEWLIB_INCLUDE_PATH) -nostdlibinc -D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS

CXX := clang++
CXXFLAGS += -O2 -Wall --target=x86_64-elf -ffreestanding -mno-red-zone -fno-exceptions -fno-rtti -std=c++17 -I$(LIBCXX_INCLUDE_PATH) -I$(NEWLIB_INCLUDE_PATH) -I. -nostdlibinc -D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS

AS := nasm

LD := ld.lld
LDFLAGS += -T linker.ld -L$(LIBPATH) -z norelro -static

OBJS := main.o graphics.o font.o fonts.gen.o newlib_support.o console.o asmfunc.o pci.o logger.o
LIBS := $(LIBPATH)/libc++.a $(LIBPATH)/libc++abi.a $(LIBPATH)/libc.a
USBS := $(wildcard usb/*.o)

.PHONY: default
default: Makefile
> make kernel

.PHONY: $(TARGET)
$(TARGET): $(OBJS) $(LIBS) $(USBS) Makefile
> $(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBS) $(USBS)

%.o: %.cpp Makefile
> $(CXX) $(CXXFLAGS) -o $@ -c $<

%.o: %.c Makefile
> $(CC) $(CFLAGS) -o $@ -c $<

%.o: %.asm Makefile
> $(AS) -f elf64 -o $@ $<

.PHONY: kernel
kernel: Makefile
> make $(TARGET)

.PHONY: clean
clean: Makefile
> rm -rf *.o
> rm -rf */*.o
