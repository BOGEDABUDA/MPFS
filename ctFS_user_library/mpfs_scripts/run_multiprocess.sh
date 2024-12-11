#! /bin/bash
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <number_of_processes>"
    exit 1
fi

NUM_PROCESSES=$1

# set -x
cd ../test
sudo ./mkfs 1
cd ../mpfs_scripts
EXECUTION="../test/run_ctfs.sh"
PARAMETER="/mnt/MPFS"

# prepare files to be read
start=$(date +%s%N)
for ((i = 0; i < $NUM_PROCESSES; i++)); do
    sudo $EXECUTION ../mpfs_test $PARAMETER 1 $i &
done

wait
end=$(date +%s%N)
time_taken=$((end - start))
echo "all files are prepared in $time_taken nanoseconds"
# start
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
