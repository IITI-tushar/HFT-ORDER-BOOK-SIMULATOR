#include "order_book.h"
#include <iostream>
#include <cstdlib>

using namespace std;

OrderNode::OrderNode(string i, int q, int idx) : id(i), quantity(q), orderIndex(idx), next(nullptr) {}

PriceLevel::PriceLevel() : price(0.0f), orders(nullptr) {}
PriceLevel::PriceLevel(float p) : price(p), orders(nullptr) {}

Trade::Trade(string i, float p, int q) : id(i), price(p), quantity(q), next(nullptr) {}

bool isValidPrice(float price) { return price > 0; }
bool isValidQuantity(int qty) { return qty > 0; }
bool isValidType(const string &type) { return type == "BUY" || type == "SELL"; }

bool parseInput(string input, Order *orders, int &n)
{
    n = 0;
    size_t pos = 0;
    while (pos < input.size() && n < 10)
    {
        size_t comma1 = input.find(',', pos);
        
        if (comma1 == string::npos)
            return n > 0;
        
        size_t comma2 = input.find(',', comma1 + 1);
        
        if (comma2 == string::npos)
            return false;
        
        size_t comma3 = input.find(',', comma2 + 1);
        
        if (comma3 == string::npos)
            return false;
        
        size_t semi = input.find(';', comma3 + 1);
        
        if (semi == string::npos)
            semi = input.size();

        string type = input.substr(pos, comma1 - pos);
        string id = input.substr(comma1 + 1, comma2 - comma1 - 1);
        string priceStr = input.substr(comma2 + 1, comma3 - comma2 - 1);
        string qtyStr = input.substr(comma3 + 1, semi - comma3 - 1);

        if (!isValidType(type))
            return false;
        
        float price = atof(priceStr.c_str());
        int quantity = atoi(qtyStr.c_str());
        
        if (!isValidPrice(price) || !isValidQuantity(quantity))
            return false;
        
        orders[n] = {id, type == "BUY", price, quantity};
        pos = semi + 1;
        n++;
    }
    
    return true;
}

void insertOrder(PriceLevel *levels, int &levelCount, string id, float price, int quantity, int index)
{
    for (int i = 0; i < levelCount; i++)
    {
        if (levels[i].price == price)
        {
            OrderNode *newNode = new OrderNode(id, quantity, index);
            newNode->next = levels[i].orders;
            levels[i].orders = newNode;
            return;
        }
    }
    levels[levelCount] = PriceLevel(price);
    levels[levelCount].orders = new OrderNode(id, quantity, index);
    levelCount++;
}

void sortPriceLevels(PriceLevel *levels, int levelCount, bool isBuy)
{
    for (int i = 0; i < levelCount - 1; i++)
    {
        for (int j = 0; j < levelCount - i - 1; j++)
        {
            bool swap = isBuy ? levels[j].price < levels[j + 1].price : levels[j].price > levels[j + 1].price;
            if (swap)
            {
                PriceLevel temp = levels[j];
                levels[j] = levels[j + 1];
                levels[j + 1] = temp;
            }
        }
    }
}

void buildOrderBook(Order *orders, int n, PriceLevel *buyLevels, int &buyCount, PriceLevel *sellLevels, int &sellCount)
{
    buyCount = sellCount = 0;
    for (int i = 0; i < n; i++)
    {
        if (orders[i].isBuy)
        {
            insertOrder(buyLevels, buyCount, orders[i].id, orders[i].price, orders[i].quantity, i);
        }
        else
        {
            insertOrder(sellLevels, sellCount, orders[i].id, orders[i].price, orders[i].quantity, i);
        }
    }
    
    sortPriceLevels(buyLevels, buyCount, true);
    
    sortPriceLevels(sellLevels, sellCount, false);
}

void buildGraph(Order *orders, PriceLevel *buyLevels, int buyCount, PriceLevel *sellLevels, int sellCount, Edge *edges, int &edgeCount)
{
    edgeCount = 0;
    for (int i = 0; i < buyCount; i++)
    {
        for (int j = 0; j < sellCount && buyLevels[i].price >= sellLevels[j].price; j++)
        {
            OrderNode *buyNode = buyLevels[i].orders;
            while (buyNode)
            {
                OrderNode *sellNode = sellLevels[j].orders;
                while (sellNode)
                {
                    edges[edgeCount++] = {i, j, buyNode->orderIndex, sellNode->orderIndex};
                    sellNode = sellNode->next;
                }
                buyNode = buyNode->next;
            }
        }
    }
}

void matchOrders(Order *orders, PriceLevel *buyLevels, int buyCount, PriceLevel *sellLevels, int sellCount, Trade *&tradeHead)
{
    Edge edges[100];
    int edgeCount = 0;
    buildGraph(orders, buyLevels, buyCount, sellLevels, sellCount, edges, edgeCount);
    int totalVolume = 0;
    float totalPrice = 0;
    int tradeCount = 0;
    for (int i = 0; i < edgeCount; i++)
    {
        int buyIdx = edges[i].buyOrderIndex;
        int sellIdx = edges[i].sellOrderIndex;
        
        if (orders[buyIdx].quantity == 0 || orders[sellIdx].quantity == 0)
            continue;
        
        int tradeQty = min(orders[buyIdx].quantity, orders[sellIdx].quantity);
        float tradePrice = orders[sellIdx].price;
        
        Trade *newTrade = new Trade(orders[buyIdx].id, tradePrice, tradeQty);
        newTrade->next = tradeHead;
        tradeHead = newTrade;
        
        cout << "Trade: " << tradeQty << " shares of " << orders[buyIdx].id << " at $" << tradePrice << "\n";
        
        orders[buyIdx].quantity -= tradeQty;
        orders[sellIdx].quantity -= tradeQty;
        
        totalVolume += tradeQty;
        
        totalPrice += tradePrice * tradeQty;
        
        tradeCount++;
        if (tradeQty > 50)
        {
            int sellOrderCount = 0;
            for (int j = 0; j < sellCount; j++)
            {
                OrderNode *node = sellLevels[j].orders;
                while (node)
                {
                    sellOrderCount++;
                    node = node->next;
                }
            }
            float impact = tradeQty / 1000.0 * (sellOrderCount > 0 ? 1.0 / sellOrderCount : 1.0);
            for (int j = 0; j < buyCount; j++)
                buyLevels[j].price += impact;
            for (int j = 0; j < sellCount; j++)
                sellLevels[j].price -= impact;
        }
    }
    if (tradeCount > 0)
    {
        cout << "Total Traded Volume: " << totalVolume << " shares\n";
        cout << "Average Trade Price: $" << totalPrice / totalVolume << "\n";
    }
}

void printTrades(Trade *head)
{
    cout << "Stored Trades:\n";
    while (head)
    {
        cout << head->quantity << " shares of " << head->id << " at $" << head->price << "\n";
        head = head->next;
    }
}
