#! /bin/bash
# set -x

#! note that the configuration of ld should be modified by 'sudo vim /etc/ld.so.conf.d/x86_64-linux-gnu.conf' and 'sudo ldconfig'

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <fs type> <number_of_processes>"
    exit 1
fi

FSTYPE=$1
NUM_PROCESSES=$2

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

# prepare files to be read
start=$(date +%s%N)
for ((i = 0; i < $NUM_PROCESSES; i++)); do
    sudo $EXECUTION ../mpfs_test $PARAMETER 1 $i &
done

wait
end=$(date +%s%N)
time_taken=$((end - start))
echo "all files are prepared in $time_taken nanoseconds"

start=$(date +%s%N)

for ((i = 0; i < $NUM_PROCESSES; i++)); do
    sudo $EXECUTION ../mpfs_test $PARAMETER 3 $i &
done

wait
end=$(date +%s%N)
# set +x

time_taken=$((end - start))
echo "Time taken: $time_taken nanoseconds"
echo "Time taken in seconds: $(bc <<<"scale=9; $time_taken/1000000000")"
