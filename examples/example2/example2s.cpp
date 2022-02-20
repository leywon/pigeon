#include <pigeon.hpp>

using namespace pigeon;

/**
 * @file example2s.cpp
 * @author Wang Liao
 * @brief
 * @version 0.1
 * @date 2022-02-20
 *
 * @copyright Copyright Wang Liao (c) 2022, MIT licence
 */
#include <pigeon.hpp>
using namespace pigeon;

/**
 * @brief This program demonstrate how to send information to a client with TCP protocol. Compile with
 * $g++ -Wall example2s.cpp -lpigeon -o example2s
 * This program is intended to run on a machine that has only an static IP address.
 *
 *
 */

// the port must be opened by the firewall
#define PORT 2022

int main()
{
    // create a serverSideTCP object
    serverSideTCP server{PORT};
    // this string will be sent to the server
    std::string message{"Hello, from the server."};
    // server will wait for 60 second, if there is no response, then the control flow will continue, and will not block
    server.setTimeOut_Read(TIMEOUT_ONE_MINUTE);
    server.openServer();
    // the tcp connection session opened
    // start to handle incoming connection
    visitor newClient;
    server.getOneVisitor(newClient);
    // once we get one client, we can continue to get more visitor. But for this example, we don't need that much:
    server.closeServer();
    // to store client response
    char reply[256];

    // read the data from the server
    newClient.readData(reply, sizeof(reply));
    // print the response
    std::cout << std::string{reply} << "\n"
              << "From:\n"
              << newClient.getClientIP() << " At port: " << newClient.getPortNumber()
              << std::endl;
    // sent greeting to the client
    newClient.sendData(message.c_str(), strlen(message.c_str()));

    // close the communication
    newClient.closeConnection();
}