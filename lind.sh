#!/bin/bash

# set -o xtrace

if test -z "$REPY_PATH"; then
  exit 1
fi

cwd="$PWD"
cd "$REPY_PATH/repy" || exit 1

trap '{ cd "$cwd"; trap - INT; kill -INT "$$"; } ' INT
trap '{ cd "$cwd"; trap - EXIT; kill -EXIT "$$"; } ' EXIT

sel_ldr -a -- /lib/glibc/runnable-ld.so --library-path /lib/glibc "$@"
