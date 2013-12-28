#!/bin/bash

export NACL_ARCH=x86_64
export NACL_GLIBC=1
export NACLPORTS_PREFIX=${REPY_PATH}/usr

source ../naclports/src/build_tools/common.sh

export NACL_SEL_LDR
export NACL_IRT
export NACL_SDK_LIB
export CC=${NACLCC}
export CXX=${NACLCXX}
export AR=${NACLAR}
export RANLIB=${NACLRANLIB}
export PKG_CONFIG_PATH=${NACLPORTS_LIBDIR}/pkgconfig
export PKG_CONFIG_LIBDIR=${NACLPORTS_LIBDIR}
export FREETYPE_CONFIG=${NACLPORTS_PREFIX_BIN}/freetype-config
export PATH=${NACL_BIN_PATH}:${PATH};
conf_host=${NACL_CROSS_PREFIX}

Banner "Build curl-7.33.0"
ChangeDir "../curl-7.33.0"
Remove "build-nacl"
MakeDir "build-nacl"
cd "build-nacl"
../configure --prefix=${NACLPORTS_PREFIX} --host="i686-linux"
make V=1
make install
make clean
cd ".."

Banner "Build cpuminer"
ChangeDir "../cpuminer-2.3.2"
Remove "build-nacl"
MakeDir "build-nacl"
cd "build-nacl"
LIBS="-lssl -lcrypto -lz" ../configure --prefix=${NACLPORTS_PREFIX} --host="i686-linux" --with-libcurl=${NACLPORTS_PREFIX}
make V=1
make install
make clean
cd ".."

