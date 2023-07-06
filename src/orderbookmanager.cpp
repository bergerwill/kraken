#include "orderbookmanager.h"

#include <iostream>
#include <boost/array.hpp>
#include <thread>
#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::udp;

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

    if (message[0] == 'F') {
        order.flush = true;
        return order;
    }

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

std::string getOrderString(const Order& order) {
    std::ostringstream oss;
    oss << "Received Order:"
        << " User: " << order.userId
        << " Symbol: " << order.symbol
        << " Price: " << order.price
        << " Quantity: " << order.quantity
        << " Side: " << order.side
        << " User Order ID: " << order.userOrderId ;

    return oss.str();
}

void OrderBookManager::processMessage(const std::string &message) {
    Order order = parseOrder(message);
        
    if (order.flush) {
        publishResults("Flushing");
        for (auto& orderBook : orderBooks) {
            orderBook.second.flush();
        }
        return;
    }

    if (order.price == 0) {
        std::cout << "Partial order not supported yet.  It wasn't clear to me what should happen here.";
        return;
    }
    // Process the order as needed
    // Example: Print the parsed order details
    publishResults(getOrderString(order));
    
    // Add the order to the respective OrderBook instance for the given symbol
    if (orderBooks.find(order.symbol) == orderBooks.end()) {
        orderBooks[order.symbol] = OrderBook();
    }        
    orderBooks[order.symbol].processOrder(order);

    publishAck(order);
    publishTrade(order.getTrade()); 
    publishTopOfBookChange(orderBooks[order.symbol], order);
}

void OrderBookManager::publishTrade(Trade& trade) {
    if (trade.quantity != 0) {
        std::string tradeAck = "T, " + trade.userIdBuy + ", " + std::to_string(trade.userOrderIdBuy)
                + ", " + trade.userIdSell + ", " + std::to_string(trade.userOrderIdSell)
                + ", " + std::to_string(trade.price) + ", " + std::to_string(trade.quantity);;
        publishResults(tradeAck);
    }
}

void OrderBookManager::publishAck(Order& order) {
    if (order.ack) {
        std::string orderAck = "A, " + order.userId + ", " + std::to_string(order.userOrderId);
        publishResults(orderAck);
    }
}

void OrderBookManager::publishTopOfBookChange(OrderBook &orderBook, Order &order) {
    std::string topOfBookString;
    if (order.getTrade().quantity > 0) {

         if (order.side == 'B') {
            topOfBookString = 
                    "B, S, " + std::to_string(orderBook.getTopAsk()) + ", " + std::to_string(orderBook.getTotalQuantity(orderBook.getSellOrders()));
                    publishResults(topOfBookString);
        } else {
            topOfBookString = 
            "B, B, " + std::to_string(orderBook.getTopBid()) + ", " + std::to_string(orderBook.getTotalQuantity(orderBook.getBuyOrders()));
                    publishResults(topOfBookString);
        }
    } 
    else if (order.side == 'B') {
        if (order.price >= orderBook.getTopBid()) {
            topOfBookString = 
            "B, B, " + std::to_string(orderBook.getTopBid()) + ", " + std::to_string(orderBook.getTotalQuantity(orderBook.getBuyOrders()));
            publishResults(topOfBookString);
        }
    } 
    else if (order.side == 'S') {
        if (order.price <= orderBook.getTopAsk()) {
            topOfBookString = 
            "B, S, " + std::to_string(orderBook.getTopAsk()) + ", " + std::to_string(orderBook.getTotalQuantity(orderBook.getSellOrders()));
            publishResults(topOfBookString);
        }
    }
    //std::string topOfBookString = 
    //"B, B, " + std::to_string(orderBook.getTopBid()) + ", " + std::to_string(orderBook.getTotalQuantity(orderBook.getBuyOrders())) + "\n"
    //                + "B, S, " + std::to_string(orderBook.getTopAsk()) + ", " + std::to_string(orderBook.getTotalQuantity(orderBook.getSellOrders())) + "\n";

}

void OrderBookManager::processMessages() {
    boost::asio::io_context ioContext;
    boost::asio::ip::udp::socket socket(ioContext, udp::endpoint(udp::v4(), 12345));

    while (true) {
        boost::array<char, 1024> buffer;
        udp::endpoint senderEndpoint;
        size_t bytesRead = socket.receive_from(boost::asio::buffer(buffer), senderEndpoint);
        std::string message(buffer.data(), bytesRead);

        // Output the received message to stdout
        // std::cout << "Received message: " << std::endl << message << std::endl;

        std::vector<std::string> lines = splitStringByNewline(message);
        for (const auto& line : lines) {
            processMessage(line);
        }
        //publishTopOfBookChange(orderBooks[order.symbol]);

    }
}

void OrderBookManager::publishResults(const std::string& message) {
    std::lock_guard<std::mutex> lock(publishMutex);
    publishQueue.push(message);
    publishCondition.notify_one();
}
/**
 * @brief  Publish updates to the market data feed.
 * 
 * The publishUpdates function is responsible for publishing updates to the market data feed in a separate thread from a queue
 */
void OrderBookManager::publishUpdates() {
    
    while (true) {

        // obtain a lock on the publish mutex
        std::unique_lock<std::mutex> lock(publishMutex);

        // upon wait, lock is released and we suspend this thread until there is a message to publish.
        // when thelamda fn return true, we will reaquie the lock and wake up and continue
        publishCondition.wait(lock, [this]() { return !publishQueue.empty() || stopPublishing; });

        if (stopPublishing)
            break;

        std::string message = publishQueue.front();
        publishQueue.pop();
        lock.unlock();

        // Publish the message (e.g., write to console)
        std::cout << "publish: " + message << std::endl;
        // Example: Publishing a trade
        // std::cout << "T, userIdBuy, userOrderIdBuy, userIdSell, userOrderIdSell, price, quantity" << std::endl;
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

