#!/bin/sh
set -e
set -u
#set -x

# convert od -x output to s file with array ready for assembly

input="$1"

size=`ls -l -L "$input" | awk '{print $5}'`

basename="`echo "$input" | cut -d'.' -f1`"

echo ".data"
echo ".globl ${basename}_len"
echo "${basename}_len:"
echo ".long $size"

echo ".globl ${basename}"
echo "${basename}:"

handle_line ()
{
    #echo "# $*"
    shift
    while [ $# -gt 0 ]; do
    echo ".value 0x$1"
    shift
    done
}

od -xv "$input" | while read line; do
    handle_line $line
done
