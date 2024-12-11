/home/dingbo/Desktop/qemu-7.2.0/build/qemu-system-x86_64 \
  -kernel /home/dingbo/Desktop/linux-5.7.1-MPFS/arch/x86/boot/bzImage \
  -initrd /boot/initrd.img-5.7.1+ \
  -append "root=/dev/sda5 nokaslr console=ttyS0" \
  -hda /home/dingbo/Desktop/qemu_env/qemu-img.qcow2 \
  -net user,hostfwd=tcp::6004-:22 -net nic,model=virtio \
  -gdb tcp::1122 \
  -nographic \
  -m 40960,slots=12,maxmem=900g \
  -cpu host -enable-kvm -smp 52 \
  -machine pc,accel=kvm,nvdimm=on,nvdimm-persistence=cpu \
  -object memory-backend-file,id=mem1,share=on,mem-path=/mnt/pmem1/pmem_qemu,size=700G,pmem=on \
  -device nvdimm,id=nvdimm1,memdev=mem1,label-size=256K

  
