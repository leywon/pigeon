# The 🕊️ Pigeon 🕊️  Project
A simple C++ Unix Socket Netowork library. 
>"Pigeon library, network made easy!"

## About this software

This software contains the tools to deal with network socket in a simple way. It will enable the user to integrate the network functionality rapidly into their project, and build softwares on top of the transport layer of the Open System Interconnection (OSI) model.

>The original purpose was to let C++ applications communicate with the python scripts, and livestream the numerical simulation over the Internet.  This is only the first step. For the interprocess communication, because one would work with the same computer, so the representation of floating-point numbers isn't an issue. However, floating-point numbers are represented in different binary forms under different computer architectures. To overcome this difficulty, one would need data serialization tools. I believe there're plenty of them that have been developed.

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

#### Uninstallation (if the software is installed from source)
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
## Things to know when dealing with network programming
### About Firewall
The ports on **computers** which are running as a server (*passively* accepting incoming connection requests that *are actively sent* by the clients) must be opened before the establishment of the connection. Usually these ports are blocked by the firewall.

Objects initialized from the classes `serverSideTCP` and `serverSideUDP`, which provide the functionality of a TCP or a UDP server, will not work if the ports they are binding to is blocked by the firewall.
##### Here are example for how to deal with ports on the firewall with the software `firewalld`
Show a list of ports that are opened by the firewall:
```
$ sudo firewall-cmd --list-port
```
Open a port 2022 for TCP service:
```
$ sudo firewall-cmd --add-port=2022/tcp
```
Open a port 2022 for UDP service:
```
$ sudo firewall-cmd --add-port=2022/udp
```
Close a port 2022 for TCP service:
```
$ sudo firewall-cmd --remove-port=2022/tcp
```
Close a port 2022 for UDP service:
```
$ sudo firewall-cmd --remove-port=2022/udp
```
To make the change permanent, attach `--permanent` flag at the end.
>Close ports as much as possible, only open them if it is absolutely necessary for the program to run (e.g., running the programs compiled from the `examples` folders, without opening these ports, the program will not be able to send information outside: they don't have the root privilege). **Always make it clear what to do before opening a port, and close it after using it** 

### About the Termination of TCP connection
To terminate the TCP connection, one end of the connection should receive another end's `FIN` signal (handled by the Linux kernel) before closing the connection. If both ends of the connection close simultaneously, then the `FIN` signal will not reach each other, and the port will be blocked by the kernel. The time it is blocked after such abruption is subjected to the kernel's discretion.

### How to check the IP address information
To show the computer's IP address information, execute in the terminal:
```
$ ip a
```
To find the broadcast IPv4 address, look for the keyword `brd`

### How to check ative port
Execute in the terminal:
```
$ netstat -tulpen
```

### Documentation for this software

Read the header file in the `include` directory of this repository. The documentation style follows the *Doxygen style* so it is possible to generate a documentation with the Doxygen software.

For example programs, see the codes under the `examples` directory.

There's a `CMakeLists.txt` under the examples folder, to build all examples, navigate to the `examples` folder, and execute the following commands after install the software:

```
$ mkdir build
$ cd build
$ cmake ..
$ make
```
