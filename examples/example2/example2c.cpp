/**
 * @file example2c.cpp
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
 * @brief This program demonstrate how to send information to a server with TCP protocol. Compile with
 * $g++ -Wall example2c.cpp -lpigeon -o example2c
 * This program is intended to run on a computer that has either a static or dynamic IP address.
 *
 *
 */

// replace "localhost" with the static ip address of the machine that runs the server program
#define HOST "localhost"
// the port must be opened by the firewall
#define PORT 2022

int main()
{

    // create a client side object, set the server to connect to be HOST, and open port number PORT
    clientSide client{HOST, PORT};
    // this string will be sent to the server
    std::string message{"Hello, from the client."};
    // client will wait for 5 second, if there is no response, then the control flow will continue, and will not block
    client.setTimeOut_Read(5);

    // we need to open the connection so the message can be sent
    client.openConnection();
    // connection session opened

    // sendData member function will only be valid between a pair of ``openConnection()'' and ``closeConnection()''
    client.sendData(message.c_str(), strlen(message.c_str()));

    // to store response
    char reply[1024];

    // read the data from the server
    client.readData(reply, sizeof(reply));
    // print the response
    std::cout << std::string{reply} << "\n"
              << "From:\n"
              << client.getServerIP() << " At port: " << client.getPortNumber()
              << std::endl;

    // close the tcp connection session after finishing the connection
    client.closeConnection();

    return 0;
}
