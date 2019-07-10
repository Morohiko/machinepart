# headmachine

## glassespart
### Required 
    min sdk version 27 (Android 8.1.0, Oreo)
    target sdk version 28 (Android 9, Pie)

### Android Feature/Issue
    For create Socket, need create Assync Tast (something like thread).
    For send info to server (machinepart) need create socket and thread for it.
    For working with OpenCV need create thread.

### Threads/modules
 - thread for tcp/udp socket for receive info from camera
 - thread for convert and record image (OpenCV processing)
 - thread for get Gyroscope data
 - thread for send gyroscope data to server (machinepart)
 - thread for activity

### Activity

#### Debug Activity
    Need for debugging all parts of project separately

#### Config Activity
    Need for chaning configuration of project, like ip, port of local and target

## machinepart

### TODO: Modules (required)

#### Overview

Modules its part of project, must running as separate thread

#### Communication through modules
#### proposal 1
Communication can be like `sharing one memory`.
In that case need avoid access to that memory from different modules at the same time.
Make variable like `busy`. When write/read to memory, install variable to true, if another memory want to write/read then he need to check busy variable. Something like `condition variable`.

#### Core (module & main thread(main() ) )
 - start Camera module thread
 - start Camera data transmitter thread
 - start Gyroscope data receiver thread
 - start Motor Controller thread
 
#### Camera module (module & thread)
 - get stream from camera (must have access to camera)
 - convert it to bytes array (in this module because its need to do in OpenCV scope)
 - save it to Camera memory struct (as bytes array)

#### Camera data transmitter (module & thread)
 - get data from Camera memory struct
 - send struct to client (glassespart) (TCP or UDP ??????)

#### Gyroscope data receiver (module & thread)
 - recv data from client part as byte array (glasspart) (TCP or UDP ?????)
 - convert data to X, Y, Z coordinates (can be in Motor controller module)
 - save to Motor memory struct

#### Motor controller (module & thread)
 - get data from memory struct
 - make interrupt for motor start move to possition based on memory struct (mocked cause not have real motor) ?

### Preinstall environment

#### CMake
    sudo apt install cmake

#### Python (required for OpenCV)
    sudo apt-get -y install python3-dev python3-pip
    sudo -H pip3 install -U pip numpy
    sudo apt-get -y install python3-testresources

#### Other dependencies (biggest required for OpenCV)
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
 
#### Optional dependencies
    sudo apt-get -y install libprotobuf-dev protobuf-compiler
    sudo apt-get -y install libgoogle-glog-dev libgflags-dev
    sudo apt-get -y install libgphoto2-dev libeigen3-dev libhdf5-dev doxygen

#### Download and install OpenCV
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

    cmake -D CMAKE_BUILD_TYPE=RELEASE \
                -D CMAKE_INSTALL_PREFIX=$cwd/installation/OpenCV-"$cvVersion" \
                -D INSTALL_C_EXAMPLES=ON \
                -D INSTALL_PYTHON_EXAMPLES=ON \
                -D WITH_TBB=ON \
                -D WITH_V4L=ON \
                -D OPENCV_PYTHON3_INSTALL_PATH=$cwd/OpenCV-$cvVersion-py3/lib/python3.5/site-packages   \
            -D WITH_QT=ON \
            -D WITH_OPENGL=ON \
            -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules \
            -D BUILD_EXAMPLES=ON ..

    make -j$(nproc)

    make install

    sudo sed -i 's/CONF_SWAPSIZE=1024/CONF_SWAPSIZE=100/g' /etc/dphys-swapfile
    sudo /etc/init.d/dphys-swapfile stop
    sudo /etc/init.d/dphys-swapfile start

    echo "sudo modprobe bcm2835-v4l2" >> ~/.profile


    ### Demo

#### Camera demo
Provide example of functional for using camera sources.
Sources used OpenCV module for access to camera, getting Mat struct from them
and convert it to bytes array.

#### Wifi TCP demo

Provide example of functional for using communication with client part through TCP and send/receive plain text.

#### Wifi UDP Demo

Provide example of functional for using communication with client part through UDP and send/receive plain text.

#### Gyroscope data receiver demo

Provide example of functional for using receive gyroscope data from client and try parse it to x, y, z coordinates.

