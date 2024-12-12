# Build and Runtime requirements

- sudo apt-get install libopencv-dev libeigen3-dev qtbase5-dev libqt5serialport5 libpython2.7-dev

# How to run

- Open a terminal in the bin folder of the binaries
- Make sure its executable by running command "chmod +x MotMot"
- Run command "./MotMot <path_to_vocabfile> <path_to_config_file> false <imu_serial_port>"
- The vocab file is present in the config folder, unzip the provided tar to get the file and use that
- An example config yaml file has been provided as a reference in config folder, the Calibration_Tutorial.pdf provides the steps to write yaml file for the camera, that file needs to be created for raspi cam v3 and then used with the application
- It is assumed that IMU provides on serial port data in the following format 
"0.55.., 0.55.., 0.55.., 0.55.., 0.55.., 0.55.." [mx, my, mz, ay, ay, az] with units rad/s and m/s^2

# How to build

- Create a build folder in the root folder of the project
- Open a terminal inside the build folder and run "cmake -DCMAKE_INSTALL_PREFIX=../install .."
- Then run "make -j2 install" 
- This will build and install in <project_root>/install folder
- Run the executable