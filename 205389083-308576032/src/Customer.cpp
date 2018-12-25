#include "../include/Customer.h"
#include <limits.h>

//--------------------------------------------Customer-----------------------------------------------
//constructor
Customer::Customer(std::string c_name, int c_id) : name(c_name), id(c_id) {}

//destructor
Customer::~Customer(){}

//copy constructor
Customer::Customer(const Customer &c):name(c.name), id(c.id){}

//move constructor
Customer::Customer(Customer &&c):name(c.name), id(c.id){}

std::string Customer::getName() const{
    return name;
}

int Customer::getId() const{
    return id;
}

//-----------------------------------------AlchoholicCustomer--------------------------------------------------
//constructor
AlchoholicCustomer::AlchoholicCustomer(std::string name, int id):Customer(name,id),prevOrder(-1,0){}

//destructor
AlchoholicCustomer::~AlchoholicCustomer(){}

//copy constructor
AlchoholicCustomer::AlchoholicCustomer(const AlchoholicCustomer &other):Customer(other.getName(),other.getId()),prevOrder(other.prevOrder){}

//move constructor
AlchoholicCustomer::AlchoholicCustomer(AlchoholicCustomer &&other):Customer(other.getName(),other.getId()),prevOrder(other.prevOrder){}

std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu){
    std::vector<int> vect;
    int minDif=INT_MAX;
    int idToInsert=-1;
    bool flag=true;	
    //find min alc dish that wasnt ordered
    for (unsigned int i=0 ; i<menu.size(); i++){        
	    if (menu[i].getType()==ALC&&menu[i].getId()!=prevOrder.first&&menu[i].getPrice()-prevOrder.second>=0){
		    if(menu[i].getPrice()-prevOrder.second==0)
			    flag=menu[i].getId()>prevOrder.first;
		    if(menu[i].getPrice()-prevOrder.second<minDif&&flag){
		        idToInsert=i;
		    minDif=menu[i].getPrice()-prevOrder.second; 	
		    }
		    flag=true;
	    }
    }
    //make sure that the dish isnt same as last and exist
    if(prevOrder.first!=idToInsert&&idToInsert!=-1){
	    vect.push_back(idToInsert);
	    prevOrder.first=idToInsert;
	    prevOrder.second=menu[idToInsert].getPrice();
    }    
    return vect;
}

std::string AlchoholicCustomer::toString() const{
    std::string str= std::to_string(this->getId())+" "+this->getName();
    return str;
}

std::string AlchoholicCustomer::getType() const{return "alc";}

PrevOrder AlchoholicCustomer::getPrevOrder() const{return prevOrder;}

Customer* AlchoholicCustomer::clone(){
	return new AlchoholicCustomer(*this);
}

//-------------------------------------CheapCustomer------------------------------------------------------
//constructor
CheapCustomer::CheapCustomer(std::string name, int id):Customer(name,id),firstOrder(true){}

//destructor
CheapCustomer::~CheapCustomer(){}

//copy constructor
CheapCustomer::CheapCustomer(const CheapCustomer &other):Customer(other.getName(),other.getId()),firstOrder(other.firstOrder){}

//move constructor
CheapCustomer::CheapCustomer(CheapCustomer &&other):Customer(other.getName(),other.getId()),firstOrder(other.firstOrder){}

std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu){
    std::vector<int> vect;
    //if is first order find min price dish
    if(firstOrder) {
        int minPrice = INT_MAX;
        int idToInsert = -1;
        for (unsigned int i = 0; i < menu.size(); i++) {
            if (minPrice > menu[i].getPrice()) {
                minPrice = menu[i].getPrice();
                idToInsert = i;
            }
        }
        if(idToInsert != -1){
            vect.push_back(idToInsert);
            firstOrder=false;
        }
    }
    return vect;
}

std::string CheapCustomer::toString() const{
	std::string outp(std::to_string(this->getId())+" "+this->getName());
	return outp;
}

std::string CheapCustomer::getType() const{return "chp";}

Customer* CheapCustomer::clone(){
	return new CheapCustomer(*this);
}

//-------------------------------------SpicyCustomer------------------------------------------------------
//constructor
SpicyCustomer::SpicyCustomer(std::string name, int id):Customer(name,id),firstOrder(true){}

//destructor
SpicyCustomer::~SpicyCustomer(){}

//copy constructor
SpicyCustomer::SpicyCustomer(const SpicyCustomer &other):Customer(other.getName(),other.getId()),firstOrder(other.firstOrder){}

//move constructor
SpicyCustomer::SpicyCustomer(SpicyCustomer &&other):Customer(other.getName(),other.getId()),firstOrder(other.firstOrder){}

std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu){
    std::vector<int> vect;
    //if first order find most expensive spc dish
    if(firstOrder) {
	int maxPrice=0;
	int idToInsert=-1;
        for (unsigned int i = 0; i < menu.size() ; i++) {
            if (menu[i].getType() == SPC&&menu[i].getPrice()>maxPrice) {
                maxPrice=menu[i].getPrice();
                idToInsert=i;
            }
        }
	if(idToInsert!=-1){
	    vect.push_back(idToInsert);
	    firstOrder=false;	
	}
		
	
    }
    else {
	//if isn't first order find cheapest bvg dish
	int minPrice=INT_MAX;
	int idToInsert=-1;
        for (unsigned int i = 0; i < menu.size(); i++) {
            if (menu[i].getType() == BVG&&menu[i].getPrice()<minPrice) {
		minPrice=menu[i].getPrice();
		idToInsert=i;
	    }
        }
	if(idToInsert!=-1)
	    vect.push_back(idToInsert);
    }
    return vect;
}
std::string SpicyCustomer::toString() const{
    std::string outp(std::to_string(this->getId())+" "+this->getName());
    return outp;
}

std::string SpicyCustomer::getType() const{return "spc";}

Customer* SpicyCustomer::clone(){

    return new SpicyCustomer(*this);
}

//------------------------------------VegetarianCustomer-------------------------------------------------------
//constructor
VegetarianCustomer::VegetarianCustomer(std::string name, int id) : Customer(name,id){}

//destructor
VegetarianCustomer::~VegetarianCustomer(){}

//copy constructor
VegetarianCustomer::VegetarianCustomer(const VegetarianCustomer &other):Customer(other.getName(),other.getId()){}

//move constructor
VegetarianCustomer::VegetarianCustomer(VegetarianCustomer &&other):Customer(other.getName(),other.getId()){}

std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu){
    std::vector<int> vect;
    bool found= false;
    //find first veg dish in menu
    for (unsigned int i=0 ; i<menu.size() && !found ; i++){
        if (menu[i].getType()==VEG){
            vect.push_back(i);
            found=true;
        }
    }
    if (found==false)
        return vect;
    int maxPrice=0;
    int idToInsert=-1;
    //find most expensive bvg dish
    for (unsigned int i=0 ; i<menu.size(); i++){
        if (menu[i].getType()==BVG&&menu[i].getPrice()>maxPrice){
	    maxPrice=menu[i].getPrice();
            idToInsert=i;
        }
    }
    if(idToInsert!=-1)	
	vect.push_back(idToInsert);
    else{
	std::vector<int> empty;
	return empty;
    }
    return vect;
}

std::string VegetarianCustomer::toString() const{
    std::string str(std::to_string(this->getId())+" "+this->getName());
    return str;
}

 std::string VegetarianCustomer::getType() const{return "veg";}

Customer* VegetarianCustomer::clone(){
	return new VegetarianCustomer(this->getName(), this->getId());
}

