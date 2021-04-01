#!/bin/bash -eux

LLVM_VERSION=10.0.0-rc6
LLVM_DIR=${HTOS_THIRD_PARTY}/src/llvm-${LLVM_VERSION}

cd $HTOS_ROOT/third_party
if [ ! -e ${LLVM_DIR} ]; then
    mkdir -p src
    git clone --depth 1 --branch llvmorg-${LLVM_VERSION} https://github.com/llvm/llvm-project.git ${LLVM_DIR}
fi

LIBCXXABI=libc++abi-${LLVM_VERSION}

LIBCXXABI_BUILD_DIR=${HTOS_ROOT}/scripts/build/${LIBCXXABI}
LIBCXXABI_SRC_DIR=${LLVM_DIR}/libcxxabi

NEWLIB_OUT_DIR=${HTOS_ROOT}/scripts/x86_64-elf
NEWLIB_INC_PATH=${NEWLIB_OUT_DIR}/include

LIBCXX_INC_PATH=${LLVM_DIR}/libcxx/include

mkdir -p ${LIBCXXABI_BUILD_DIR}
cd ${LIBCXXABI_BUILD_DIR}
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
    -DCMAKE_CXX_FLAGS="$COMMON_COMPILER_FLAGS" \
    -DCMAKE_C_COMPILER=${CC} \
    -DCMAKE_C_FLAGS="$COMMON_COMPILER_FLAGS" \
    -DCMAKE_LINKER=${LD_LLD} \
    -DCMAKE_RANLIB=${RANLIB} \
    -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
    -DLIBCXXABI_LIBCXX_INCLUDES="$LIBCXX_INC_PATH" \
    -DLIBCXXABI_ENABLE_THREADS=OFF \
    -DLIBCXXABI_ENABLE_EXCEPTIONS=OFF \
    -DLIBCXXABI_TARGET_TRIPLE=x86_64-unknown-none-elf \
    -DLIBCXXABI_ENABLE_SHARED=OFF \
    -DLIBCXXABI_ENABLE_STATIC=ON \
    -DLLVM_PATH=${LLVM_DIR} \
    -DCMAKE_INSTALL_PREFIX=${NEWLIB_OUT_DIR} \
    -DLIBCXXABI_USE_LLVM_UNWINDER=ON \
    -DLIBCXXABI_ENABLE_ASSERTIONS=OFF \
    -DLIBCXXABI_ENABLE_NEW_DELETE_DEFINITIONS=OFF \
    -DLIBCXXABI_BAREMETAL=ON \
    ${LIBCXXABI_SRC_DIR}
make clean
make -j8 install
