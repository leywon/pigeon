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

visitor::visitor()
{
    setLatencyTime(LATENCY_DEFAULT);
    setTimeOut_Read(TIMEOUT_ONE_MINUTE);
    setTimeOut_Send(TIMEOUT_ONE_MINUTE);
}

int visitor::closeConnection()
{
    if (_SocketDescriptor < 0)
    {
        _errorLog = "Error! Invalid connection to close";

        return -1;
    }
    else
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(closeWaitLatency));
        return close(_SocketDescriptor);
    }
}

// public destructor
visitor::~visitor()
{
    closeConnection();
}

std::string visitor::getClientIP()
{
    return getIP();
}

int visitor::setLatencyTime(int millisecond)
{
    if (millisecond < 0)
    {
        _errorLog = "Error: Invalid latency time!";
        closeWaitLatency = LATENCY_DEFAULT;

        return -1;
    }
    else
    {
        closeWaitLatency = millisecond;

        return 0;
    }
}

int visitor::getLatencyTime()
{
    return closeWaitLatency;
}
