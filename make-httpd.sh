#!/bin/bash

export LIND_PREFIX="${LIND_PREFIX:-$HOME}"
export LIND_BASE="${LIND_BASE:-$LIND_PREFIX/lind_project}"
export LIND_SRC="${LIND_SRC:-$LIND_BASE/lind}"
export LIND_MONITOR="${LIND_MONITOR:-$LIND_BASE/reference_monitor}"
export REPY_PATH="${REPY_PATH:-$LIND_SRC/repy}"
export NACL_SDK_ROOT="${NACL_SDK_ROOT:-$REPY_PATH/sdk}"
export PYTHON="${PYTHON:-python2}"
export PNACLPYTHON="${PNACLPYTHON:-python2}"
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH:-/lib/glibc}"

export PACKAGE_NAME=httpd
export NACL_ARCH=x86_64
export NACL_GLIBC=1
export NACLPORTS_PREFIX=${REPY_PATH}/usr

source ../naclports/build_tools/common.sh

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

Banner "Build apr"
ChangeDir "../apr-1.4.8"
Remove "build-nacl"
MakeDir "build-nacl"
cd "build-nacl"
../configure --prefix=${NACLPORTS_PREFIX} --host="i686-linux" --enable-debug ac_cv_file__dev_zero=yes \
  ac_cv_func_setpgrp_void=yes apr_cv_epoll=yes apr_cv_tcp_nodelay_with_cork=yes \
  ac_cv_o_nonblock_inherited=no ac_cv_sizeof_struct_iovec=8
make V=1
make install
make clean
cd ".."

Banner "Build apr-util"
ChangeDir "../apr-util-1.5.2"
Remove "build-nacl"
MakeDir "build-nacl"
cd "build-nacl"
../configure --prefix=${NACLPORTS_PREFIX} --host="i686-linux" --with-apr=${NACLPORTS_PREFIX}
make V=1
make install
make clean
cd ".."

Banner "Build pcre"
ChangeDir "../pcre-8.33"
Remove "build-nacl"
MakeDir "build-nacl"
cd "build-nacl"
../configure --prefix=${NACLPORTS_PREFIX} --host="i686-linux"
make V=1
make install
make clean
cd ".."

Banner "Build httpd"
ChangeDir "../httpd-2.4.6"
Remove "build-nacl"
MakeDir "build-nacl"
cd "build-nacl"
../configure --prefix=${NACLPORTS_PREFIX} --host="i686-linux" --with-apr=${NACLPORTS_PREFIX} --with-apr-util=${NACLPORTS_PREFIX} --with-pcre=${NACLPORTS_PREFIX} \
    --enable-debugger-mode ap_cv_void_ptr_lt_long=no
make V=1
make install
make clean
cd ".."
