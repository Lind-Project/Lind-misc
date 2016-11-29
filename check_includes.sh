#! /bin/bash
# Given a repy file with preprocessor directives, check they are all valid


f=$1
file_path=`dirname $f`
file_name=`basename $f`

files=`grep -e ^include $f | awk '{print $2}'|tr "\\n" " "`

rc=0

for file in $files 
do
    real_f=$file_path/$file
    fs_files=$file_path/fs/$file
    net_files=$file_path/net/$file
    sys_files=$file_path/sys/$file
    lind_files=$file_path/lind/$file
    xmlrpc_files=$file_path/xmplrpc/$file
    librepy_files=$file_path/librepy/$file

    if [ ! -f $real_f ] && \
       [ ! -f $fs_files ] && \
       [ ! -f $net_files ] && \
       [ ! -f $sys_files ] && \
       [ ! -f $lind_files ] && \
       [ ! -f $xmlrpc_files ] && \
       [ ! -f $librepy_files ]
    then
	echo "Error: include $file is missing in $file_name"
	rc=1
    fi
done 

exit $rc
