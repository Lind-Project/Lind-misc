#!/bin/sh

# check for xtrace flag
while getopts :hx opt; do
	case "$opt" in
	x)
		set -o xtrace
		;;
	h)
		printf 'usage: %s [-x] <lind program path>\n' "${0##*/}" >&2
		exit
		;;
	:|?)
		printf 'illegal option letter: %s\n' "'$OPTARG'" >&2
		printf 'usage: %s [-x] <lind program path>\n' "${0##*/}" >&2
		exit 1
		;;
	esac
done
shift "$((OPTIND - 1))"

export LIND_BASE="${LIND_BASE:-/usr/lind_project}"
export LIND_SRC="${LIND_SRC:-/usr/lind_project/lind}"
export LIND_MONITOR="${LIND_MONITOR:-/usr/lind_project/reference_monitor}"
export NACL_SDK_ROOT="${NACL_SDK_ROOT:-/usr/lind_project/lind/repy/sdk}"
export REPY_PATH="${REPY_PATH:-/usr/lind_project/lind/repy}"
export PYTHON="${PYTHON:-python2}"
export PNACLPYTHON="${PNACLPYTHON:-python2}"
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH:-/lib/glibc}"

cwd="$PWD"
trap 'cd "$cwd"' EXIT

cd "$REPY_PATH/repy" || exit 1

sel_ldr -a -- "$LD_LIBRARY_PATH/runnable-ld.so" --library-path "$LD_LIBRARY_PATH" "$@"

