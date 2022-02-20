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
int serverSideTCP::getOneVisitor(visitor &newVisitor)
{
    // create variable to store incoming connection info
    struct sockaddr_storage incoming_addr;
    socklen_t incoming_addr_len = sizeof(incoming_addr);

    // accept will return a new socket descriptor so the information will be processed
    int incoming_socket_descriptor = accept(_SocketDescriptor,                 // take one connection from the queue waiting on the socket "local"
                                            (struct sockaddr *)&incoming_addr, // the function fills this pointer with incoming connection's address info
                                            &incoming_addr_len                 // Even though the user need to supply an approximate size of addr_len for this argument, the function will calculate the lenght of the incoming_addr and fill in this pointer the correct value.
    );

    if (incoming_socket_descriptor < 0)
    {
        _errorLog = "Error at Accepting: " + std::string{strerror(errno)};
        return -1;
    }
    else
    {
        // record the client information
        int portNumberClient = _addressDestructuring((struct sockaddr *)&incoming_addr, newVisitor.IP);

        // record the socket descriptor
        newVisitor.setTimeOut_Read(TIMEOUT_ONE_MINUTE);
        newVisitor.setTimeOut_Send(TIMEOUT_ONE_MINUTE);

        newVisitor._SocketDescriptor = incoming_socket_descriptor;
        newVisitor.portNumber = portNumberClient;
        newVisitor._protocolType = _protocolType;
        newVisitor.setTimeOut_Read(_timeOutRead.tv_sec);
        newVisitor.setTimeOut_Send(_timeOutSend.tv_sec);

        return 0;
    }
}

// constructor
serverSideTCP::serverSideTCP(int portNumber)
{
    setPortNumber(portNumber);
    setProtocolType("tcp");
    setQueryRate(BACKLOG_DEFAULT);
    setTimeOut_Read(TIMEOUT_ONE_MINUTE);
    setTimeOut_Send(TIMEOUT_ONE_MINUTE);
}

serverSideTCP::serverSideTCP()
{
    setPortNumber(PORT_NUMBER_DEFAULT);
    setProtocolType("tcp");
    setQueryRate(BACKLOG_DEFAULT);
    setTimeOut_Read(TIMEOUT_ONE_MINUTE);
    setTimeOut_Send(TIMEOUT_ONE_MINUTE);
}

// destructor
serverSideTCP::~serverSideTCP()
{
    closeServer();
}

int serverSideTCP::openServer()
{

    if (bindSocket() < 0)
    {
        return -1;
    }
    // enforce time out time setting
    _timeOutEnforcement(_SocketDescriptor);

    if (listen(_SocketDescriptor, // the socket descriptor
               queryRate          // queryRate determines the rate of client connection: the higher the number, the faster the connection. Restricted by the operating systems, typical value ~ 120
               ) < -1)
    {
        _errorLog = "Error at Listening: " + std::string{strerror(errno)};
        closeServer();
        return -1;
    }

    return 0;
}