# Linux Kernel Netfilter Framework Module

### Setup
* 2 VMs with separate NAT adapters & host-only adapters to get them in the same subnet
* Both of them running bare-bones Artix Linux with runit
* Kernel version - 5.10.90-1-lts
* VM1 \[50.0.0.3] <-----> \[50.0.0.4] VM2

#### On VM 2
* Do a full system upgrade -> $ `sudo pacman -Syu`
* Install linux-lts-headers -> $ `sudo pacman -S linux-lts-headers`
* Install glibc ->​ `$ sudo pacman -S glibc`
* Reboot







