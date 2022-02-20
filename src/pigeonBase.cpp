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
#include "pigeon.hpp"

using namespace pigeon;

// constructor
__pigeon::__pigeon()
{
    _SocketDescriptor = -1;
}

void __pigeon::printErrorLog()
{
    std::cout << "Log :\n"
              << _errorLog << std::endl;
}

std::string __pigeon::getIP()
{
    return IP;
}

std::string __pigeon::getProtocolType()
{
    return _protocolType;
}

int __pigeon::getPortNumber()
{
    return portNumber;
}

int __pigeon::sendData(const void *data, size_t sizeInBytes)
{
    if (data == nullptr)
    {
        _errorLog = "Warning: Null pointer pass on to sendData.";
        return -1;
    }
    if (_SocketDescriptor < 0)
    {
        _errorLog = "Warning: Invalid Socket Descriptor.";
        return -1;
    }
    int bytesSent = send(_SocketDescriptor, data, sizeInBytes, MSG_NOSIGNAL);
    if (bytesSent < 0)
    {
        _errorLog = "Warning: " + std::string{strerror(errno)};

        return -1;
    }
    else
    {
        return bytesSent;
    }
}

int __pigeon::readData(void *data, size_t sizeInBytes)
{
    if (data == nullptr)
    {
        _errorLog = "Warning: Null pointer pass on to readData.";
        return -1;
    }
    if (_SocketDescriptor < 0)
    {
        _errorLog = "Warning: Invalid Socket Descriptor.";
        return -1;
    }
    int bytesReceived = recv(_SocketDescriptor, data, sizeInBytes, 0);
    if (bytesReceived < 0)
    {
        _errorLog = "Warning:" + std::string{strerror(errno)};
        return -1;
    }
    else
    {
        return bytesReceived;
    }
}

void __pigeon::setAddress(std::string serverAddress)
{
    _Address = serverAddress;
}

int __pigeon::setPortNumber(int portNumber)
{
    if (portNumber < 0)
    {
        _errorLog = "Error: Invalid port number!";
        return -1;
    }
    this->portNumber = portNumber;

    return 0;
}

int __pigeon::setTimeOut_Read(int second)
{
    if (second < 0)
    {
        _errorLog = "Error: Invalid ReadOut time!";

        return -1;
    }
    _timeOutRead.tv_sec = second;
    _timeOutRead.tv_usec = 0;
    if (_SocketDescriptor != -1)
    {
        _timeOutEnforcement(_SocketDescriptor);
    }

    return 0;
}

int __pigeon::setTimeOut_Send(int second)
{
    if (second < 0)
    {
        _errorLog = "Error: Invalid SendOut time!";

        return -1;
    }
    _timeOutSend.tv_sec = second;
    _timeOutSend.tv_usec = 0;

    if (_SocketDescriptor != -1)
    {
        _timeOutEnforcement(_SocketDescriptor);
    }

    return 0;
}

int __pigeon::getTimeOut_Send()
{
    return _timeOutSend.tv_sec;
}

int __pigeon::getTimeOut_Read()
{
    return _timeOutRead.tv_sec;
}

void __pigeon::setProtocolType(std::string type)
{
    _protocolType = type;
}

int __pigeon::_addressDestructuring(struct addrinfo *serverInfo)
{
    if (serverInfo->ai_family == AF_INET)
    {
        // ipv4 address
        // cast into ipv4 format
        struct sockaddr_in *serverinfo_in = (struct sockaddr_in *)serverInfo->ai_addr;
        char ip4[INET_ADDRSTRLEN];
        if (inet_ntop(AF_INET, &(serverinfo_in->sin_addr), ip4, INET_ADDRSTRLEN) != NULL)
        {
            this->IP = std::string{ip4};

            return serverinfo_in->sin_port;
        }
        else
        {
            _errorLog = "Decoding network address error: " + std::string{strerror(errno)};

            return -1;
        }
    }
    else
    {
        // ipv6 address
        struct sockaddr_in6 *serverinfo_in = (struct sockaddr_in6 *)serverInfo->ai_addr;
        char ip6[INET6_ADDRSTRLEN];
        if (inet_ntop(AF_INET6, &(serverinfo_in->sin6_addr), ip6, INET6_ADDRSTRLEN) != NULL)
        {
            this->IP = std::string{ip6};

            return serverinfo_in->sin6_port;
        }
        else
        {
            _errorLog = "Decoding network address error: " + std::string{strerror(errno)};

            return -1;
        }
    }
}

