 #include "Action.h"
#include "Restaurant.h" 

//------------------------------------------------BaseAction---------------------------------------------
//constructor
BaseAction::BaseAction():errorMsg(""),status(PENDING){}

BaseAction::BaseAction(std::string msg,ActionStatus stat):errorMsg(msg),status(stat){}

ActionStatus BaseAction::getStatus() const{return status;}

void BaseAction::complete(){status=COMPLETED;}

BaseAction::~BaseAction(){}

void BaseAction::error(std::string errorMsg){
    status=ERROR;
    this->errorMsg=errorMsg;
    std::cout<<"Error: "<<errorMsg;
}

void BaseAction::setStatus(ActionStatus other){ status=other; }

void BaseAction::setErrorMsg(std::string other){ errorMsg=other; }

std::string BaseAction::getErrorMsg() const{return errorMsg;}

//--------------------------------------------BackupRestaurant-------------------------------------------------
//constructor
BackupRestaurant::BackupRestaurant(){}

//destructor
BackupRestaurant::~BackupRestaurant(){}

//copy constructor
BackupRestaurant::BackupRestaurant(const BackupRestaurant &other):BaseAction(other.getErrorMsg(),other.getStatus()){}

//move constructor 
BackupRestaurant::BackupRestaurant(BackupRestaurant&& other){
	setErrorMsg(other.getErrorMsg());
	setStatus(other.getStatus());
}

void BackupRestaurant::act(Restaurant &restaurant){
    if(backup!= nullptr) {
        delete backup;
    }
	backup=new Restaurant(restaurant);
	complete();
}

std::string BackupRestaurant::toString() const{
	return "backup Completed\n";
}

BaseAction* BackupRestaurant::clone(){
	return new BackupRestaurant(*this);
}

//------------------------------------------------Close---------------------------------------------
//constructor
Close::Close(int id): tableId(id){}

//destructor
Close::~Close(){}

//copy constructor
Close::Close(const Close &other):BaseAction(other.getErrorMsg(),other.getStatus()),tableId(other.tableId){}

//move constructor 
Close::Close(Close&& other):tableId(other.tableId){
	setErrorMsg(other.getErrorMsg());
	setStatus(other.getStatus());
}

void Close::act(Restaurant &restaurant){
    if(restaurant.getNumOfTables()<tableId || restaurant.getTable(tableId)->isOpen()==false)
        error("Table does not exist or is not open\n");
    else{
        std::string str=std::to_string(restaurant.getTable(tableId)->getBill())+"NIS";
        std::cout<<"Table "<<tableId<<" was closed. Bill "<<str<<std::endl;
        restaurant.getTable(tableId)->closeTable();
        complete();
    }
}

std::string Close::toString() const{
	std::string str= "close "+std::to_string(tableId);
	if(getStatus()==COMPLETED)
            str+=" Completed\n";
        else
            str+=" Error: "+getErrorMsg();
        return str;
}

BaseAction* Close::clone(){
	return new Close(*this);
}

//---------------------------------------------CloseAll------------------------------------------------
//constructor
CloseAll::CloseAll(){}

//destructor
CloseAll::~CloseAll(){}

//copy constructor
CloseAll::CloseAll(const CloseAll &other):BaseAction(other.getErrorMsg(),other.getStatus()){}

//copy assingment
CloseAll& CloseAll::operator=(const CloseAll& other){
	if(&other!=this){
		setErrorMsg(other.getErrorMsg());
		setStatus(other.getStatus());
	}
	return *this;
}

//move constructor 
CloseAll::CloseAll(CloseAll&& other){
	setErrorMsg(other.getErrorMsg());
	setStatus(other.getStatus());
}

//move assingment
CloseAll& CloseAll::operator=(CloseAll&& other){
	if(&other!=this){
		setErrorMsg(other.getErrorMsg());
		setStatus(other.getStatus());
	}
	return *this;
}

