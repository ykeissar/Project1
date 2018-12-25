#include "Table.h"
#include <iostream>

//constructor
Table::Table(int t_capacity):capacity(t_capacity),open(false),customersList{}, orderList{}, lastOrder{}{

}

//destructor
Table::~Table(){
	orderList.clear();
	for(unsigned int i=0;i<customersList.size();i++){
		delete customersList[i];
	}
    lastOrder.clear();
	customersList.clear();
}

//copy constructor
Table::Table(const Table& other):capacity(other.capacity),open(other.open),customersList{}, orderList{}, lastOrder{}{

	for(unsigned int i=0;i<other.orderList.size();i++)
		orderList.push_back(other.orderList[i]);
	for(unsigned int i=0;i<other.lastOrder.size();i++)
        lastOrder.push_back(other.lastOrder[i]);
	for(unsigned int i=0;i<other.customersList.size();i++){
	    customersList.push_back(other.customersList[i]->clone());
	}
}

//copy assingment
Table& Table::operator=(const Table& other){
	if(&other!=this){
		orderList.clear();
        	lastOrder.clear();
		//for(unsigned int i=0;i<customersList.size();i++)
		//	delete customersList[i];
		customersList.clear();
		for(unsigned int i=0;i<other.customersList.size();i++){
			Customer* toAdd=other.customersList[i]->clone();
			customersList.push_back(toAdd);
		}
		for(unsigned int i=0;i<other.orderList.size();i++)
			orderList.push_back(other.orderList[i]);
       		for(unsigned int i=0;i<other.lastOrder.size();i++)
            	lastOrder.push_back(other.lastOrder[i]);
		capacity=other.capacity;
		open=other.open;
	}
	return *this;
}

//move constructor 
Table::Table(Table&& other):capacity(other.capacity),open(other.open),customersList{}, orderList{}, lastOrder{}{

	for(unsigned int i=0;i<other.customersList.size();i++){
		customersList.push_back(other.customersList[i]->clone());
	}
	for(unsigned int i=0;i<other.orderList.size();i++)
		orderList.push_back(other.orderList[i]);
	for(unsigned int i=0;i<other.lastOrder.size();i++)
		lastOrder.push_back(other.lastOrder[i]);
    for(unsigned int i=0;i<other.customersList.size();i++) {
        if(other.customersList[i]!= nullptr) {
            delete other.customersList[i];
            customersList[i] = nullptr;
        }
    }
	other.customersList.clear();
	other.orderList.clear();
}

//move assingment
Table& Table::operator=(Table&& other){
	if(&other!=this){
		orderList.clear();
        	lastOrder.clear();
		for(unsigned int i=0;i<customersList.size();i++)
			delete customersList[i];
		customersList.clear();
		for(unsigned int i=0;i<other.customersList.size();i++){
			Customer* toAdd=other.customersList[i]->clone();
			customersList.push_back(toAdd);
		}
		for(unsigned int i=0;i<other.orderList.size();i++)
			orderList.push_back(other.orderList[i]);
		for(unsigned int i=0;i<other.customersList.size();i++){
			delete other.customersList[i];
            other.customersList[i]= nullptr;
            }
		other.customersList.clear();
		other.orderList.clear();
        	other.lastOrder.clear();
		}
	return *this;
}

int Table::getCapacity() const{
	return capacity;
}
//check if checking is needed
void Table::addCustomer(Customer* customer){
	unsigned int i=capacity;
	if(customersList.size()<i){	
		customersList.push_back(customer);
	}
}
//check if checking is needed
void Table::removeCustomer(int id){
	for(unsigned int i=0;i<customersList.size();i++){
		if(id==customersList[i]->getId())
			customersList.erase(customersList.begin()+i);
	}
}

Customer* Table::getCustomer(int id){
	for(unsigned int i=0;i<customersList.size();i++){
		if(id==customersList[i]->getId())
			return customersList[i];	
	}	
	return nullptr;
}

std::vector<Customer*>& Table::getCustomers(){ return customersList;}

std::vector<OrderPair>& Table::getOrders(){return orderList;}

std::vector<OrderPair>& Table::getLastOrder(){return lastOrder;}

void Table::order(const std::vector<Dish> &menu){
	for(unsigned int i=0;i<customersList.size();i++){
		std::vector<int> vect= customersList[i]->order(menu);
		for(unsigned int j=0;j<vect.size();j++){
			OrderPair p(customersList[i]->getId(),menu[vect[j]]);
			orderList.push_back(p);
			lastOrder.push_back(p);
		}
		vect.clear();
	}
}

void Table::clearLastOrder(){ lastOrder.clear();}

void Table::openTable(){ open=true;}

void Table::closeTable(){
    open=false;
    for(Customer* customer : customersList)
        delete customer;
    customersList.clear();
    orderList.clear();
    lastOrder.clear();
}

int Table::getBill(){
	int bill=0;
	for(unsigned int i=0;i<orderList.size();i++)
		bill+=(orderList[i].second).getPrice();
	return bill;
}

bool Table::isOpen(){return open;}



