# The Pigeon Project
A simple C++ Unix Socket Netowork library. 
>"Pigeon library, network made easy!"

## About this software

This software contains the tools to deal with network socket in a simple way. It will enable the user to integrate the network functionality rapidly into their project, and build some functionality on top of the transport layer of the Open System Interconnection (OSI) model.

>The original purpose was to let C++ applications communicate with the python scripts, and livestream the numerical simulation over the Internet.  This is only the first step. For the interprocess communication, because one work with the same machine, so the representation of floating-point numbers isn't an issue. However, floating-point numbers are represented in different binary forms for different machine architectures. To overcome this difficulty, one would need data serialization tools. I believe there're plenty of them that have been developed.

## Installation 

The installation instructions in this section is only tested on Debian GNU/Linux 11 (bullseye) distribution.

### To install this software from source

#### Installation
Navigate to the root directory of this repository, execute the following commands:
```
$ mkdir build
$ cd build
$ cmake ..
$ make
$ sudo make install
```

#### Uninstall if the software is installed from source
Navigate to the `build` directory created during the building process, executing the following commands:
```
$ sudo xargs rm <install_manifest.txt
```
### To build the debian package from source

Navigate to the root directory of this repository, executing the following commands:
```
$ mkdir build
$ cd build
$ cmake ..
$ make
$ cpack
```

## Application Notes
