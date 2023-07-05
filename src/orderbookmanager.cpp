#include "orderbookmanager.h"

#include <iostream>
#include <boost/array.hpp>
#include <thread>
#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::udp;

std::map<std::string, OrderBook> OrderBookManager::orderBooks;

Order OrderBookManager::parseOrder(const std::string& message) {
    Order order;

    // Create a string stream to parse the message
    std::stringstream iss(message);

    // Read the individual components of the message
    char command;
    char comma;

    iss >> command >> comma;
    if (command != 'N') {
        // Handle error: Invalid command, expected 'N'
        // You can throw an exception or return an error code
    }

   std::string field;

    // Parse user
    std::getline(iss >> std::ws, order.userId, ',');
    // Parse symbol
    std::getline(iss >> std::ws, order.symbol, ',');

    // Parse price
    std::getline(iss >> std::ws, field, ',');
    order.price = std::stoi(field);

    // Parse quantity
    std::getline(iss >> std::ws, field, ',');
    order.quantity = std::stoi(field);

    // Parse side
    std::getline(iss >> std::ws, field, ',');
    order.side = field[0];

    // Parse userOrderId
    std::getline(iss >> std::ws, field, ',');
    order.userOrderId = std::stoi(field);

    // Error checking and validation can be added here

    return order;
}

std::vector<std::string> splitStringByNewline(const std::string& str) {
    std::vector<std::string> lines;
    std::stringstream ss(str);
    std::string line;

    while (std::getline(ss, line)) {
        lines.push_back(line);
    }

    return lines;
}

void OrderBookManager::processMessage(const std::string &message) {
    Order order = parseOrder(message);
        
    // Process the order as needed
    // Example: Print the parsed order details
    std::cout << "Received Order:"
                << " User: " << order.userId
                << " Symbol: " << order.symbol
                << " Price: " << order.price
                << " Quantity: " << order.quantity
                << " Side: " << order.side
                << " User Order ID: " << order.userOrderId << std::endl;
    
    // Add the order to the respective OrderBook instance for the given symbol
    if (orderBooks.find(order.symbol) == orderBooks.end()) {
        orderBooks[order.symbol] = OrderBook();
    }        
    //OrderBook& orderBook = orderBooks[order.symbol];
    orderBooks[order.symbol].processOrder(order);
}

void OrderBookManager::processMessages() {
    boost::asio::io_context ioContext;
    boost::asio::ip::udp::socket socket(ioContext, udp::endpoint(udp::v4(), 12345));

    while (true) {
        boost::array<char, 1024> buffer;
        udp::endpoint senderEndpoint;
        size_t bytesRead = socket.receive_from(boost::asio::buffer(buffer), senderEndpoint);
        std::cout << " We are receiving a message";

        std::string message(buffer.data(), bytesRead);

        // Output the received message to stdout
        // std::cout << "Received message: " << std::endl << message << std::endl;

        std::vector<std::string> lines = splitStringByNewline(message);
        for (const auto& line : lines) {
            processMessage(line);
        }
        
    }
}

void OrderBookManager::publishUpdates() {
    bool publishUpdates = false;
    while (true) {
        // TODO: Implement publishing logic
        if (publishUpdates) {
            // Example: Publishing a trade
            std::cout << "T, userIdBuy, userOrderIdBuy, userIdSell, userOrderIdSell, price, quantity" << std::endl;
        }
    }
}

void OrderBookManager::start() {
    // Start the message processing thread
   
    std::thread messageThread(&OrderBookManager::processMessages, this);

    // Start the publishing thread
    std::thread publishThread(&OrderBookManager::publishUpdates, this);

    // Wait for the threads to finish
    messageThread.join();
    publishThread.join();
}

void OrderBookManager::stop() {
    std::cout << "Stop OrderBookManager" << std::endl; 
}

