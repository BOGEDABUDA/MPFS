/home/dingbo/Desktop/qemu-7.2.0/build/qemu-system-x86_64 \
  -kernel /usr/src/linux-5.7/arch/x86/boot/bzImage -initrd /boot/initrd.img-5.7.0-050700rc7-generic \
  -append "root=/dev/sda5 nokaslr console=ttyS0" \
  -hda /home/dingbo/Desktop/qemu_env/qemu-img.qcow2 \
  -net user,hostfwd=tcp::6002-:22 -net nic,model=virtio \
  -s -nographic \
  -m 4096,slots=12,maxmem=900G -cpu host -enable-kvm -smp 32 \
  -machine pc,nvdimm=on \
  -object memory-backend-file,id=pmem,share=on,mem-path=/dev/dax0.0,size=700G,align=128M \
  -device nvdimm,id=nvdimm,memdev=pmem,label-size=2M
