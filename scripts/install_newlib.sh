#!/bin/bash -eux

# このバージョンでないとエラーが出るので注意されたし
NEWLIB=newlib-3.1.0

echo NEWLIB=${NEWLIB}

NEWLIB_BUILD_DIR=${HTOS_THIRD_PARTY}/build/${NEWLIB}
NEWLIB_SRC_DIR=${HTOS_THIRD_PARTY}/src/${NEWLIB}
NEWLIB_OUT_DIR=${HTOS_THIRD_PARTY}/out/x86_64-elf

echo NEWLIB_BUILD_DIR=${NEWLIB_BUILD_DIR}
echo NEWLIB_SRC_DIR=${NEWLIB_SRC_DIR}

# ソースコードの取得
cd $HTOS_THIRD_PARTY
if [ ! -e ${NEWLIB_BUILD_DIR} ]; then
    mkdir -p src
    wget -N http://sourceware.org/pub/newlib/${NEWLIB}.tar.gz -P src/
    tar -xvf src/${NEWLIB}.tar.gz -C src/
    rm -f src/${NEWLIB}.tar.gz
fi

CC=clang
AR=llvm-ar
RANLIB=llvm-ranlib

echo CC=${CC}
echo AR=${AR}
echo RANLIB=${RANLIB}

CFLAGS="-fPIC -target x86_64-unknown-none-elf -nostdlibinc -O2"

echo CFLAGS=${CFLAGS}

# コンパイル
mkdir -p ${NEWLIB_BUILD_DIR}
mkdir -p ${NEWLIB_OUT_DIR}
cd ${NEWLIB_BUILD_DIR}
${NEWLIB_SRC_DIR}/newlib/configure \
                 CC=${CC} \
                 CC_FOR_BUILD=${CC} \
                 --target=x86_64-elf --disable-multilib --disable-newlib-multithread \
                 --prefix=${NEWLIB_OUT_DIR} \
                 CFLAGS="$CFLAGS"
make clean
make -j8 install
