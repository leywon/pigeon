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
clientSide::clientSide(std::string serverAddress, int portNumber, std::string protocolType, std::string IPversion)
{
    _Address = serverAddress;
    this->portNumber = portNumber;
    _protocolType = protocolType;
    this->IPversion = IPversion;

    setTimeOut_Read(TIMEOUT_ONE_MINUTE);
    setTimeOut_Send(TIMEOUT_ONE_MINUTE);
}

// override default constructor
clientSide::clientSide()
{
    _protocolType = "tcp";
    setTimeOut_Read(TIMEOUT_ONE_MINUTE);
    setTimeOut_Send(TIMEOUT_ONE_MINUTE);
}

// close connection
int clientSide::closeConnection()
{
    if (_SocketDescriptor < 0)
    {
        _errorLog = "Warning! Invalid connection to close";

        return -1;
    }
    else
    {
        return close(_SocketDescriptor);
    }
}

std::string clientSide::getServerIP()
{
    return getIP();
}

void clientSide::setServerAddress(std::string serverAddress)
{
    setAddress(serverAddress);
}

clientSide::~clientSide()
{
    closeConnection();
}

int clientSide::openNewConnection(std::string serverAddress, int portNumber, std::string protocolType, std::string IPversion)
{
    closeConnection();
    _Address = serverAddress;
    this->portNumber = portNumber;
    _protocolType = protocolType;
    this->IPversion = IPversion;
    return openConnection();
}

int clientSide::openConnection()
{
    struct addrinfo *serverinfo, *server_linked; // store server information
    bool success = false;                        // recording whether a valid socket is here
    int status = _addressLinkedConstructuring(&server_linked);

    if (status < 0)
    {
        // get DNS info failed
        _errorLog = "Error at getting a valid IP: " + std::string{gai_strerror(status)};
        return -1;
    }
    else
    {
        // walk through the server link to find a valid one
        for (serverinfo = server_linked; serverinfo != NULL; serverinfo = serverinfo->ai_next)
        {
            _SocketDescriptor = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol);
            if (_SocketDescriptor < -1)
            {
                continue;
            }
            else
            {
                if (connect(_SocketDescriptor, serverinfo->ai_addr, serverinfo->ai_addrlen) < 0)
                {
                    // even if the socket is available, we should abandon the unconnectable one
                    close(_SocketDescriptor);
                    continue;
                }
                else
                {
                    // if the connection is datagram, force using specific IP version
                    if (_protocolType == "udp")
                    {
                        if ((serverinfo->ai_family == AF_INET) && (IPversion == PIGEON_IPV4))
                        {

                            // successfully find a valid socket descriptor
                            success = true;
                            // record the IP address
                            portNumber = _addressDestructuring(serverinfo);

                            // break the loop
                            break;
                        }
                        else if ((serverinfo->ai_family == AF_INET6) && (IPversion == PIGEON_IPV6))
                        {

                            // successfully find a valid socket descriptor
                            success = true;
                            // record the IP address
                            portNumber = _addressDestructuring(serverinfo);
                            // break the loop
                            break;
                        }
                        else
                        {
                            // we need to close the socket descriptor
                            close(_SocketDescriptor);
                        }
                    }
                    else
                    {
                        // for the case of TCP
                        //  successfully find a valid socket descriptor
                        success = true;
                        // record the IP address
                        portNumber = _addressDestructuring(serverinfo);
                        // break the loop
                        break;
                    }
                }
            }
        }
    }

    if (!success)
    {
        _errorLog = std::string{strerror(errno)} != "Success" ? ("Error: " + std::string{strerror(errno)}) : "Error: Probably ipv6 UDP fault.";
        _SocketDescriptor = -1;

        return -1;
    }

    // serverinfo is not longer needed: release it
    freeaddrinfo(server_linked);
    // enforce time out time setting
    _timeOutEnforcement(_SocketDescriptor);

    return 0;
}