# Qemu and Raspberry Pi emulation (from: https://interrupt.memfault.com/blog/emulating-raspberry-pi-in-qemu)

used this to remove pre-installed qemu:
 ~~~
sudo apt-get autoremove qemu*
 ~~~

some requirements:
~~~
sudo apt-get install build-essential zlib1g-dev pkg-config libglib2.0-dev \
binutils-dev libboost-all-dev autoconf libtool libssl-dev libpixman-1-dev \
~~~
Install qemu:
  ~~~
sudo apt install qemu-system
~~~

Rpi image download and decompression
~~~
wget https://downloads.raspberrypi.org/raspios_arm64/images/raspios_arm64-2023-05-03/2023-05-03-raspios-bullseye-arm64.img.xz

xz -d 2023-05-03-raspios-bullseye-arm64.img.xz
~~~

Inspecting the partitions:
~~~
fdisk -l ./2023-05-03-raspios-bullseye-arm64.img

Disk ./2023-05-03-raspios-bullseye-arm64.img: 4.11 GiB, 4412407808 bytes, 8617984 sectors
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: dos
Disk identifier: 0x3e247b30

Device                                   Boot  Start     End Sectors  Size Id Type
./2023-05-03-raspios-bullseye-arm64.img1        8192  532479  524288  256M  c W95 FAT32 (LBA)
./2023-05-03-raspios-bullseye-arm64.img2      532480 8617983 8085504  3.9G 83 Linux
~~~

We need the Kernel and the device tree for the emulation, by mounting the first partition:
~~~
sudo mkdir /mnt/rasparm64
sudo mount -o loop,offset=$(( 8192 * 512)) /2023-05-03-raspios-bullseye-arm64.img /mnt/rasparm64/
~~~

Inspecting the mounted directory, we can see the /boot and device tree files:
~~~
ls -ls /mnt/image/
total 30244
  30 -rwxr-xr-x 1 root root   30390 Apr  5 21:32 bcm2710-rpi-2-b.dtb
  32 -rwxr-xr-x 1 root root   32142 Apr  5 21:32 bcm2710-rpi-3-b.dtb
  32 -rwxr-xr-x 1 root root   32753 Apr  5 21:32 bcm2710-rpi-3-b-plus.dtb
  30 -rwxr-xr-x 1 root root   30285 Apr  5 21:32 bcm2710-rpi-cm3.dtb
  32 -rwxr-xr-x 1 root root   31318 Apr  5 21:32 bcm2710-rpi-zero-2.dtb
  32 -rwxr-xr-x 1 root root   31318 Apr  5 21:32 bcm2710-rpi-zero-2-w.dtb
  52 -rwxr-xr-x 1 root root   52682 Apr  5 21:32 bcm2711-rpi-400.dtb
  52 -rwxr-xr-x 1 root root   52593 Apr  5 21:32 bcm2711-rpi-4-b.dtb
  52 -rwxr-xr-x 1 root root   53202 Apr  5 21:32 bcm2711-rpi-cm4.dtb
  38 -rwxr-xr-x 1 root root   38182 Apr  5 21:32 bcm2711-rpi-cm4-io.dtb
  50 -rwxr-xr-x 1 root root   50504 Apr  5 21:32 bcm2711-rpi-cm4s.dtb
  52 -rwxr-xr-x 1 root root   52476 Apr  5 21:32 bootcode.bin
   2 -rwxr-xr-x 1 root root     193 May  3 13:23 cmdline.txt
   4 -rwxr-xr-x 1 root root    2109 May  3 12:53 config.txt
  20 -rwxr-xr-x 1 root root   18693 Apr  5 21:32 COPYING.linux
~~~

So we copy them for qemu:
~~~
cp /mnt/rasparm64/bcm2710-rpi-3-b-plus.dtb ~/Desktop/arm64
cp /mnt/rasparm64/kernel8.img ~/Desktop/arm64/
~~~

Setting up SSH
~~~
openssl passwd -6
Password: [enter your passwrod]
Verifying - Pasword [enter again]
[you'll get a hash]

$ echo '[your hash]' | sudo tee /mnt/rasparm64/userconf

sudo touch /mnt/rasparm64/ssh
~~~

## Running qemu:
Resize img to the power of 2:
~~~
qemu-img resize /2023-05-03-raspios-bullseye-arm64.img 8G
~~~

Now run:
~~~
qemu-system-aarch64 -machine raspi3b -cpu cortex-a72 -nographic -dtb bcm2710-rpi-3-b-plus.dtb -m 1G -smp 4 -kernel kernel8.img -sd 2023-05-03-raspios-bullseye-arm64.img -append "rw earlyprintk loglevel=8 console=ttyAMA0,115200 dwc_otg.lpm_enable=0 root=/dev/mmcblk0p2 rootdelay=1" -device usb-net,netdev=net0 -netdev user,id=net0,hostfwd=tcp::2222-:22
~~~

## Conneting through SSH:
~~~
ssh -p 2222 pi@localhost
password: [enter your password]
~~~

