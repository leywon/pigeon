/**
 * @file example3s.cpp
 * @author Wang Liao
 * @brief
 * @version 0.1
 * @date 2022-02-21
 *
 * @copyright Copyright Wang Liao (c) 2022, MIT licence
 */
#include <pigeon.hpp>
using namespace pigeon;

/**
 * @brief This program demonstrate how to send information to a client with UDP protocol. Compile with
 * $g++ -Wall example3s.cpp -lpigeon -o example3s
 * This program is intended to run on a computer that has only an static IP address.
 *
 *
 */

// the port must be opened by the firewall
#define PORT 2022

int main()
{
    // create a serverSideUDP object
    serverSideUDP server{PORT};
    // this string will be sent to the server
    std::string message{"Hello, from the server."};
    // server will wait for 60 second, if there is no response, then the control flow will continue, and will not block
    server.setTimeOut_Read(TIMEOUT_ONE_MINUTE);
    server.openServer();
    // the tcp connection session opened
    // to store client response
    char reply[256];

    // read the data from the server
    server.readData(reply, sizeof(reply));
    // print the response
    std::cout << std::string{reply} << "\n"
              << "From:\n"
              << server.checkIncomingAddress() << " At port: " << server.checkIncomingPortNumber()
              << std::endl;
    // sent greeting to the client
    server.replyToRead(message.c_str(), strlen(message.c_str()));

    // close the communication
    server.closeServer();

    return 0;
}