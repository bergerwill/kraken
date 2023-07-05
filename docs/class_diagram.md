        +----------------+          +------------------+       +------------------+
        |   OrderBook    |          | OrderBookManager |       |      Trade       |
        +----------------+          +------------------+       +------------------+
        | - topBid: int  |          | - start()        |       | - userIdBuy: std::string |
        | - topAsk: int  |          | - stop()         |       | - userOrderIdBuy: int   |
        | - buyOrders:   |          | - publishUpdates()        | - userIdSell: std::string|
        |   std::vector<Order> |     | - processMessages()       | - userOrderIdSell: int  |
        | - sellOrders:  |          | - processMessage(message: const std::string&) | - price: int            |
        |   std::vector<Order> |     | - processLimitBuyOrder(order: Order&)       | - quantity: int         |
        |                  |          | - processLimitSellOrder(order: Order&)      |                        |
        |                  |          | - updateTopOfBook()            |                        |
        |                  |          | - getTotalQuantity(orders: std::vector<Order>&): int |               |
        |                  |          | - parseOrder(message: const std::string&): Order |             |
        |                  |          | - static orderBooks: std::map<std::string, OrderBook> | |
        +----------------+          +------------------+       +------------------+
                  |                           |
                  |                           |
            +---------+                 +---------+
            |  Order  |                 |  Trade  |
            +---------+                 +---------+
            | - userId: std::string    | - userIdBuy: std::string |
            | - orderId: std::string   | - userOrderIdBuy: int   |
            | - symbol: std::string    | - userIdSell: std::string|
            | - price: int             | - userOrderIdSell: int  |
            | - quantity: int          | - price: int            |
            | - side: char             | - quantity: int         |
            | - userOrderId: int       |                        |
            +------------------------>  |                        |
                  |                  |                        |
                  +------------------+                        |
                  |                                           |
                  +-------------------                        |
                  |                                           |
                  +-------------------------------------------+
