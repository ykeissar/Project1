#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Dish.h"

typedef std::pair<int, int> PrevOrder;//previus alcCustomer order - <Id,Price>

class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual std::vector<int> order(const std::vector<Dish> &menu) = 0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;
    virtual ~Customer();
    Customer(const Customer &c);
    virtual std::string getType() const = 0;
    Customer(Customer &&c);
    virtual Customer* clone() = 0;
private:
    const std::string name;
    const int id;
};


class VegetarianCustomer : public Customer {
public:
    VegetarianCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    virtual ~VegetarianCustomer();
    VegetarianCustomer(const VegetarianCustomer &other);
    VegetarianCustomer(VegetarianCustomer &&other);
    std::string getType()const;
    virtual Customer* clone();
private:
};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    virtual ~CheapCustomer();
    std::string getType()const;
    CheapCustomer(const CheapCustomer &other);
    CheapCustomer(CheapCustomer &&other);
    virtual Customer* clone();
private:
    bool firstOrder;
};


class SpicyCustomer : public Customer {
public:
    SpicyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    virtual ~SpicyCustomer();
    SpicyCustomer(const SpicyCustomer &other);
    SpicyCustomer(SpicyCustomer &&other);
    std::string getType() const;
    virtual Customer* clone();
private:
    bool firstOrder;
};


class AlchoholicCustomer : public Customer {
public:
    AlchoholicCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    virtual ~AlchoholicCustomer();
    AlchoholicCustomer(const AlchoholicCustomer &other);
    AlchoholicCustomer(AlchoholicCustomer &&other);
    std::string getType() const;
    virtual Customer* clone();
    PrevOrder getPrevOrder() const;
private:
    PrevOrder prevOrder;
};

#endif
