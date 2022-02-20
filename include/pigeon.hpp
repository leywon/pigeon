/*
// @ file
// @ author Wang Liao <warrenleywon@gmail.com>

// @section LICENSE

//     Copyright (C) 2022  Wang Liao

//     This program is free software: you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.

//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.

//     You should have received a copy of the GNU General Public License
//     along with this program.  If not, see <https://www.gnu.org/licenses/>.
// @section DESCRIPTION
// The pigeon c++ network socket library: a simple c++ unix network socket library
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <string>
#include <iostream>
#include <arpa/inet.h>
#include <thread>
#include <errno.h>
#include <chrono>
#include <sys/time.h>

// include guard
#ifndef __pigeon_stations_1519__
#define __pigeon_stations_1519__

#define TIMEOUT_ONE_MINUTE 60
#define BACKLOG_DEFAULT 5
#define LATENCY_DEFAULT 300
#define PORT_NUMBER_DEFAULT -1

#define PIGEON_IPV4 "ipv4"
#define PIGEON_IPV6 "ipv6"

#define PIGEON_TCP "tcp"
#define PIGEON_UDP "udp"

namespace pigeon
{
    // class declarations
    class ClientSide;
    class __serverSide;
    class visitor;
    class serverSideTCP;
    class serverSideUDP;
    class __pigeon;

    class __pigeon
    {
    protected:
        int _SocketDescriptor;
        std::string _errorLog;
        std::string _Address;
        std::string _protocolType;
        int portNumber;
        std::string IP;
        std::string IPversion;
        struct timeval _timeOutSend, _timeOutRead;
        /*
        all _addressDestructuring return port number on success,
        return -1 indicate an error: using printErrorLog() function to show the error
        */

        /**
         * @brief resolve the information contained in struct addrinfo into a human readable ip address and port number. The ip address is written to IP, and port number is returned.
         *
         * @param serverInfo pointer to the struct addrinfo, ready to be decoded.
         * @return  return port number on success, else return -1
         */
        int _addressDestructuring(struct addrinfo *serverInfo);

        /**
         * @brief resolve the information contained in struct addrinfo into a human readable ip address and port number. The ip address is written to an std string address, and port number returned.
         *
         * @param serverInfo pointer to the struct addrinfo, ready to be decoded.
         * @param address std string that will contained the decoded address, IP is untouched
         * @return port number on success, else return -1
         */

        int _addressDestructuring(struct addrinfo *serverInfo, std::string &address);

        /**
         * @brief resolve the information in a struct sockaddr pointed to by the argument socket; store the address information into the std string address, return port number on success
         *
         * @param socket pointer to the sockaddr that is going to be decripted
         * @param address will set to the address info upon success
         * @return port number, -1 for error
         */

        int _addressDestructuring(struct sockaddr *socket, std::string &address);

        /**
         * @brief Enforce timeout setting: program will wait for the information coming from the other side of the connection. If the maximum waiting time is reached, the program will skip the reading
         *
         * @param socketDescriptor file descriptor to modify the property
         * @return 0 on success, -1 on error
         */
        int _timeOutEnforcement(int socketDescriptor);

        /**
         * @brief Construct the linked structure from the object's parameter: IP, port number, etc
         *
         * @param serverLinked pointer to the pointer pointed to the structure that is the head of the data link
         * @return 0 on success and -1 on error
         */

        int _addressLinkedConstructuring(struct addrinfo **serverLinked);

        /**
         * @brief Construct the linked structure from the object's parameter: find the ip information about the machine the program is running on
         *
         * @param serverLinked pointer to the pointer pointed to the structure that is the head of the data link
         * @return 0 on success and -1 on error
         */

        int _addressLinkedSelfConstructing(struct addrinfo **serverLinked);

        int broadcastBit;
        struct sockaddr_in broadcastServerSocketAddress;

        // when success, return 0 and open a udp broadcasting session.
        /**
         * @brief This function start the broadcasting session
         *
         * @param BroadcastServerAddress The broadcast address, inferred from executing "$ip a" at the terminal
         * @return 0 on success, -1 when error occur
         */
        int startBroadcast(std::string BroadcastServerAddress);
        /**
         * @brief Send broadcasting information. This function is only valid between ``startBroadcast'' and ``closeBroadcast''
         *
         * @param data pointer to the beginning of the data block that is going to be transmitted
         * @param sizeInBytes the number of bytes that will be transmitted
         * @return 0 on success, -1 for error
         */
        int broadcasting(const void *data, size_t sizeInBytes);
        /**
         * @brief Close the broadcasting session
         *
         * @return 0 on success, -1 for error
         */
        int closeBroadcast();

    public:
        // default constructor
        __pigeon();
        /**
         * @brief Print the object's internal error log to screen
         *
         */
        void printErrorLog();

        /**
         * @brief This virtual function will send data when connection is established
         *
         * @param data constant pointer to the beginning of the data block that is going to be transmitted
         * @param sizeInBytes the number of bytes that is going to be transmitted
         * @return 0 on success, -1 for error
         */
        virtual int sendData(const void *data, size_t sizeInBytes);
        /**
         * @brief Read the data from the port
         *
         * @param data pointer to the segment of memory that is going to fill in the data
         * @param sizeInBytes the number of bytes to be received: excess bytes will be truncated
         * @return 0 on success, -1 for error
         */
        virtual int readData(void *data, size_t sizeInBytes);
        /**
         * @brief Get the Port Number
         *
         * @return port number
         */
        int getPortNumber();
        /**
         * @brief Get the Protocol Type: "tcp" or "udp"
         *
         * @return std::string
         */
        std::string getProtocolType();
        /**
         * @brief get the IP address recorded by the setAddress function
         *
         * @return std::string
         */
        std::string getIP();
        /**
         * @brief Set the Address e.g. "example.com", "127.0.0.1"
         *
         * @param serverAddress std string of the address: e.g. "example.com", "127.0.0.1"
         */
        void setAddress(std::string serverAddress);
        /**
         * @brief Set the Port Number
         *
         * @param portNumber
         * @return int
         */
        int setPortNumber(int portNumber);
        /**
         * @brief Set the Protocol Type: must be "tcp" or "udp" surrounded by double quotes
         *
         * @param type string, either "tcp" or "udp" surrounded by DOUBLE QUOTES
         */
        void setProtocolType(std::string type);
        /**
         * @brief Set the maximum amount of the time consumed for sending data
         *
         * @param second integer of second
         * @return 0 on success, -1 for error
         */
        int setTimeOut_Send(int second);
        /**
         * @brief Get the the maximum amount of the time consumed for sending data
         *
         * @return time in second
         */
        int getTimeOut_Send();
        /**
         * @brief Set the maximum amount of the time consumed for reading data, including waiting for data to come to port
         *
         * @param second time in second
         * @return 0 on success, -1 for error
         */
        int setTimeOut_Read(int second);
        /**
         * @brief Get the TimeOut time setted by the setTimeOut_Read
         *
         * @return time in second, integer
         */
        int getTimeOut_Read();

        /**
         * @brief Get the ip version from the object
         *
         * @return std::string IP version: PIGEON_IPV4, or PIGEON_IPV6 macro
         */

        std::string getIPversion();

        /**
         * @brief Set the IP version into the object
         *
         * @param IPversion std::string, either PIGEON_IPV4 or PIGEON_IPV6 macro
         */
        void setIPversion(std::string IPversion);
        // destructor
        ~__pigeon(){};
    };

    class clientSide : public __pigeon
    {
    private:
        // make some public inherited  member function private
        // using __pigeon::getIP;
        // using __pigeon::setAddress;

    public:
        // constructor
        clientSide(std::string serverAddress, int portNumber, std::string protocolType = "tcp", std::string IPversion = PIGEON_IPV4); //
        clientSide();                                                                                                                 //
        /**
         * @brief Open a new connection session
         *
         * @param serverAddress e.g. "example.com", "127.0.0.1"
         * @param portNumber port number
         * @param protocolType "tcp" or "udp" surrounded by DOUBLE QUOTES
         * @param IPversion PIGEON_IPV4 or PIGEON_IPV6
         * @return 0 on success, -1 for error
         */
        int openNewConnection(std::string serverAddress,
                              int portNumber, std::string protocolType, std::string IPversion = PIGEON_IPV4); //
        /**
         * @brief Close the connection session to the server
         *
         * @return 0 on success, -1 for error
         */
        int closeConnection(); //
        /**
         * @brief Open a connection session
         *
         * @return 0 on success, -1 for error
         */
        int openConnection();
        /**
         * @brief Get the Server IP address
         *
         * @return std::string, the address of the server
         */
        std::string getServerIP(); //
        /**
         * @brief Set the Server Address
         *
         * @param serverAddress e.g. "example.com", "127.0.0.1" surround by double quotes
         */
        void setServerAddress(std::string serverAddress); //
        // destructor
        ~clientSide(); //

        // client side is allowed to use broadcast function
        /**
         * @brief Some members that are allowed to be publicly available
         *
         */
        using __pigeon::broadcasting;
        using __pigeon::closeBroadcast;
        using __pigeon::startBroadcast;
    };

    class __serverSide : protected __pigeon
    {
    protected:
        // when success, bindSocket will prepare a socket descriptor for further process
        /**
         * @brief This function will bind the socket to a specific port that is store inside the object (the port number can be accessed using corresponding getters and setters member functions)
         *
         * @return int, 0 on success, -1 for error
         */
        int bindSocket();
        int queryRate;

    public:
        // openServer is a pure virtual method
        /**
         * @brief This is a pure virtual methods that need to be override by the correspoinding derived class
         *
         * @return int , should return 0 on success
         */
        virtual int openServer() = 0;
        /**
         * @brief Close the server
         *
         * @return int ,0 on success, -1 for error
         */
        virtual int closeServer();
        /**
         * @brief Set the speed of connection: how fast that is needed for a client to connect to the server
         *
         * @param rate integer (typically 5 to 120), the higher the number, the faster the connection speed
         */
        void setQueryRate(int rate);
        /**
         * @brief Get the Query Rate
         *
         * @return int , the higher the number, the faster the connection speed
         */
        int getQueryRate();

        // making some protected members public
        /**
         * @brief Some members are allowed to be used in this case
         *
         */
        using __pigeon::getPortNumber;
        using __pigeon::getTimeOut_Read;
        using __pigeon::getTimeOut_Send;
        using __pigeon::printErrorLog;
        using __pigeon::setPortNumber;
        using __pigeon::setTimeOut_Read;
        using __pigeon::setTimeOut_Send;
    };

    class serverSideTCP : public __serverSide
    {
    public:
        // constructor
        /**
         * @brief Construct a new server Side TCP object
         *
         * @param portNumber set the port number of the server
         */
        serverSideTCP(int portNumber); //
        serverSideTCP();
        // openServer must be override
        int openServer();
        // destructor
        ~serverSideTCP(); //
        /**
         * @brief This function will write the necessary information about a newly accepted connection to a visitor object. Later, the communication through the new connection will be completed with the visitor object
         *
         * @param newVisitor reference to the visitor object: this will be modified
         * @return int, 0 on success, -1 for error
         */
        int getOneVisitor(visitor &newVisitor);
    };

    class visitor : public __pigeon
    {
    private:
        // only allow serverSide to initialize the object
        friend int serverSideTCP::getOneVisitor(visitor &);

        int closeWaitLatency;

        // restrict the access of setters
        using __pigeon::setAddress;
        using __pigeon::setPortNumber;
        using __pigeon::setProtocolType;
        // using __pigeon::setTimeOut_Read;
        // using __pigeon::setTimeOut_Send;

        // redefine some getters
        using __pigeon::getIP;

    public:
        visitor();
        std::string getClientIP(); //
        int closeConnection();     //
        // destructor
        ~visitor(); //
        /**
         * @brief Set the Latency Time: when closing the visitor object, it will wait for a certain amount of millisecond before actually close the connection. This functionality is to hopefully make sure the visitor receives the closing FIN from the client (This process is handled by the kernel). Set to 0 if you have your own protocol of closing built on top of the transport layer.
         *
         * @param millisecond integer, typically 300
         * @return int ,0 on success, -1 for error
         */
        int setLatencyTime(int millisecond);
        /**
         * @brief Show the latency time
         *
         * @return int , in the unit of millisecond
         */
        int getLatencyTime();
    };

    // UDP server

    class serverSideUDP : public __serverSide
    {
    private:
        struct sockaddr_storage addr_incomingClient;
        struct sockaddr_storage addr_destinationClient;
        socklen_t incomingClientAddressLength;

    public:
        // constructor
        /**
         * @brief Construct a new server Side UDP object, with port number, and the ip version that is going to be used.
         *
         * @param portNumber nonnegative integer
         * @param IPversion PIGEON_IPV4(default) or PIGEON_IPV6
         */
        serverSideUDP(int portNumber, std::string IPversion = PIGEON_IPV4); //
        serverSideUDP();                                                    //
        // openServer must be override
        /**
         * @brief Open a new UDP connection session
         *
         * @return int, 0 on success, -1 for error
         */
        int openServer(); //
        // override the sendData and readData
        /**
         * @brief Write down the information about the destination (a machine) so that the sendData function can deliver the data to the destination by the UDP protocol.
         *
         * @param address e.g. "example.com", "127.0.0.1", or "localhost"
         * @param portNumber non-negative integer
         * @param IPversion PIGEON_IPV4 (default) or PIGEON_IPV6
         * @return int, 0 on success, -1 for error
         */
        int writeDestinationInfo(std::string address, int portNumber, std::string IPversion = PIGEON_IPV4); //
        /**
         * @brief getters to show the destination port number
         *
         * @return int , port number
         */
        int checkDestinationPortNumber();
        /**
         * @brief getters to show the destination address
         *
         * @return std::string, contains the IP address
         */
        std::string checkDestinationAddress();
        /**
         * @brief Getters to show where's the port that the incoming traffic is sent from
         *
         * @return int, port number
         */
        int checkIncomingPortNumber();
        /**
         * @brief Getters, check the IP address where the incoming data comes from
         *
         * @return std::string, contains IP address
         */
        std::string checkIncomingAddress();
        /**
         * @brief Send a block of data to the destination with the information setted by the setters
         *
         * @param data pointed to the beginning of the block of information that is going to be sent
         * @param sizeInBytes how many bytes to send
         * @return int, 0 on success, -1 for error
         */
        int sendData(const void *data, size_t sizeInBytes); //
        /**
         * @brief Read the incoming data, and set the internal variables to hold the incoming address, and port number to be read by getters
         *
         * @param data pointer point to the beginning of the memory segment that contains the incoming binary data
         * @param sizeInBytes how many bytes to read, overflow data will be trimmed and discarded
         * @return int, 0 on success, -1 for error
         */
        int readData(void *data, size_t sizeInBytes); //
        /**
         * @brief The object will record temporarily the incoming address and port number until the next readData (after that they will be resetted). This function will reply message to the address that has just been registered by the readData member function
         *
         * @param data pointed to the beginning of the block of information that is going to be sent
         * @param sizeInBytes how many bytes to read, overflow data will be trimmed and discarded
         * @return int, 0 on success, -1 for error
         */
        int replyToRead(const void *data, size_t sizeInBytes); //
        // destructor
        ~serverSideUDP(); //
        // UDP server is allowed to use broadcast function
        using __pigeon::broadcasting;
        using __pigeon::closeBroadcast;
        using __pigeon::startBroadcast;
    };

}; // namespace pigeon
#endif