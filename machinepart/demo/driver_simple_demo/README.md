# prepare environment
sudo apt-get install raspberrypi-kernel-headers

# create device
for create test device please use:
mknod /dev/test c 77 0

# insert module
insmod test.ko

# remove module
rmmod test

# module info
modinfo test.ko

# testing
Please use test.sh script for testing read/write ops in drv
