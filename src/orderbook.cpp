#include <iostream>
#include "orderbook.h"


void OrderBook::processOrder(Order& order) {
    //std::cout << "Processing order: " << order.userId << ", " << order.userOrderId << std::endl;

    if (order.side == 'B') {
         processBuyOrder(order);
    } else if (order.side == 'S') {
         processSellOrder(order);
    } else {
         std::cout << "Invalid order side: " << order.side << " for order: " << std::endl;
    }
}

/*
void OrderBook::publishAcknowledgment(Order& order) {
    std::cout << "A, " << order.userId << ", " << order.userOrderId << std::endl;
}

void OrderBook::publishTrade(Trade& trade) {
    std::cout << "T, " << trade.userIdBuy << ", " << trade.userOrderIdBuy
                << ", " << trade.userIdSell << ", " << trade.userOrderIdSell
                << ", " << trade.price << ", " << trade.quantity << std::endl;
}

void OrderBook::publishTopOfBook() {
    std::cout << "B, B, " << topBid << ", " << getTotalQuantity(buyOrders) << std::endl;
    std::cout << "B, S, " << topAsk << ", " << getTotalQuantity(sellOrders) << std::endl;
}
*/

void OrderBook::processSellOrder(Order& order) {
    if (order.price == 0) {
        processMarketSellOrder(order);
    } else {
        processLimitSellOrder(order);
    }
}

void OrderBook::processBuyOrder(Order& order) {
    if (order.price == 0) {
        processMarketBuyOrder(order);
    } else {
        processLimitBuyOrder(order);
    }
}

void OrderBook::flush() {
    buyOrders.clear();
    sellOrders.clear();
    topBid = 0;
    topAsk = 0;
}

void OrderBook::processMarketBuyOrder(Order& marketBuyOrder) {   
    std::string result;
    for (auto it = sellOrders.begin(); it != sellOrders.end();it++) {
        if (marketBuyOrder.quantity == 0) {
            break;
        }
        if (marketBuyOrder.quantity >= it->quantity) {
            Trade trade(marketBuyOrder.userId, marketBuyOrder.userOrderId, it->userId, it->userOrderId,
                        it->price, it->quantity);
            marketBuyOrder.addTrade(trade);
            //publishTrade(trade);
            marketBuyOrder.quantity -= it->quantity;
            it = sellOrders.erase(it);
        } else {
            Trade trade{it->userId, it->userOrderId, it->userId, it->userOrderId,
                        it->price, it->quantity};
            marketBuyOrder.addTrade(trade);
            //publishTrade(trade);
            it->quantity -= it->quantity;
            marketBuyOrder.quantity = 0;
            break;
        }

    }
}

void OrderBook::processMarketSellOrder(Order& order) {
    for (auto it = buyOrders.begin(); it != buyOrders.end();) {
        if (order.quantity == 0) {
            break;
        }

        if (order.quantity >= it->quantity) {
            Trade trade{it->userId, it->userOrderId, order.userId, order.userOrderId,
                        it->price, it->quantity};
            order.addTrade(trade);
            //publishTrade(trade);
            order.quantity -= it->quantity;
            it = buyOrders.erase(it);
        } else {
            Trade trade{it->userId, it->userOrderId, order.userId, order.userOrderId,
                        it->price, order.quantity};
            order.addTrade(trade);
            //publishTrade(trade);
            it->quantity -= order.quantity;
            order.quantity = 0;
            break;
        }
    }

    if (order.quantity > 0) {
        sellOrders.push_back(order);
        //publishAcknowledgment(order);
    }

    updateTopOfBook();
}

void OrderBook::processLimitBuyOrder(Order& order) {
    bool isMatched = false;
    for (auto it = sellOrders.begin(); it != sellOrders.end(); ++it) {
        if (order.price >= it->price) {
            if (order.quantity >= it->quantity) {
                Trade trade{order.userId, order.userOrderId, it->userId, it->userOrderId,
                            it->price, it->quantity};
                //publishTrade(trade);
                order.addTrade(trade);
                order.quantity -= it->quantity;
                sellOrders.erase(it);
                isMatched = true;
                order.ack = true;
                break;
            }
        }
    }

    if (!isMatched) {
        buyOrders.push_back(order);
        order.ack = true;
    }

    updateTopOfBook();
}

 void OrderBook::processLimitSellOrder(Order& order) {
    bool isMatched = false;
    for (auto it = buyOrders.begin(); it != buyOrders.end(); ++it) {
        if (order.price <= it->price) {
            if (order.quantity >= it->quantity) {
                Trade trade{ order.userId, order.userOrderId, it->userId, it->userOrderId,
                            it->price, it->quantity};
                order.addTrade(trade);
                //publishTrade(trade);
                order.quantity -= it->quantity;
                buyOrders.erase(it);
                isMatched = true;
                order.ack = true;
                break;
            }
        }
    }

    if (!isMatched) {
        sellOrders.push_back(order);
        order.ack = true;
    }

    updateTopOfBook();
}

void OrderBook::updateTopOfBook() {
    if (!buyOrders.empty()) {
        const auto& maxBuyOrder = *std::max_element(buyOrders.begin(), buyOrders.end(),
                                                    [](const Order& a, const Order& b) {
                                                        return a.price < b.price;
                                                    });
        topBid = maxBuyOrder.price;
    } else {
        topBid = -1;
    }

    if (!sellOrders.empty()) {
        const auto& minSellOrder = *std::min_element(sellOrders.begin(), sellOrders.end(),
                                                        [](const Order& a, const Order& b) {
                                                            return a.price < b.price;
                                                        });
        topAsk = minSellOrder.price;
    } else {
        topAsk = -1;
    }

    //publishTopOfBook();
}

int OrderBook::getTotalQuantity(std::vector<Order>& orders) {
    int totalQuantity = 0;
    for (auto &order : orders) {
        totalQuantity += order.quantity;
    }
    return totalQuantity;
}

void OrderBook::processCancellation(const std::string& orderId) {
    // TODO: Implement cancellation logic
    std::cout << "Processing cancellation: " << orderId << std::endl;
}
