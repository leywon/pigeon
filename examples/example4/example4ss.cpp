/**
 * @file example4s1.cpp
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
 * $g++ -Wall example4ss.cpp -lpigeon -o example4ss
 * This program is intended to run on a computer that has a static IP address.
 *
 *  Run example4sr first.
 *
 */

// replace "localhost" with the static ip address of the computer that runs the server4sr.cpp program
#define HOST "localhost"
// the port must be opened by the firewall
#define PORTs 2022
#define PORTr 3000

int main()
{

    // create a client side object, set the server to connect to be HOST, and open port number PORT
    // this is the only place you need to change in order to create a client that uses UDP protocol
    serverSideUDP server1{PORTs};
    // this string will be sent to the server
    std::string message{"Hello, from the server1."};
    // client will wait for 5 second, if there is no response, then the control flow will continue, and will not block
    server1.setTimeOut_Read(TIMEOUT_ONE_MINUTE);

    // we need to open the connection so the message can be sent
    server1.openServer();
    // server1.printErrorLog();
    // connection session opened

    // sendData member function will only be valid between a pair of ``openConnection()'' and ``closeConnection()''
    server1.writeDestinationInfo(HOST, PORTr);
    server1.sendData(message.c_str(), strlen(message.c_str()));

    // to store response
    char reply[1024];

    // read the data from the server
    server1.readData(reply, sizeof(reply));
    // print the response
    std::cout << std::string{reply} << "\n"
              << "From:\n"
              << server1.checkIncomingAddress() << " At port: " << server1.checkIncomingPortNumber()
              << std::endl;

    // close the tcp connection session after finishing the connection
    server1.closeServer();

    return 0;
}
