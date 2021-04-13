#!/bin/bash -eux

EDK2_DIR=${HTOS_ROOT}/third_party/edk2
EDK2_VERSION=stable202102

cd $HTOS_ROOT/third_party
if [ ! -e ${EDK2_DIR} ]; then
    mkdir -p src
    git clone --depth 1 --branch edk2-${EDK2_VERSION} https://github.com/tianocore/edk2.git ${EDK2_DIR}
fi

cd $EDK2_DIR
git submodule update --init
make -C BaseTools/Source/C
