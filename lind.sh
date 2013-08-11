#!/bin/bash
# set -o xtrace
if [ -z "${REPY_PATH}" ]; then
  exit 1
fi

trap '${REPY_PATH}/bin/sel_ldr -a -s -- /lib/glibc/runnable-ld.so --library-path /lib/glibc:/usr/lib $@' INT TERM EXIT
