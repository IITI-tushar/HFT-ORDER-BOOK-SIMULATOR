#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include <string>

using namespace std;

struct Order
{
    string id;
    bool isBuy;
    float price;
    int quantity;
};

struct OrderNode
{
    string id;
    int quantity;
    int orderIndex;
    OrderNode *next;
    OrderNode(string i, int q, int idx);
};

struct PriceLevel
{
    float price;
    OrderNode *orders;
    PriceLevel();
    PriceLevel(float p);
};

struct Edge
{
    int buyLevel;
    int sellLevel;
    int buyOrderIndex;
    int sellOrderIndex;
};

struct Trade
{
    string id;
    float price;
    int quantity;
    Trade *next;
    Trade(string i, float p, int q);
};

bool parseInput(string input, Order *orders, int &n);
void buildOrderBook(Order *orders, int n, PriceLevel *buyLevels, int &buyCount, PriceLevel *sellLevels, int &sellCount);
void matchOrders(Order *orders, PriceLevel *buyLevels, int buyCount, PriceLevel *sellLevels, int sellCount, Trade *&tradeHead);
void printTrades(Trade *head);

#endif
