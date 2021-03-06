#!/bin/bash -eux

LLVM_VERSION=10.0.0-rc6
LLVM_DIR=${HTOS_THIRD_PARTY}/src/llvm-${LLVM_VERSION}

cd $HTOS_ROOT/third_party
if [ ! -e ${LLVM_DIR} ]; then
    mkdir -p src
    git clone --depth 1 --branch llvmorg-${LLVM_VERSION} https://github.com/llvm/llvm-project.git ${LLVM_DIR}
fi

LIBCXX=libc++-${LLVM_VERSION}
LIBCXX_BUILD_DIR=${HTOS_ROOT}/third_party/build/${LIBCXX}
LIBCXX_SRC_DIR=${LLVM_DIR}/libcxx

NEWLIB_OUT_DIR=${HTOS_ROOT}/third_party/x86_64-elf
NEWLIB_INC_PATH=${NEWLIB_OUT_DIR}/include

mkdir -p ${LIBCXX_BUILD_DIR}
cd ${LIBCXX_BUILD_DIR}
rm -rf CMakeCache.txt

CC=clang
CXX=clang++
AR=/usr/bin/llvm-ar
RANLIB=/usr/bin/llvm-ranlib
LD_LLD=/usr/bin/ld.lld

COMMON_COMPILER_FLAGS="\
        -nostdlibinc -I$NEWLIB_INC_PATH \
        -D__ELF__ \
        -D_LDBL_EQ_DBL -D_LIBCPP_HAS_NO_THREADS -D_LIBCPP_HAS_NO_LIBRARY_ALIGNED_ALLOCATION \
        "

cmake \
    -DCMAKE_AR=${AR} \
    -DCMAKE_CXX_COMPILER=${CXX} \
    -DCMAKE_CXX_COMPILER_TARGET=x86_64-unknown-none-elf \
    -DCMAKE_CXX_FLAGS="$COMMON_COMPILER_FLAGS -I${NEWLIB_INC_PATH} -D_LIBUNWIND_IS_BAREMETAL=1 -D_GNU_SOURCE -D_POSIX_TIMERS" \
    -DCMAKE_C_COMPILER=${CC} \
    -DCMAKE_C_FLAGS="$COMMON_COMPILER_FLAGS -I${NEWLIB_INC_PATH} -D_LIBUNWIND_IS_BAREMETAL=1 -D_GNU_SOURCE -D_POSIX_TIMERS" \
    -DCMAKE_INSTALL_PREFIX=${NEWLIB_OUT_DIR} \
    -DCMAKE_LINKER=${LD_LLD} \
    -DCMAKE_RANLIB=${RANLIB} \
    -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
    -DLIBCXX_CXX_ABI=libcxxabi \
    -DLIBCXX_CXX_ABI_INCLUDE_PATHS="${LLVM_DIR}/libcxxabi/include" \
    -DLIBCXX_CXX_ABI_LIBRARY_PATH="${LLVM_DIR}/libcxxabi/lib" \
    -DLIBCXX_ENABLE_FILESYSTEM=False \
    -DLIBCXX_ENABLE_MONOTONIC_CLOCK=False \
    -DLIBCXX_ENABLE_RTTI=False \
    -DLIBCXX_ENABLE_SHARED=False \
    -DLIBCXX_ENABLE_STATIC=True \
    -DLIBCXX_ENABLE_THREADS=False \
    -DLIBCXX_ENABLE_EXCEPTIONS=False \
    -DLLVM_PATH=${LLVM_DIR} \
    ${LIBCXX_SRC_DIR}
make clean
make -j8 install
