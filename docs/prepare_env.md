# preconfigure environment

defaule logi: pi
default pwd: raspberry

## configure keyboard:
sudo nano /etc/default/keyboard
in XKBLAYOUT change gb to us
sudo reboot

## configure network:
sudo rfkill unblock wifi
sudo ifconfig wlan0 up
add:
"""
etwork={
ssid="wifi_name"
psk="wifi_pass"
}
"""
to /etc/wpa_supplicant/wpa_supplicant.conf

## configure static ip:
add:
"""
auto wlan0
iface wlan0 inet static
  address 192.168.1.188
  netmask 255.255.255.0
  gateway 192.168.1.1
  wpa-conf /etc/wpa_supplicant/wpa_supplicant.conf
"""
to /etc/network/interfaces

## start ssh server on boot
sudo systemctl enable ssh

# install environment for machinepart

sudo apt update
sudo apt upgrade
sudo apt install wiringpi

## Optional
In latest version we no need OpenCV
TODO: fully deprecate OpenCV

## capture video 1 (OpenCV way)
### Python (required for OpenCV)
    sudo apt-get -y install python3-dev python3-pip
    sudo -H pip3 install -U pip numpy
    sudo apt-get -y install python3-testresources

### Other dependencies (biggest required for OpenCV)
    sudo apt -y update
    sudo apt -y upgrade
    sudo apt-get -y remove x264 libx264-dev

    sudo apt-get -y install build-essential checkinstall cmake pkg-config yasm
    sudo apt-get -y install git gfortran
    sudo apt-get -y install libjpeg8-dev libjasper-dev libpng12-dev
 
    sudo apt-get -y install libtiff5-dev
    
    sudo apt-get -y install libtiff-dev
    
    sudo apt-get -y install libavcodec-dev libavformat-dev libswscale-dev libdc1394-22-dev
    sudo apt-get -y install libxine2-dev libv4l-dev
    cd /usr/include/linux
    sudo ln -s -f ../libv4l1-videodev.h videodev.h
    cd $cwd
    
    sudo apt-get -y install libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev
    sudo apt-get -y install libgtk2.0-dev libtbb-dev qt5-default
    sudo apt-get -y install libatlas-base-dev
    sudo apt-get -y install libmp3lame-dev libtheora-dev
    sudo apt-get -y install libvorbis-dev libxvidcore-dev libx264-dev
    sudo apt-get -y install libopencore-amrnb-dev libopencore-amrwb-dev
    sudo apt-get -y install libavresample-dev
    sudo apt-get -y install x264 v4l-utils
 
### Optional dependencies
    sudo apt-get -y install libprotobuf-dev protobuf-compiler
    sudo apt-get -y install libgoogle-glog-dev libgflags-dev
    sudo apt-get -y install libgphoto2-dev libeigen3-dev libhdf5-dev doxygen

### Download and install OpenCV
    git clone https://github.com/opencv/opencv.git
    cd opencv
    git checkout $cvVersion
    cd ..
    
    git clone https://github.com/opencv/opencv_contrib.git
    cd opencv_contrib
    git checkout $cvVersion
    cd ..

    cd opencv
    mkdir build
    cd build

    cmake .. -DCMAKE_BUILD_TYPE=RELEASE \
             -DCMAKE_INSTALL_PREFIX=/usr/local \
             -DWITH_TBB=ON \
             -DWITH_V4L=ON \
             -DWITH_QT=ON \
             -DWITH_OPENGL=ON \
             -DOPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules

    make

    sudo make install
    sudo sh -c 'echo "/usr/local/lib" >> /etc/ld.so.conf.d/opencv.conf'
    sudo ldconfig

    // if any problem
    sudo sed -i 's/CONF_SWAPSIZE=1024/CONF_SWAPSIZE=100/g' /etc/dphys-swapfile
    sudo /etc/init.d/dphys-swapfile stop
    sudo /etc/init.d/dphys-swapfile start


    echo "sudo modprobe bcm2835-v4l2" >> ~/.profile

### capture video 2 (vgrabbj way)
    sudo apt install libv4l-dev
    sudo ln /usr/include/libv4l1-videodev.h /usr/include/linux/videodev.h