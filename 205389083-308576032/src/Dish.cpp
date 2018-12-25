#include "Dish.h"

//constructor
Dish::Dish(int d_id, std::string d_name, int d_price, DishType d_type):id(d_id),name(d_name),price(d_price),type(d_type) {}

//destructor
Dish::~Dish(){}

//copy constructor
Dish::Dish(const Dish &other):id(other.getId()),name(other.getName()),price(other.getPrice()),type(other.getType()){}

//move constructor
Dish::Dish(Dish &&other):id(other.getId()),name(other.getName()),price(other.getPrice()),type(other.getType()){}

int Dish::getId() const{ return id;}

std::string Dish::getName() const{ return name;}

int Dish::getPrice() const{ return price;}

DishType Dish::getType() const{ return type;}

std::string Dish::toString() const{
    std::string str;
    if(getType()==VEG)
	str=" VEG ";
    if(getType()==SPC)
	str=" SPC ";
    if(getType()==BVG)
	str=" BVG ";
    if(getType()==ALC)
	str=" ALC ";
    std::string price=std::to_string(getPrice())+"NIS";
    str=getName()+str+price;
    return str;	
}


