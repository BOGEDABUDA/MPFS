#!/bin/bash

# clear pagecache
echo 'wojiaodingbo@123' | su -c 'echo 3 > /proc/sys/vm/drop_caches'
# change directory to enable ext4-DAX
sed -i "s|/mnt/MPFS|/mnt/pmem1|g" ./jobfile.fio
cat ./jobfile.fio | grep /mnt/pmem1
# execute fio
sudo ../test/run_ext4.sh ../fio-fio-3.34/fio ./jobfile.fio