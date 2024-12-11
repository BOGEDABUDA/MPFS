#!/bin/bash

# clear pagecache
echo 'wojiaodingbo@123' | su -c 'echo 3 > /proc/sys/vm/drop_caches'
# change directory to enable ext4-DAX
sed -i "s|/mnt/pmem1|/mnt/MPFS|g" ./jobfile.fio
cat ./jobfile.fio | grep /mnt/MPFS
# clear existing files
cd ../test
sudo ./mkfs 1
cd ../mpfs_scripts
# execute fio
sudo ../test/run_mpfs.sh ../fio-fio-3.34/fio ./jobfile.fio
