# HTOS

## library build & install

```shell
export LIB_BUILD_SRC=/path/to/src
export LIB_INSTALL=/path/to/install
```

### newlib

```shell
$ cd $LIB_BUILD_SRC
$ git clone git://sourceware.org/git/newlib-cygwin.git newlib
$ cd newlib
$ mkdir build && cd build
$ ../newlib/configure CC=clang CC_FOR_BUILD=clang CFLAGS="-fPIC -nostdlibinc -O2 -D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS" --target=x86_64-elf --prefix=$LIB_INSTALL --disable-multilib --disable-newlib-multithread
```

## libc++

```shell
$ export LIBCXX_PATH=$(LIB_BUILD_SRC)/libc++
```

```shell
$ git clone https://github.com/llvm/llvm-project.git
$ cd llvm-project
$ mkdir build && cd build
$ cmake -DCMAKE_C_COMPILER=clang \
        -DCMAKE_CXX_COMPILER=clang++ \
        -DLLVM_ENABLE_PROJECTS="libcxx;libcxxabi" \
        -DCMAKE_INSTALL_PREFIX=$LIBCXX_PATH \
        ../llvm
$ make # Build
$ make check-cxx # Test
$ make install-cxx install-cxxabi # Install
```

