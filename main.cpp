#include "order_book.h"
#include <iostream>
using namespace std;

int main()
{
    string input = "BUY,AAPL,150.5,100;SELL,AAPL,150.5,50;BUY,AAPL,150.6,80;SELL,AAPL,150.4,60";
    Order orders[10];
    int n = 0;

    if (!parseInput(input, orders, n))
    {
        cout << "Invalid input\n";
        return 1;
    }

    PriceLevel buyLevels[100], sellLevels[100];

    int buyCount = 0, sellCount = 0;

    Trade *tradeHead = nullptr;

    buildOrderBook(orders, n, buyLevels, buyCount, sellLevels, sellCount);

    matchOrders(orders, buyLevels, buyCount, sellLevels, sellCount, tradeHead);

    printTrades(tradeHead);

    for (int i = 0; i < buyCount; i++)
    {
        while (buyLevels[i].orders)
        {
            OrderNode *temp = buyLevels[i].orders;
            buyLevels[i].orders = temp->next;
            delete temp;
        }
    }

    for (int i = 0; i < sellCount; i++)
    {
        while (sellLevels[i].orders)
        {
            OrderNode *temp = sellLevels[i].orders;
            sellLevels[i].orders = temp->next;
            delete temp;
        }
    }

    while (tradeHead)
    {
        Trade *temp = tradeHead;
        tradeHead = temp->next;
        delete temp;
    }

    return 0;
}