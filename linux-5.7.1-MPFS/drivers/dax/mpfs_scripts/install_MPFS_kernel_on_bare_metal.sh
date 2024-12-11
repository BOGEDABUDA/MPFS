cd /home/dingbo/Desktop/linux-5.7.1-MPFS/
make -j52
make bzImage -j16
sudo make install
make O=$BUILD modules
sudo make O=$BUILD modules_install
sudo make O=$BUILD install
sudo update-grub
sudo update-grub2

# remeber to change the boot order by modifying the /etc/default/grub file