void CloseAll::act(Restaurant &restaurant){
	for(int i=0;i<restaurant.getNumOfTables();i++) {//delete tables
        if (restaurant.getTable(i)->isOpen()) {
            Close c(i);
            c.act(restaurant);
        }
    }
	restaurant.closeRest();
    restaurant.getMenu().clear();
	complete();
}

std::string CloseAll::toString() const{
	return "closeall\n";
}

BaseAction* CloseAll::clone(){
	return new CloseAll(*this);
}

//-----------------------------------------------MoveCustomer----------------------------------------------
//constructor
MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTable(src), dstTable(dst), id(customerId) {}

//destructor
MoveCustomer::~MoveCustomer(){}

//copy constructor
MoveCustomer::MoveCustomer(const MoveCustomer &other):BaseAction(other.getErrorMsg(),other.getStatus()),srcTable(other.srcTable),dstTable(other.dstTable),id(other.id){}

//move constructor 
MoveCustomer::MoveCustomer(MoveCustomer&& other):BaseAction(other.getErrorMsg(),other.getStatus()),srcTable(other.srcTable),dstTable(other.dstTable),id(other.id){}

void MoveCustomer::act(Restaurant &restaurant){
    bool flag1= true;
    // If either the origin or destination table are closed or doesn't exist
    if(restaurant.getNumOfTables()<srcTable || restaurant.getNumOfTables()<dstTable || restaurant.getTable(srcTable)->isOpen()==false || restaurant.getTable(dstTable)->isOpen()==false){
        error("Cannot move customer\n");
        flag1=false;
    }
    
    //if no customer with the received id is in the origin table
    if(restaurant.getTable(srcTable)->getCustomer(id)==nullptr && flag1==true){
        error("Cannot move customer\n");
        flag1=false;
    }
    
    // if the destination table has no available seats for additional customers
    unsigned int k =restaurant.getTable(dstTable)->getCapacity();
    if(restaurant.getTable(dstTable)->getCustomers().size()== k && flag1==true){
       error("Cannot move customer\n");
        flag1=false;
    }
    
    if(flag1==true){
        restaurant.getTable(dstTable)->addCustomer(restaurant.getTable(srcTable)->getCustomer(id));
		restaurant.getTable(srcTable)->removeCustomer(id);
		std::vector<OrderPair> temp;
		for (unsigned int i=0 ; i<restaurant.getTable(srcTable)->getOrders().size() ; i++){
			if(restaurant.getTable(srcTable)->getOrders()[i].first==id){
				restaurant.getTable(dstTable)->getOrders().push_back(restaurant.getTable(srcTable)->getOrders()[i]);
			}
	    	else{
	    		temp.push_back(restaurant.getTable(srcTable)->getOrders()[i]);
	   		 }
		}
		restaurant.getTable(srcTable)->getOrders().clear();
		for (unsigned int i=0 ; i<temp.size() ; i++){
	    	restaurant.getTable(srcTable)->getOrders().push_back(temp[i]);

		}
		temp.clear();

        // the origin table has no customers left after this move, the program will close the origin table
        if (restaurant.getTable(srcTable)->getCustomers().size()==0)
            restaurant.getTable(srcTable)->closeTable();
    	complete();
	}
    
}

std::string MoveCustomer::toString() const{
    std::string str= "move " + std::to_string(srcTable) + " " + std::to_string(dstTable) + " " + std::to_string(id);
    if(getStatus()==COMPLETED)
        str+=" Completed\n";
    else
        str+=" Error: " +getErrorMsg();
    return str;
}

BaseAction* MoveCustomer::clone(){
	return new MoveCustomer(*this);
}

//-----------------------------------------------OpenTable----------------------------------------------
//constructor
OpenTable::OpenTable(int id, std::vector<Customer*> &customersList):BaseAction(),tableId(id),customers{}{
	customers=customersList;
}

//copy constructor
OpenTable::OpenTable(const OpenTable &other):BaseAction(other.getErrorMsg(),other.getStatus()),tableId(other.getTableId()),customers{}{
	for(unsigned int i=0;i<other.customers.size();i++){
		Customer* toAdd=other.customers[i]->clone();
		customers.push_back(toAdd);
	}
}

