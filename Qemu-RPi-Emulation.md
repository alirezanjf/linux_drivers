# Qemu and Raspberry Pi emulation

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

-Rpi image from here: https://www.raspberrypi.com/software/operating-systems/ and decompress with:
~~~
xz --decompress 2024-11-19-raspios-bookworm-arm64-full.img.xz 
~~~
