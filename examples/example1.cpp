#include <pigeon.hpp>
using namespace pigeon;

/**
 * @brief This program demonstrate how to query information from a http serverv. Compile with
 * $g++ -Wall example1.cpp -lpigeon -o example1
 *
 *
 */

#define HOST "example.com"
// http uses port 80
#define PORT 80

int main()
{

    clientSide client{HOST, PORT};
    // this string will tell the server to reply the html page information
    std::string message{"GET / HTTP/1.1\r\nHost: cubicshape.com\r\n\r\n"};
    // client will wait for 5 second, if there is no response, then the control flow will continue, and will not block
    client.setTimeOut_Read(5);

    client.openConnection();
    // connection session opened
    client.sendData(message.c_str(), strlen(message.c_str()));

    // to store response
    char reply[1024];

    client.readData(reply, sizeof(reply));
    // print the response
    std::cout << std::string{reply} << "\n"
              << "From:\n"
              << client.getServerIP() << " At port: " << client.getPortNumber()
              << std::endl;

    return 0;
}