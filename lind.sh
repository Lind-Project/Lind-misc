#!/bin/bash
# set -o xtrace
trap 'python ${REPY_PATH}/repy/repy.py --safebinary ${REPY_PATH}/repy/restrictions.lind ${REPY_PATH}/repy/lind_server.py $@' INT TERM EXIT
