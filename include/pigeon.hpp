/* The pigeon c++ network socket library: a simple c++ unix network socket library
    Copyright (C) 2022  Wang Liao

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
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

namespace pigeon
{
    // class declarations
    class ClientSide;
    class serverSide;
    class visitor;
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
         * @return int return port number on success, else return -1
         */
        int _addressDestructuring(struct addrinfo *serverInfo);

        /**
         * @brief resolve the information contained in struct addrinfo into a human readable ip address and port number. The ip address is written to an std string address, and port number returned.
         *
         * @param serverInfo pointer to the struct addrinfo, ready to be decoded.
         * @param address std string that will contained the decoded address, IP is untouched
         * @return int port number on success, else return -1
         */

        int _addressDestructuring(struct addrinfo *serverInfo, std::string &address);

        /**
         * @brief
         *
         * @param socket
         * @param address
         * @return int
         */

        int _addressDestructuring(struct sockaddr *socket, std::string &address);

        int _timeOutEnforcement(int socketDescriptor);

        int _addressLinkedConstructuring(struct addrinfo **serverLinked);

        int _addressLinkedSelfConstructing(struct addrinfo **serverLinked);

        int broadcastBit;
        struct sockaddr_in broadcastServerSocketAddress;

        // when success, return 0 and open a udp broadcasting session.
        int startBroadcast(std::string BroadcastServerAddress);
        int broadcasting(const void *data, size_t sizeInBytes);
        int closeBroadcast();

    public:
        // default constructor
        __pigeon();
        void printErrorLog();
        virtual int sendData(const void *data, size_t sizeInBytes);
        virtual int readData(void *data, size_t sizeInBytes);
        int getPortNumber();
        std::string getProtocolType();
        std::string getIP();
        void setAddress(std::string serverAddress);
        int setPortNumber(int portNumber);
        void setProtocolType(std::string type);
        int setTimeOut_Send(int second);
        int getTimeOut_Send();
        int setTimeOut_Read(int second);
        int getTimeOut_Read();
        // destructor
        ~__pigeon(){};
    };

    class clientSide : public __pigeon
    {
    private:
        // make some public inherited  member function private
        using __pigeon::getIP;
        using __pigeon::setAddress;

    public:
        // constructor
        clientSide(std::string serverAddress, int portNumber, std::string protocolType = "tcp", std::string IPversion = PIGEON_IPV4); //
        clientSide();                                                                                                                 //
        int openNewConnection(std::string serverAddress,
                              int portNumber, std::string protocolType, std::string IPversion = PIGEON_IPV4); //

        int closeConnection(); //
        int openConnection();
        std::string getServerIP();                        //
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
        int bindSocket();
        int queryRate;

    public:
        // openServer is a pure virtual method
        virtual int openServer() = 0;
        virtual int closeServer();
        void setQueryRate(int rate);
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
        serverSideTCP(int portNumber); //
        serverSideTCP();
        // openServer must be override
        int openServer();
        // destructor
        ~serverSideTCP(); //
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
        int setLatencyTime(int millisecond);
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
        serverSideUDP(int portNumber, std::string IPversion = PIGEON_IPV4); //
        serverSideUDP();                                                    //
        // openServer must be override
        int openServer(); //
        // override the sendData and readData
        int writeDestinationInfo(std::string address, int portNumber, std::string IPversion = PIGEON_IPV4); //
        int checkDestinationPortNumber();
        std::string checkDestinationAddress();
        int checkIncomingPortNumber();
        std::string checkIncomingAddress();
        int sendData(const void *data, size_t sizeInBytes);    //
        int readData(void *data, size_t sizeInBytes);          //
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