#include <vector>
#include <string>

/**
 * @brief  Trade struct that represents a trade between two orders.
 * 
 * The Trade struct is used to publish trades to the market data feed.
*/
struct Trade {
    std::string userIdBuy = "";
    int userOrderIdBuy = 0;
    std::string userIdSell = "";
    int userOrderIdSell = 0;
    int price = 0;
    int quantity = 0;
    std::string msg = "";
    Trade() = default;  // Default constructor
    Trade(std::string userIdBuy, int userOrderIdBuy, std::string userIdSell, int userOrderIdSell, int price, int quantity) :
        userIdBuy(userIdBuy), userOrderIdBuy(userOrderIdBuy), userIdSell(userIdSell), userOrderIdSell(userOrderIdSell), price(price), quantity(quantity) {}
};

/**
 * @brief  OrderBook class that represents the order book for a single symbol.
 * 
 * The OrderBook class is responsible for processing orders and cancellations
*/
class Order {

private:
    std::string msg;
    Trade trade;

public:
    std::string userId = ""; 
    std::string symbol = "";
    int price = 0;
    int quantity = 0;
    char side = ' ';
    int userOrderId = 0;
    bool flush = false;
    bool ack = false;

    Order() = default;  // Default constructor
    Order(std::string userId, std::string symbol, int price, int quantity, char side, int userOrderId) :
        userId(userId), symbol(symbol), price(price), quantity(quantity), side(side), userOrderId(userOrderId) {}  

     /**
     * @brief  add trade.  This may become a list
    */
    void addTrade(Trade& newTrade) {
        trade = newTrade;
    }

    /**
     * get the trades for this order
    */  
    Trade& getTrade() {
        return trade;
    }

    /**
     * @brief  add msg.  This may become a list
    */
   void addMsg(std::string& message) {
        msg = message;
    }

    /**
     * get the messages for this order
    */
    std::string getMsgs() {
        return msg;
    }
};


/**
 * brief  OrderBook class that represents the order book for a single symbol.
 * 
 * The OrderBook class is responsible for processing orders and cancellations
*/
class OrderBook {

    /**
     * @brief  Top of book bid price
     * 
     * The top of book bid price is the highest price that a buyer is willing to pay for a security.
    */
    int topBid;

    /**
     * @brief  Top of book ask price
     * 
     * The top of book ask price is the lowest price that a seller is willing to accept for a security.
    */
    int topAsk;

    /**
     * @brief  Vector of buy orders
     * 
     * The buy orders vector contains all of the buy orders for the given symbol.
    */
    std::vector<Order> buyOrders;

    /**
     * @brief  Vector of sell orders
     * 
     * The sell orders vector contains all of the sell orders for the given symbol.
    */
    std::vector<Order> sellOrders;


public:  
    /**
     * @brief  Process an order
     * 
     * The processOrder function is responsible for processing an order. It will call the appropriate function based on the order side.
    */
    void processOrder(Order& order);

    /** 
     * @brief  Process a buy order
     * 
     * The processBuyOrder function is responsible for processing a buy order. It will call the appropriate function based on the order type.
    */
    void processCancellation(const std::string& orderId);

    /**
     * @brief  Process a sell order
     * 
     * The processSellOrder function is responsible for processing a sell order. It will call the appropriate function based on the order type.
    */
    void printTopOfBook();

    /**
     * @brief  Flush the order book
     * 
     *   The flush function is responsible for flushing the order book.
    */
    void flush();

//private: - commenting out so we can perform unit tests using gtest

    /**
     * @brief  Process a market buy order
    */
    void processBuyOrder(Order& order);
    
    /**
     * @brief  Process a market sell order  
    */
    void processSellOrder(Order& order);

    /**
     * @brief  Process a market buy order
    */
    void processMarketBuyOrder(Order& order);

    /**
     * @brief  Process a market sell order
    */
    void processMarketSellOrder(Order& order);

    /**
     * @brief  Process a limit buy order
    */
    void processLimitBuyOrder(Order& order);

    /**
     * @brief  Process a limit sell order
    */
    void processLimitSellOrder(Order& order);

    /**
     * @brief  Publish a trade
    */
    void publishTrade(Trade& trade);

    /**
     * @brief  Publish the top of book
    */
    void publishTopOfBook();

    /**
     * @brief  Publish an acknowledgment
    */
    void publishAcknowledgment(Order & order);

    /**
     * @brief  Update the top of book
    */
    void updateTopOfBook();

    /**
     * 
    */
    int getTotalQuantity(std::vector<Order>& orders);

    //
    // Getter setters used for unit testing
    //
    int getTopBid() const {
        return topBid;
    }

    // Setter for topBid
    void setTopBid(int value) {
        topBid = value;
    }

    // Getter for topAsk
    int getTopAsk() const {
        return topAsk;
    }

    // Setter for topAsk
    void setTopAsk(int value) {
        topAsk = value;
    }

    // Getter for buyOrders
    std::vector<Order>& getBuyOrders() {
        return buyOrders;
    }

    // Setter for buyOrders
    void setBuyOrders( std::vector<Order>& orders) {
        buyOrders = orders;
    }

    // Getter for sellOrders
    std::vector<Order>& getSellOrders()  {
        return sellOrders;
    }

    // Setter for sellOrders
    void setSellOrders( std::vector<Order>& orders) {
        sellOrders = orders;
    }
};