#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"



class Restaurant{
public:
    Restaurant();
    Restaurant(const std::string &configFilePath);
    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();
    virtual ~Restaurant();
    Restaurant(const Restaurant& other);
    Restaurant(Restaurant&& other);
    Restaurant& operator=(Restaurant&& other);
    Restaurant& operator=(const Restaurant& other);     
    void closeRest();
    unsigned int getNumOfActions() const; //to imply    
private:
    bool open;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
    int cCount;//Customers in system
};

#endif
