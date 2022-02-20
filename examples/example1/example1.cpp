/**
 * @file example1.cpp
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
 * @brief This program demonstrate how to query information from a http server. Compile with
 * $g++ -Wall example1.cpp -lpigeon -o example1
 *
 *
 */

#define HOST "example.com"
#define HOST_ANOTHER "google.com"
// http uses port 80
#define PORT 80

int main()
{

    // create a client side object, set the server to connect to be HOST, and open port number PORT
    clientSide client{HOST, PORT};
    // this string will tell the server to reply the html page information
    std::string message{"GET / HTTP/1.1\r\nHost: "};
    message += (std::string{HOST} + "\r\n\r\n");
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

    // object can be reused once previous connection session is closed
    // Use setServerAddress not setAddress(ambiguous)
    client.setServerAddress(HOST_ANOTHER);
    client.openConnection();
    // open another connection session
    // this string will tell the server to reply the html page information
    message = "GET / HTTP/1.1\r\nHost: ";
    message += (std::string{HOST_ANOTHER} + "\r\n\r\n");

    // sendData member function will only be valid between a pair of ``openConnection()'' and ``closeConnection()''
    client.sendData(message.c_str(), strlen(message.c_str()));

    // read the data from the server
    client.readData(reply, sizeof(reply));
    // print the response
    std::cout << std::string{reply} << "\n"
              << "From:\n"
              << client.getServerIP() << " At port: " << client.getPortNumber()
              << std::endl;

    client.closeConnection();

    return 0;
}