int __pigeon::_addressDestructuring(struct addrinfo *serverInfo, std::string &address)
{
    if (serverInfo->ai_family == AF_INET)
    {
        // ipv4 address
        // cast into ipv4 format
        struct sockaddr_in *serverinfo_in = (struct sockaddr_in *)serverInfo->ai_addr;
        char ip4[INET_ADDRSTRLEN];
        if (inet_ntop(AF_INET, &(serverinfo_in->sin_addr), ip4, INET_ADDRSTRLEN) != NULL)
        {
            address = std::string{ip4};

            return serverinfo_in->sin_port;
        }
        else
        {
            _errorLog = "Decoding network address error: " + std::string{strerror(errno)};

            return -1;
        }
    }
    else
    {
        // ipv6 address
        struct sockaddr_in6 *serverinfo_in = (struct sockaddr_in6 *)serverInfo->ai_addr;
        char ip6[INET6_ADDRSTRLEN];
        if (inet_ntop(AF_INET6, &(serverinfo_in->sin6_addr), ip6, INET6_ADDRSTRLEN) != NULL)
        {
            address = std::string{ip6};

            return serverinfo_in->sin6_port;
        }
        else
        {
            _errorLog = "Decoding network address error: " + std::string{strerror(errno)};

            return -1;
        }
    }
}

int __pigeon::_addressLinkedConstructuring(struct addrinfo **serverLinked)
{
    int status;                       // store getaddrinfo return value
    struct addrinfo hints;            // additional information about the TCP/IP that is going to be found
    memset(&hints, 0, sizeof(hints)); // make sure the struct has no value
    hints.ai_family = AF_UNSPEC;      // protocol agnostic
    if (_protocolType == "tcp")
        hints.ai_socktype = SOCK_STREAM; // datastream rather than datagram
    else
        hints.ai_socktype = SOCK_DGRAM; // datagram rather than datastream

    // fetch TCP/IP info from the outside network
    status = getaddrinfo(_Address.c_str(),                   // webadress, or ip address in string
                         std::to_string(portNumber).c_str(), //"http" or port number like "234"
                         &hints,                             // pass on additional information for how to fetch
                         serverLinked                        // the result of query, returned as a link, is stored here
    );

    if (status < 0)
    {
        // get DNS info failed
        _errorLog = "Error at getting a valid IP: " + std::string{gai_strerror(status)};
        return -1;
    }
    else
    {
        return 0;
    }
}

int __pigeon::_addressDestructuring(struct sockaddr *socket, std::string &address)
{
    if (socket->sa_family == AF_INET)
    { // we get an ipv4 client
        // create chars to hold IPv4 string
        char ip4[INET_ADDRSTRLEN];
        // create int to store port number
        int port_number;
        struct sockaddr_in *socket_in = (struct sockaddr_in *)socket;
        if (inet_ntop(AF_INET, &(socket_in->sin_addr), ip4, INET_ADDRSTRLEN) != NULL)
        {
            address = std::string{ip4};
            port_number = socket_in->sin_port;

            return port_number;
        }
        else
        {
            _errorLog = "Decoding network address error: " + std::string{strerror(errno)};

            return -1;
        }
    }
    else
    {
        // we get an ipv6 client
        char ip6[INET6_ADDRSTRLEN];
        // create int to store port number
        int port_number;
        struct sockaddr_in6 *socket_in = (struct sockaddr_in6 *)socket;
        if (inet_ntop(AF_INET, &(socket_in->sin6_addr), ip6, INET_ADDRSTRLEN) != NULL)
        {
            address = std::string{ip6};
            port_number = socket_in->sin6_port;

            return port_number;
        }
        else
        {
            _errorLog = "Decoding network address error: " + std::string{strerror(errno)};

            return -1;
        }
    }
}

