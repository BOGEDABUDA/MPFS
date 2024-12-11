#!/bin/bash
# set -x

# Ensure the correct number of arguments are passed
if [[ $# -ne 2 ]]; then
    echo "Usage: $0 <symbol> <offset>"
    exit 1
fi

# Get the symbol and offset from the command line
symbol=$1
offset=$2
vmlinux=/home/dingbo/Desktop/linux-5.7.1-MPFS/vmlinux

# Find the address of the symbol in vmlinux
address=$(nm -n $vmlinux | grep "\<$symbol\>" | awk '{print $1}')

# Add the offset to the address
address=$(printf "0x%x" $(( 0x$address + $offset )))

# Get the line with 'addr2line'
addr2line -e $vmlinux $address
