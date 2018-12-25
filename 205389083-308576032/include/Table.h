#ifndef TABLE_H_
#define TABLE_H_

#include <vector>
#include "Customer.h"
#include "Dish.h"

typedef std::pair<int, Dish> OrderPair;

class Table{
public:
    Table();
    Table(int t_capacity);
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    std::vector<OrderPair>& getLastOrder();
    void order(const std::vector<Dish> &menu);
    void openTable();
    void closeTable();
    int getBill();
    bool isOpen();
    virtual ~Table(); //destructor
    Table(const Table& other); //copy constructor
    Table& operator=(const Table& other); //operator
    Table(Table&& other); //move constructor
    Table& operator=(Table&& other);
    void clearLastOrder();
    
private:
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order in a table - (customer_id, Dish)
    std::vector<OrderPair> lastOrder;
};


#endif