//destructor
OpenTable::~OpenTable(){
	for(unsigned int i=0;i<customers.size();i++)
		delete customers[i];
	customers.clear();
}

//move constructor 
OpenTable::OpenTable(OpenTable&& other):BaseAction(other.getErrorMsg(),other.getStatus()),tableId(other.getTableId()),customers{}{
	for(unsigned int i=0;i<other.customers.size();i++){
		Customer* toAdd=other.customers[i]->clone();
		customers.push_back(toAdd);
	}
	for(unsigned int i=0;i<other.customers.size();i++)
		delete other.customers[i];
	other.customers.clear();	
}

void OpenTable::act(Restaurant &restaurant){
    if(restaurant.getNumOfTables()-1<tableId||tableId<0||restaurant.getTable(tableId)->isOpen())
        error("Table does not exist or is already open\n");
    else {
        for (unsigned int i = 0; i < customers.size(); i++)
            restaurant.getTable(tableId)->addCustomer(customers[i]->clone());
        restaurant.getTable(tableId)->openTable();   
	complete();
    }
    
}


std::string OpenTable::toString() const{
    std::string str="open "+std::to_string(tableId)+" ";
    for (unsigned int i = 0; i < customers.size(); i++)
        str+=customers[i]->getName()+","+customers[i]->getType()+" ";
    if(getStatus()==COMPLETED)
        str+="Completed\n";
    else
        str+="Error: "+getErrorMsg();
    return str;
}

BaseAction* OpenTable::clone(){
	return new OpenTable(*this);
}

int OpenTable::getTableId() const{ return tableId; }

//-----------------------------------------------Order----------------------------------------------
//constructor
Order::Order(int id):BaseAction(),tableId(id){}

//destructor
Order::~Order(){}

//copy constructor
Order::Order(const Order &other):BaseAction(other.getErrorMsg(),other.getStatus()),tableId(other.getTableId()){}

//move constructor 
Order::Order(Order&& other):BaseAction(other.getErrorMsg(),other.getStatus()),tableId(other.getTableId()){}

void Order::act(Restaurant &restaurant){
    if(tableId+1>restaurant.getNumOfTables()||!restaurant.getTable(tableId)->isOpen())
        error("Table does not exist or is not open\n");
    else{
        restaurant.getTable(tableId)->order(restaurant.getMenu());
	std::string toPrint;        
	for(unsigned int i=0;i<restaurant.getTable(tableId)->getLastOrder().size();i++){                    
		std::string str=restaurant.getTable(tableId)->getCustomer(restaurant.getTable(tableId)->getLastOrder()[i].first)->getName()+" ordered "+restaurant.getTable(tableId)->getLastOrder()[i].second.getName()+"\n";
		toPrint+=str;

	}
	std::cout<<toPrint;
	complete();
	restaurant.getTable(tableId)->clearLastOrder();
    }
}

std::string Order::toString() const{
    std::string str= "order " + std::to_string(tableId);
    if(getStatus()==COMPLETED)
        str+=" Completed\n";
    else
        str+=" Error: " + getErrorMsg();
    return str;
}

BaseAction* Order::clone(){
	return new Order(*this);
}

int Order::getTableId() const{ return tableId;}

//----------------------------------------------PrintActionsLog-----------------------------------------------
//constructor
PrintActionsLog::PrintActionsLog():BaseAction(){}

//destructor
PrintActionsLog::~PrintActionsLog(){} 

//copy constructor
PrintActionsLog::PrintActionsLog(const PrintActionsLog &other):BaseAction(other.getErrorMsg(),other.getStatus()){}

//move constructor 
PrintActionsLog::PrintActionsLog(PrintActionsLog&& other){
    setStatus(other.getStatus());
    setErrorMsg(other.getErrorMsg());
}

void PrintActionsLog::act(Restaurant &restaurant){
	for(unsigned int i=0;i<restaurant.getNumOfActions();i++)
		std::cout<<restaurant.getActionsLog()[i]->toString();
	
}

