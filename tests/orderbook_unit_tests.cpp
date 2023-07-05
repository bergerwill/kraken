#include <gtest/gtest.h>
#include "orderbook.h"

// Test the processBuyOrder() function
TEST(OrderBookTest, ProcessBuyOrder) {
    OrderBook orderBook;
    Order order1{"user1", "order1", "ABC", 100, 10, 'B', 1};
    Order order2{"user2", "order2", "ABC", 105, 15, 'B', 2};

    orderBook.processBuyOrder(order1);
    orderBook.processBuyOrder(order2);

    // Assert the state of buyOrders and topBid
    ASSERT_EQ(orderBook.getBuyOrders().size(), 2);
    ASSERT_EQ(orderBook.getBuyOrders()[0].price, 105);
    ASSERT_EQ(orderBook.getBuyOrders()[1].price, 100);
    ASSERT_EQ(orderBook.getTopBid(), 105);
}

// Test the processSellOrder() function
TEST(OrderBookTest, ProcessSellOrder) {
    OrderBook orderBook;
    Order order1{"user1", "order1", "ABC", 100, 10, 'S', 1};
    Order order2{"user2", "order2", "ABC", 105, 15, 'S', 2};

    orderBook.processSellOrder(order1);
    orderBook.processSellOrder(order2);

    // Assert the state of seclearllOrders and topAsk
    ASSERT_EQ(orderBook.getSellOrders().size(), 2);
    ASSERT_EQ(orderBook.getSellOrders()[0].price, 100);
    ASSERT_EQ(orderBook.getSellOrders()[1].price, 105);
    ASSERT_EQ(orderBook.getTopAsk(), 100);
}

// Test the processMarketBuyOrder() function
TEST(OrderBookTest, ProcessMarketBuyOrder) {
    OrderBook orderBook;
    Order order1{"user1", "order1", "ABC", 0, 10, 'B', 1};
    Order order2{"user2", "order2", "ABC", 0, 15, 'B', 2};
    Order order3{"user3", "order3", "ABC", 100, 5, 'S', 3};

    orderBook.processSellOrder(order3);
    orderBook.processMarketBuyOrder(order1);

    // Assert the state of sellOrders, buyOrders, and topBid
    ASSERT_EQ(orderBook.getSellOrders().size(), 0);
    ASSERT_EQ(orderBook.getBuyOrders().size(), 1);
    ASSERT_EQ(orderBook.getBuyOrders()[0].price, 100);
    ASSERT_EQ(orderBook.getBuyOrders()[0].quantity, 5);
    ASSERT_EQ(orderBook.getTopBid(), 100);

    orderBook.processMarketBuyOrder(order2);

    // Assert the state of sellOrders, buyOrders, and topBid
    ASSERT_EQ(orderBook.getSellOrders().size(), 0);
    ASSERT_EQ(orderBook.getBuyOrders().size(), 1);
    ASSERT_EQ(orderBook.getBuyOrders()[0].price, 100);
    ASSERT_EQ(orderBook.getBuyOrders()[0].quantity, 0);
    ASSERT_EQ(orderBook.getTopBid(), 100);
}

// Test the processMarketSellOrder() function
TEST(OrderBookTest, ProcessMarketSellOrder) {
    OrderBook orderBook;
    Order order1{"user1", "order1", "ABC", 100, 5, 'B', 1};
    Order order2{"user2", "order2", "ABC", 100, 10, 'B', 2};
    Order order3{"user3", "order3", "ABC", 0, 8, 'S', 3};

    orderBook.processBuyOrder(order1);
    orderBook.processMarketSellOrder(order3);

    // Assert the state of sellOrders, buyOrders, and topAsk
    ASSERT_EQ(orderBook.getSellOrders().size(), 0);
    ASSERT_EQ(orderBook.getBuyOrders().size(), 1);
    ASSERT_EQ(orderBook.getBuyOrders()[0].price, 100);
    ASSERT_EQ(orderBook.getBuyOrders()[0].quantity, 3);
    ASSERT_EQ(orderBook.getTopAsk(), -1);

    orderBook.processMarketSellOrder(order2);

    // Assert the state of sellOrders, buyOrders, and topAsk
    ASSERT_EQ(orderBook.getSellOrders().size(), 0);
    ASSERT_EQ(orderBook.getBuyOrders().size(), 1);
    ASSERT_EQ(orderBook.getBuyOrders()[0].price, 100);
    ASSERT_EQ(orderBook.getBuyOrders()[0].quantity, 0);
    ASSERT_EQ(orderBook.getTopAsk(), -1);
}

// Test the processLimitBuyOrder() function
TEST(OrderBookTest, ProcessLimitBuyOrder) {
    OrderBook orderBook;
    Order order1{"user1", "order1", "ABC", 100, 10, 'B', 1};
    Order order2{"user2", "order2", "ABC", 105, 15, 'B', 2};
    Order order3{"user3", "order3", "ABC", 98, 8, 'S', 3};

    orderBook.processSellOrder(order3);
    orderBook.processLimitBuyOrder(order1);

    // Assert the state of sellOrders, buyOrders, and topBid
    ASSERT_EQ(orderBook.getSellOrders().size(), 0);
    ASSERT_EQ(orderBook.getBuyOrders().size(), 1);
    ASSERT_EQ(orderBook.getBuyOrders()[0].price, 98);
    ASSERT_EQ(orderBook.getBuyOrders()[0].quantity, 8);
    ASSERT_EQ(orderBook.getTopBid(), 100);

    orderBook.processLimitBuyOrder(order2);

    // Assert the state of sellOrders, buyOrders, and topBid
    ASSERT_EQ(orderBook.getSellOrders().size(), 0);
    ASSERT_EQ(orderBook.getBuyOrders().size(), 2);
    ASSERT_EQ(orderBook.getBuyOrders()[0].price, 98);
    ASSERT_EQ(orderBook.getBuyOrders()[0].quantity, 8);
    ASSERT_EQ(orderBook.getBuyOrders()[1].price, 105);
    ASSERT_EQ(orderBook.getBuyOrders()[1].quantity, 15);
    ASSERT_EQ(orderBook.getTopBid(), 105);
}

// Test the processLimitSellOrder() function
TEST(OrderBookTest, ProcessLimitSellOrder) {
    OrderBook orderBook;
    Order order1{"user1", "order1", "ABC", 100, 5, 'S', 1};
    Order order2{"user2", "order2", "ABC", 105, 10, 'S', 2};
    Order order3{"user3", "order3", "ABC", 98, 8, 'B', 3};

    orderBook.processBuyOrder(order3);
    orderBook.processLimitSellOrder(order1);

    // Assert the state of sellOrders, buyOrders, and topAsk
    ASSERT_EQ(orderBook.getSellOrders().size(), 1);
    ASSERT_EQ(orderBook.getSellOrders()[0].price, 100);
    ASSERT_EQ(orderBook.getSellOrders()[0].quantity, 5);
    ASSERT_EQ(orderBook.getBuyOrders().size(), 0);
    ASSERT_EQ(orderBook.getTopAsk(), 100);
}