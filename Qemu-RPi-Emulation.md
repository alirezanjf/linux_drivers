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
~~~

