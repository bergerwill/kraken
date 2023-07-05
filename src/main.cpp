#include <iostream>
#include <boost/array.hpp>
#include <thread>
#include <boost/asio.hpp>
#include "orderbookmanager.h"

/**
 * @brief  main function of the OrderBook Simulator program.
 * 
 * Creates an OrderBookManager object and starts it.
 */

int main() {
     std::cout << "Starting OrderBook Simulator!" << std::endl;

    OrderBookManager orderBookManager;
    orderBookManager.start();

    return 0;
} 