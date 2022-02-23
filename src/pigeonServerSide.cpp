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

int __serverSide::bindSocket()
{
    struct addrinfo *server_linked, *serverinfo; // store server information
    bool success = false;

    int status = _addressLinkedSelfConstructing(&server_linked);

    if (status < 0)
    {
        // get DNS info failed
        _errorLog = "Error: get DNS info failed: " + std::string{gai_strerror(status)};
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
                if (bind(_SocketDescriptor, serverinfo->ai_addr, serverinfo->ai_addrlen) < 0)
                {
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
                            _addressDestructuring(serverinfo);
                            // break the loop
                            break;
                        }
                        else if ((serverinfo->ai_family == AF_INET6) && (IPversion == PIGEON_IPV6))
                        {

                            // successfully find a valid socket descriptor
                            success = true;
                            // record the IP address
                            _addressDestructuring(serverinfo);
                            // break the loop
                            break;
                        }
                        else
                        {
                            // release the bind to the socket
                            close(_SocketDescriptor);
                        }
                    }
                    else
                    {
                        // for the case of TCP
                        //  successfully find a valid socket descriptor
                        success = true;
                        // record the IP address
                        _addressDestructuring(serverinfo);
                        // break the loop
                        break;
                    }
                }
            }
        }
    }
    if (!success)
    {
        // fail to find a valid socket descriptor
        _errorLog = "Error at finding a valid socket descriptor: " + std::string(strerror(errno));
        closeServer();
        return -1;
    }

    // serverinfo is not longer needed: release it
    freeaddrinfo(server_linked);

    return 0;
}

int __serverSide::closeServer()
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

void __serverSide::setQueryRate(int rate)
{
    queryRate = rate;
}

int __serverSide::getQueryRate()
{
    return queryRate;
}