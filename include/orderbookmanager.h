#include <map>
#include <vector>
#include <string>
#include <queue>
#include <sstream>
#include "orderbook.h"

/**
 * @brief  OrderBookManager class.
 * 
 * The OrderBookManager class is responsible for processing messages from the market data feed.
*/
class OrderBookManager {

public:
    /**
     * @brief  Start the OrderBookManager.
     * 
     * The start() function starts the OrderBookManager and begins processing messages from the market data feed.
    */
    void start();

    /**
     * @brief  Stop the OrderBookManager.
     * 
     * The stop() function stops the OrderBookManager.
    */
    void stop();

    void publishMessage(const std::string& message);

private:

 /**
     * @brief stores all the OrderBook objects managed bu this instance.
     * 
     * The processCancellation function is responsible for processing a cancellation.
    */
    std::map<std::string, OrderBook> orderBooks;

    std::mutex publishMutex;
    std::condition_variable publishCondition;
    std::queue<std::string> publishQueue;
    bool stopPublishing = false;



    /**
     * @brief  Process a message.
     * 
     * The processMessage function is responsible for processing a single message from the market data feed.
    */  
    void publishUpdates();

    void publishResults(const std::string& message);

    /**
     * @brief  enquues the order details to be published.
     * 
    */
    void publishAck(Order& order);

    /**
     * @brief  enquues the trade details to be published.
     *  
    */
    void publishTrade(Trade& trade);

    /**
     * @brief  enquues the top of book details to be published.
    */
    void publishTopOfBookChange(OrderBook &orderBook, Order &order);
    
    /**
     * @brief  Process a message.
     *
     * The processMessage function is responsible for processing a single message from the market data feed.  
    */
    void processMessages();

    /**
     * @brief  Process a message.
     * 
     * The processMessage function is responsible for processing a single message from the market data feed.
    */
    void processMessage(const std::string &message);

    /**
     * @brief  Split a string by newline.
     * 
     * The splitStringByNewline function is responsible for splitting a string by newline.
    */
    void processLimitBuyOrder(Order& order);

    /**
     * @brief  Process a market buy order.
     * 
     * The processMarketBuyOrder function is responsible for processing a market buy order.
    */
    void processLimitSellOrder(Order& order);

    /**
     * @brief  Process a market sell order.
     * 
     * The processMarketSellOrder function is responsible for processing a market sell order.
    */
    void updateTopOfBook();

    /**
     * @brief  Process a cancellation.
     * 
     * The processCancellation function is responsible for processing a cancellation.
    */
    int getTotalQuantity(std::vector<Order>& orders);

    /**
     * @brief  Process a cancellation.
     * 
     * The parseOrder function is responsible for parsing an order and return an Order object.
    */
    Order static parseOrder(const std::string& message);

   


};