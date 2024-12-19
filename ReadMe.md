# Build and Runtime requirements

- sudo apt update
- sudo apt upgrade
- sudo apt install libgl1-mesa-dev libwayland-dev libxkbcommon-dev wayland-protocols libegl1-mesa-dev
- sudo apt install libc++-dev libepoxy-dev libglew-dev libeigen3-dev cmake g++ ninja-build
- sudo apt install libjpeg-dev libpng-dev catch2
- sudo apt install libavcodec-dev libavutil-dev libavformat-dev libswscale-dev libavdevice-dev
- sudo apt install libdc1394-dev libraw1394-dev libopenni-dev python3-dev
- sudo apt-get install cmake libopencv-dev libeigen3-dev qtbase5-dev libqt5serialport5-dev libboost1.74-dev libboost-serialization1.74-dev libcamera-apps

# How to run

- Open a terminal in the bin folder of the binaries
- Make sure its executable by running command "chmod +x MotMot"
- Run command "./MotMot <path_to_vocabfile> <path_to_config_file> false <imu_serial_port>"
- The vocab file is present in the config folder, unzip the provided tar to get the file and use that
- NOTE: An example config yaml file has been provided as a reference in config folder, the Calibration_Tutorial.pdf provides the steps to write yaml file for the camera, that file needs to be created for raspi cam v3 and IMU sensor, then used with the application otherwise SLAM will not work
- It is assumed that IMU provides on serial port data in the following format 
"0.55.., 0.55.., 0.55.., 0.55.., 0.55.., 0.55.." [mx, my, mz, ay, ay, az] with units rad/s and m/s^2

# How to build

- Create a build folder in the root folder of the project
- Open a terminal inside the build folder and run "cmake -DCMAKE_INSTALL_PREFIX=../install .."
- Then run "make -j2 install" 
- This will build and install in <project_root>/install folder
- Run the executable