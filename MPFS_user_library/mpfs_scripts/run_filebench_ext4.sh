#!/bin/bash

# clear pagecache
echo 'wojiaodingbo@123' | su -c 'echo 3 > /proc/sys/vm/drop_caches'
# manually clear /mnt/pmem1 since wrapper skips rm
sudo rm -rf /mnt/pmem1/* 
# change directory to enable ext4-DAX
sed -i "s|/mnt/MPFS|/mnt/pmem1|g" ../filebench-1.4.9.1/workloads/$1
cat ../filebench-1.4.9.1/workloads/$1 | grep /mnt/pmem1
# execute filebench
sudo ../test/run_ext4.sh ../filebench-1.4.9.1/filebench -f ../filebench-1.4.9.1/workloads/$1
