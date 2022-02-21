/**
 * @file example4s2.cpp
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
 * @brief This program demonstrate how to send information between servers with UDP protocol. Compile with
 * $g++ -Wall example4sr.cpp -lpigeon -o example4sr
 * This program is intended to run on a computer that has a static IP address.
 *
 *  Run example4sr first.
 *
 */

// replace "localhost" with the static ip address of the computer that runs the server4ss.cpp program
#define HOST "localhost"
// the port must be opened by the firewall
#define PORTs 2022
#define PORTr 3000

int main()
{

    // create a client side object, set the server to connect to be HOST, and open port number PORT
    // this is the only place you need to change in order to create a client that uses UDP protocol
    serverSideUDP server2{PORTr};
    // this string will be sent to the server
    std::string message{"Hello, from the server2."};
    // client will wait for 5 second, if there is no response, then the control flow will continue, and will not block
    server2.setTimeOut_Read(TIMEOUT_ONE_MINUTE);

    // we need to open the connection so the message can be sent
    server2.openServer();
    // server2.printErrorLog();
    // connection session opened

    // to store response
    char reply[1024];

    // read the data from the server
    server2.readData(reply, sizeof(reply));
    // print the response
    std::cout << std::string{reply} << "\n"
              << "From:\n"
              << server2.checkIncomingAddress() << " At port: " << server2.checkIncomingPortNumber()
              << std::endl;

    server2.replyToRead(message.c_str(), strlen(message.c_str()));
    // close the tcp connection session after finishing the connection
    server2.closeServer();

    return 0;
}