int __pigeon::_timeOutEnforcement(int socketDescriptor)
{
    if (setsockopt(socketDescriptor, SOL_SOCKET, SO_RCVTIMEO, &_timeOutRead, sizeof(_timeOutRead)) < 0)
    {
        _errorLog = "Fail to set timeout time for Reading: " + std::string{strerror(errno)};

        return -1;
    }
    if (setsockopt(socketDescriptor, SOL_SOCKET, SO_SNDTIMEO, &_timeOutSend, sizeof(_timeOutSend)) < 0)
    {
        _errorLog = "Fail to set timeout time for Sending: " + std::string{strerror(errno)};

        return -1;
    }
    // if successfully passing two ifs' ...
    return 0;
}

int __pigeon::_addressLinkedSelfConstructing(struct addrinfo **serverLinked)
{
    int status;                       // store getaddrinfo return value
    struct addrinfo hints;            // additional information about the TCP/IP that is going to be found
    memset(&hints, 0, sizeof(hints)); // make sure the struct has no value
    hints.ai_family = AF_UNSPEC;      // protocol agnostic
    if (_protocolType == "tcp")
        hints.ai_socktype = SOCK_STREAM; // datastream rather than datagram
    else
        hints.ai_socktype = SOCK_DGRAM; // datagram rather than datastream

    hints.ai_flags = AI_PASSIVE; // fill in the address of myself automatically

    // fetch TCP/IP info from the outside network
    status = getaddrinfo(NULL,                               // webadress, or ip address in string
                         std::to_string(portNumber).c_str(), //"http" or port number like "234"
                         &hints,                             // pass on additional information for how to fetch
                         serverLinked                        // the result of query, returned as a link, is stored here
    );

    if (status < 0)
    {
        // get DNS info failed
        _errorLog = "Error at getting a valid IP: " + std::string{gai_strerror(status)};
        return -1;
    }
    else
    {
        return 0;
    }
}

int __pigeon::startBroadcast(std::string broadcastServerAddress)
{
    // prepare the network socket
    if (close(_SocketDescriptor) < 0)
    {
        _errorLog = "Warning at closing socket before startBroadcast: " + std::string{strerror(errno)};
    }

    _SocketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (_SocketDescriptor < 0)
    {
        _errorLog = "Error at startBroadcast: " + std::string{strerror(errno)};

        return -1;
    }

    // set socket option for broadcasting
    broadcastBit = 1;
    if (setsockopt(_SocketDescriptor, SOL_SOCKET, SO_BROADCAST, &broadcastBit, sizeof(broadcastBit)) < 0)
    {
        _errorLog = "Error at startBroadcast: " + std::string{strerror(errno)};
        return -1;
    }
    // prepare broadcasting server address
    broadcastServerSocketAddress.sin_family = AF_INET;
    broadcastServerSocketAddress.sin_port = htons(portNumber);

    int status = inet_pton(AF_INET, broadcastServerAddress.c_str(), &broadcastServerSocketAddress.sin_addr);
    if (status == 0)
    {
        _errorLog = "Error: Invalid Broadcast Server Address format! Try ipv4 addresses under brd when ip a";
        close(_SocketDescriptor);
        return -1;
    }
    if (status == -1)
    {
        _errorLog = "Error: IP address format is not supported: " + std::string{strerror(errno)} + "\n Try ipv4 address under brd when ip a";
        close(_SocketDescriptor);
        return -1;
    }

    return 0;
}

int __pigeon::broadcasting(const void *Data, size_t sizeInBytes)
{
    int bytes = sendto(_SocketDescriptor, Data, sizeInBytes, 0, (struct sockaddr *)&broadcastServerSocketAddress, sizeof(broadcastServerSocketAddress));
    if (bytes < 0)
    {
        _errorLog = "Warning at sending broadcast message: " + std::string{strerror(errno)};
    }

    return bytes;
}

int __pigeon::closeBroadcast()
{
    if (close(_SocketDescriptor) < 0)
    {
        _errorLog = "Warning at closing broadcast socket: " + std::string{strerror(errno)};
    }

    return 0;
}

std::string __pigeon::getIPversion()
{
    return IPversion;
}

void __pigeon::setIPversion(std::string IPversion)
{
    this->IPversion = IPversion;
}