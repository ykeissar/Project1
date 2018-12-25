#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};

//Forward declaration
class Restaurant;

class BaseAction{
public:
    BaseAction();
    BaseAction(std::string msg,ActionStatus stat);
    ActionStatus getStatus() const;
    virtual void act(Restaurant& restaurant)=0;
    virtual std::string toString() const=0;
    virtual ~BaseAction(); 
    virtual BaseAction* clone() =0;
protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
    void setStatus(ActionStatus other);
    void setErrorMsg(std::string other);
private:
    std::string errorMsg;
    ActionStatus status;
};

class OpenTable : public BaseAction {
public:
    OpenTable(int id, std::vector<Customer *> &customersList);
    void act(Restaurant &restaurant);
    std::string toString() const;
    OpenTable(const OpenTable &other);
    virtual ~OpenTable();
    OpenTable& operator=(const OpenTable& other);
    OpenTable(OpenTable&& other);
    OpenTable& operator=(OpenTable&& other);
    virtual BaseAction* clone();
    int getTableId() const;
private:
    const int tableId;
    std::vector<Customer *> customers;
};


class Order : public BaseAction {
public:
    Order(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual ~Order();
    virtual BaseAction* clone();
    Order(const Order &other);
    Order(Order&& other);
    int getTableId() const;
private:
    const int tableId;
};

class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual ~MoveCustomer();
    virtual BaseAction* clone();
    MoveCustomer(const MoveCustomer &other);
    MoveCustomer(MoveCustomer&& other);
private:
    const int srcTable;
    const int dstTable;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual ~Close();
    virtual BaseAction* clone();
    Close(Close&& other);
    Close(const Close &other);
private:
    const int tableId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual ~CloseAll();
    virtual BaseAction* clone();
    CloseAll(const CloseAll &other);
    CloseAll& operator=(const CloseAll& other);
    CloseAll(CloseAll&& other);
    CloseAll& operator=(CloseAll&& other);
private:
};


class PrintMenu : public BaseAction {
public:
    PrintMenu();
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual ~PrintMenu();
    virtual BaseAction* clone();
    PrintMenu(const PrintMenu &other);
    PrintMenu(PrintMenu&& other);
    PrintMenu& operator=(const PrintMenu& other);
    PrintMenu& operator=(PrintMenu&& other);
private:
};


class PrintTableStatus : public BaseAction {
public:
    PrintTableStatus(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual ~PrintTableStatus();
    virtual BaseAction* clone();
    PrintTableStatus(const PrintTableStatus &other);
    PrintTableStatus(PrintTableStatus&& other);
private:
    const int tableId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual ~PrintActionsLog();
    virtual BaseAction* clone();
    PrintActionsLog(const PrintActionsLog &other);
    PrintActionsLog(PrintActionsLog&& other);
    PrintActionsLog& operator=(const PrintActionsLog& other);
private:
};

extern Restaurant* backup;

class BackupRestaurant : public BaseAction {
public:
    BackupRestaurant();
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual ~BackupRestaurant();
    virtual BaseAction* clone();
    BackupRestaurant(const BackupRestaurant &other);
    BackupRestaurant& operator=(const BackupRestaurant& other);
    BackupRestaurant(BackupRestaurant &&other);
    BackupRestaurant& operator=(const BackupRestaurant &&other);
private:
};


class RestoreResturant : public BaseAction {
public:
    RestoreResturant();
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual ~RestoreResturant();
    virtual BaseAction* clone();
    RestoreResturant(const RestoreResturant &other);
    RestoreResturant& operator=(const RestoreResturant& other);
    RestoreResturant(RestoreResturant&& other);
    RestoreResturant& operator=(RestoreResturant&& other);
private:
};


#endif
