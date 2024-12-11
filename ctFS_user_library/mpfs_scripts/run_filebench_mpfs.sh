#!/bin/bash

# clear pagecache
echo 'wojiaodingbo@123' | su -c 'echo 3 > /proc/sys/vm/drop_caches'
# manually clear /mnt/MPFS since MPFS now cannot support unlink/unlinkat for rm
cd ../test
sudo ./mkfs 1
cd ../mpfs_scripts
# change directory to enable ext4-DAX
sed -i "s|/mnt/pmem1|/mnt/MPFS|g" ../filebench-1.4.9.1/workloads/$1
cat ../filebench-1.4.9.1/workloads/$1 | grep /mnt/MPFS
# execute filebench
sudo ../test/run_mpfs.sh ../filebench-1.4.9.1/filebench -f ../filebench-1.4.9.1/workloads/$1
