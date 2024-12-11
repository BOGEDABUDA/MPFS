#! /bin/bash
set -x

#! note that the configuration of ld should be modified by 'sudo vim /etc/ld.so.conf.d/x86_64-linux-gnu.conf' and 'sudo ldconfig'

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <fs type>"
    exit 1
fi

FSTYPE=$1

if [ "$FSTYPE" = "mpfs" ]; then
    cd ../test
    sudo ./mkfs 1 # reset the file system
    cd ../mpfs_scripts
    EXECUTION="../test/run_mpfs.sh"
    PARAMETER="/mnt/MPFS"
elif [ "$FSTYPE" = "ext4" ]; then
    sudo umount /dev/pmem1
    echo y | sudo mkfs -t ext4 /dev/pmem1
    sudo mount -t ext4 -o dax /dev/pmem1 /mnt/pmem1
    EXECUTION="../test/run_ext4.sh"
    PARAMETER="/mnt/pmem1 "
else
    echo "Invalid FSTYPE"
    exit 1
fi

# prepare the file to be zipped
#! note that /dev/shm should be mounted in the format of tmpfs
if [ -f /dev/shm/tempfile ]; then
    echo "File already exists. Skipping creation."
else
    dd if=/dev/urandom of=/dev/shm/tempfile bs=1M count=5120
fi

start=$(date +%s%N)

sudo  $EXECUTION strace -o log.strace zip -q -r $PARAMETER/test.zip /dev/shm/tempfile

wait
end=$(date +%s%N)
time_taken=$((end - start))
echo "the tempfile is zipped in $time_taken nanoseconds"
echo "Time taken in seconds: $(bc <<<"scale=9; $time_taken/1000000000")"

set +x
