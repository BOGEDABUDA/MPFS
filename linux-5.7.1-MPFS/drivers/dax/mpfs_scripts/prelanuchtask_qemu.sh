set -x
# exit the last execution
tmux kill-session -t "qemu_gdb"

# rebuild kernel
cd /home/dingbo/Desktop/linux-5.7.1-MPFS
make -j52
cd /home/dingbo/Desktop/linux-5.7.1-MPFS/drivers/dax/mpfs_scripts

# use tmux to execute qemu
tmux new -s "qemu_gdb" -d
tmux send -t "qemu_gdb" "sudo ./run_qemu.sh" Enter
# enter the password
tmux send -t "qemu_gdb" "wojiaodingbo@123" Enter
set +x

# use "tmux attach -t qemu_gdb" to access kernel started by qemu