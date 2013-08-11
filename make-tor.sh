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

Banner "Configure zlib"
ChangeDir "../zlib-1.2.7"
CC=${NACLCC} AR="${NACLAR}" RANLIB=${NACLRANLIB} CFLAGS="-Dunlink=puts" ./configure\
     --prefix=${NACLPORTS_PREFIX} -s
make clean
make
make install
make clean

Banner "Configure openssl"
ChangeDir "../openssl-1.0.1e"
./config -d\
     --prefix=${NACLPORTS_PREFIX} no-asm no-hw no-krb5 shared -D_GNU_SOURCE
make clean
make build_libs
make install
make clean

Banner "Configure libevent"
ChangeDir "../libevent-2.0.21-stable"
Remove "build-nacl"
MakeDir "build-nacl"
cd "build-nacl"
../configure --prefix=${NACLPORTS_PREFIX} --host="i686-linux"
make
make install
make clean
ChangeDir ".."

Banner "Build tor"
ChangeDir "../tor-0.2.3.25"
Remove "build-nacl"
MakeDir "build-nacl"
cd "build-nacl"
../configure --prefix=${NACLPORTS_PREFIX} --host=${conf_host} --disable-transparent
make
make install
make clean
cd ".."

