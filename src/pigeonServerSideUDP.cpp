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
serverSideUDP::serverSideUDP(int portNumber, std::string IPversion)
{
    setPortNumber(portNumber);
    setProtocolType("udp");
    setQueryRate(BACKLOG_DEFAULT);
    setTimeOut_Read(0);
    setTimeOut_Send(0);
    incomingClientAddressLength = sizeof(struct sockaddr_storage);
    this->IPversion = IPversion;
}

serverSideUDP::serverSideUDP()
{
    setPortNumber(PORT_NUMBER_DEFAULT);
    setProtocolType("udp");
    setQueryRate(BACKLOG_DEFAULT);
    setTimeOut_Read(0);
    setTimeOut_Send(0);
    incomingClientAddressLength = sizeof(struct sockaddr_storage);
    IPversion = PIGEON_IPV4;
}

// destructor
serverSideUDP::~serverSideUDP()
{
    closeServer();
}

int serverSideUDP::openServer()
{

    if (bindSocket() < 0)
    {
        return -1;
    }
    // enforce time out time setting
    if (_timeOutEnforcement(_SocketDescriptor) < 0)
    {
        return -1;
    }
    return 0;
}

int serverSideUDP::writeDestinationInfo(std::string address, int portNumber, std::string IPversion)
{
    int status;            // store getaddrinfo return value
    struct addrinfo hints; // additional information about the TCP/IP that is going to be found
    struct addrinfo *serverLinked, *serverinfo;
    memset(&hints, 0, sizeof(hints)); // make sure the struct has no value
    hints.ai_family = AF_UNSPEC;      // protocol agnostic
    if (_protocolType == "tcp")
        hints.ai_socktype = SOCK_STREAM; // datastream rather than datagram
    else
        hints.ai_socktype = SOCK_DGRAM; // datagram rather than datastream

    // fetch TCP/IP info from the outside network
    status = getaddrinfo(address.c_str(),                    // webadress, or ip address in string
                         std::to_string(portNumber).c_str(), //"http" or port number like "234"
                         &hints,                             // pass on additional information for how to fetch
                         &serverLinked                       // the result of query, returned as a link, is stored here
    );

    if (status < 0)
    {
        // get DNS info failed
        _errorLog = "Error at getting a valid IP: " + std::string{gai_strerror(status)};
        return -1;
    }
    else
    {
        // walk through the link to find an address conform to IPtype
        for (serverinfo = serverLinked; serverinfo != NULL; serverinfo = serverinfo->ai_next)
        {
            if ((serverinfo->ai_family == AF_INET) && (IPversion == PIGEON_IPV4))
            {
                addr_destinationClient = *(struct sockaddr_storage *)serverinfo->ai_addr;

                freeaddrinfo(serverLinked);

                return 0;
            }

            if ((serverinfo->ai_family == AF_INET6) && (IPversion == PIGEON_IPV6))
            {
                addr_destinationClient = *(struct sockaddr_storage *)serverinfo->ai_addr;
                freeaddrinfo(serverLinked);

                return 0;
            }
        }

        _errorLog = "Error: Fail to find a valid address to send to ... ";

        freeaddrinfo(serverLinked);

        return -1;
    }
}

int serverSideUDP::sendData(const void *data, size_t sizeInBytes)
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
    int bytesSent = sendto(_SocketDescriptor, data, sizeInBytes, MSG_NOSIGNAL, (struct sockaddr *)&addr_destinationClient, sizeof(addr_destinationClient));
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

int serverSideUDP::readData(void *data, size_t sizeInBytes)
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
    int bytesReceived = recvfrom(_SocketDescriptor, data, sizeInBytes, 0, (struct sockaddr *)&addr_incomingClient, &incomingClientAddressLength);
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

int serverSideUDP::replyToRead(const void *data, size_t sizeInBytes)
{
    if (data == nullptr)
    {
        _errorLog = "Warning: Null pointer pass on to sendData.";
        return -1;
    }
    int bytesSent = sendto(_SocketDescriptor, data, sizeInBytes, MSG_NOSIGNAL, (struct sockaddr *)&addr_incomingClient, sizeof(addr_incomingClient));
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

int serverSideUDP::checkDestinationPortNumber()
{
    std::string addressDestination;
    return _addressDestructuring((struct sockaddr *)&addr_destinationClient, addressDestination);
}

std::string serverSideUDP::checkDestinationAddress()
{
    std::string addressDestination;
    _addressDestructuring((struct sockaddr *)&addr_destinationClient, addressDestination);

    return addressDestination;
}

int serverSideUDP::checkIncomingPortNumber()
{
    std::string addressIncoming;
    return _addressDestructuring((struct sockaddr *)&addr_destinationClient, addressIncoming);
}

std::string serverSideUDP::checkIncomingAddress()
{
    std::string addressIncoming;
    _addressDestructuring((struct sockaddr *)&addr_destinationClient, addressIncoming);

    return addressIncoming;
}