std::string PrintActionsLog::toString() const{
	return "log Completed\n";
}

BaseAction* PrintActionsLog::clone(){
	return new PrintActionsLog(*this);
}

//-----------------------------------------PrintMenu----------------------------------------------------
//constructor
PrintMenu::PrintMenu():BaseAction(){}

//destructor
PrintMenu::~PrintMenu(){}

//copy constructor
PrintMenu::PrintMenu(const PrintMenu &other):BaseAction(other.getErrorMsg(),other.getStatus()){}

//move constructor 
PrintMenu::PrintMenu(PrintMenu&& other):BaseAction(other.getErrorMsg(),other.getStatus()){}

void PrintMenu::act(Restaurant &restaurant){
	for(unsigned int i=0;i<restaurant.getMenu().size();i++){
		std::cout<<restaurant.getMenu()[i].toString()<<std::endl;
	}
	complete();
	
}

std::string PrintMenu::toString() const{
	return "menu Completed\n";
}

BaseAction* PrintMenu::clone(){
	return new PrintMenu(*this);
}

//---------------------------------------PrintTableStatus------------------------------------------------------
//constructor
PrintTableStatus::PrintTableStatus(int id):BaseAction(),tableId(id){}

//destructor
PrintTableStatus::~PrintTableStatus(){}

//copy constructor
PrintTableStatus::PrintTableStatus(const PrintTableStatus &other):BaseAction(other.getErrorMsg(),other.getStatus()),tableId(other.tableId){}

//move constructor 
PrintTableStatus::PrintTableStatus(PrintTableStatus&& other):BaseAction(other.getErrorMsg(),other.getStatus()),tableId(other.tableId){}

void PrintTableStatus::act(Restaurant& restaurant){
	std::string str="Table "+std::to_string(tableId)+" status: "; 	
	if(restaurant.getTable(tableId)->isOpen()){
		str+="open\nCustomers:\n";
		for(unsigned int i=0;i<restaurant.getTable(tableId)->getCustomers().size();i++){
			str+=restaurant.getTable(tableId)->getCustomers()[i]->toString()+"\n";		
		}
		str+="Orders:\n";
		for(unsigned int j=0;j<restaurant.getTable(tableId)->getOrders().size();j++)
			str+=restaurant.getTable(tableId)->getOrders()[j].second.getName()+" "+std::to_string(restaurant.getTable(tableId)->getOrders()[j].second.getPrice())+"NIS "+std::to_string(restaurant.getTable(tableId)->getOrders()[j].first)+"\n";
		str+="Current Bill: "+std::to_string(restaurant.getTable(tableId)->getBill())+"NIS\n";
	}
	else
		str+="closed\n";
	std::cout<<str;
 	complete();
	
}

std::string PrintTableStatus::toString() const{
	std::string str="status "+std::to_string(tableId);
	if(getStatus()==COMPLETED)
        	str+=" Completed\n";
   	else
        	str+=" Error: " +getErrorMsg()+"\n";	
	return str;
}

BaseAction* PrintTableStatus::clone(){
	return new PrintTableStatus(*this);
}

//-----------------------------------------RestoreResturant----------------------------------------------------
//constructor
RestoreResturant::RestoreResturant():BaseAction(){}

//destructor
RestoreResturant::~RestoreResturant(){}

//copy constructor
RestoreResturant::RestoreResturant(const RestoreResturant &other):BaseAction(other.getErrorMsg(),other.getStatus()){}

//move constructor 
RestoreResturant::RestoreResturant(RestoreResturant&& other):BaseAction(other.getErrorMsg(),other.getStatus()){}

void RestoreResturant::act(Restaurant &restaurant){
	if(backup!= nullptr) {
        restaurant = *backup;
        complete();
    }
    else
        error("No backup available\n");

	
}

std::string RestoreResturant::toString() const{
	if(getStatus()==COMPLETED)
        return "restore Completed\n";
	else
	    return "restore Error: "+getErrorMsg();

}

BaseAction* RestoreResturant::clone(){
	return new RestoreResturant(*this);
}
