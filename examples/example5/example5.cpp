/**
 * @file example5.cpp
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
 * @brief This program demonstrate how to do UDP IPv4 broadcasting: every machine in the local network that reads to the PORT will receive the broadcast message
 * $g++ -Wall example5.cpp -lpigeon -o example5
 *
 * Run program ``example3s.cpp'' to see the result
 *
 */

// Your local network broadcast address, usually the unmasked bit all set to 1.
#define BROADCAST "255.255.255.255"
#define PORT 2022

int main()
{
    std::string broadcastMessage{"Attention: Nice to meet you on the Internet!"};
    serverSideUDP loudspeaker;
    loudspeaker.setPortNumber(PORT);

    loudspeaker.startBroadcast(BROADCAST);
    loudspeaker.printErrorLog();
    loudspeaker.broadcasting(broadcastMessage.c_str(), strlen(broadcastMessage.c_str()));
    loudspeaker.printErrorLog();
    loudspeaker.closeBroadcast();

    return 0;
}