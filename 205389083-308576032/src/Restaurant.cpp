#include "Restaurant.h"
#include <iostream>
#include <fstream>


Restaurant::Restaurant():open(false),tables{},menu{},actionsLog{},cCount(0){
}

//constructor
Restaurant::Restaurant(const std::string &configFilePath):open(false),tables{},menu{},actionsLog{},cCount(0){
    cCount=0;
    std::string line;
    size_t comma=0;
    int counter=0;
    int numOfTables = 0;
    int tableCapacity;
    std::string SToInt;
    std::string dName;
    int dPrice;
    std::string dType;
    int dId=0;
    DishType dt = VEG;

    std::ifstream readFile(configFilePath);
    while (std::getline(readFile,line)){

        if (line=="" || line.at(0)=='#')
            continue;
        else{
            counter++;
            if (counter==1){
                numOfTables=std::stoi(line);
            }
            else if (counter==2){
                for (int i=1 ; i<numOfTables ; i++){
                    comma = line.find(",");
                    SToInt=line.substr(0,comma);
                    tableCapacity=std::stoi(SToInt);
                    Table *t =new Table(tableCapacity);
                    tables.push_back(t);
                    line=line.substr(comma+1,line.size()-comma-1);
                }
                tableCapacity=std::stoi(line);
                Table *t=new Table(tableCapacity);
                tables.push_back(t);
            }
            else{
                comma = line.find(",");
                dName = line.substr(0,comma);
                line=line.substr(comma+1,line.size()-comma-1);
                comma = line.find(",");
                dType=line.substr(0,comma);
                SToInt = line.substr(comma+1,line.size()-comma-1);
                dPrice = std::stoi(SToInt);

                if (dType == "VEG")
                    dt=VEG;
                if (dType == "SPC")
                    dt=SPC;
                if (dType == "BVG")
                    dt=BVG;
                if (dType == "ALC")
                    dt=ALC;
                Dish d(dId , dName , dPrice , dt);
                menu.push_back(d);
                dId++;
            }
        }
    }
}

//destructor
Restaurant::~Restaurant(){
    for(unsigned int i=0;i<tables.size();i++)
		delete tables[i];
    tables.clear();
    menu.clear();
    for(unsigned int i=0;i<actionsLog.size();i++)
		delete actionsLog[i];
    actionsLog.clear();
}

//copy constructor
Restaurant::Restaurant(const Restaurant& other):open(other.open),tables{},menu{},actionsLog{},cCount(0){
	for(unsigned int i=0;i<other.menu.size();i++)
		menu.push_back(other.menu[i]);
	for(unsigned int i=0;i<other.tables.size();i++){
		tables.push_back(new Table(*other.tables[i]));
	}
	for(unsigned int i=0;i<other.actionsLog.size();i++){
		actionsLog.push_back(other.actionsLog[i]->clone());
	}
}

//copy assignment operator
Restaurant& Restaurant::operator=(const Restaurant& other){
	if(&other!=this){
		menu.clear();
		for(unsigned int i=0;i<actionsLog.size();i++)
			delete actionsLog[i];
		actionsLog.clear();
		for(unsigned int i=0;i<tables.size();i++)
			    delete tables[i];
		tables.clear();
		for(unsigned int i=0;i<other.menu.size();i++)
			menu.push_back(other.menu[i]);
		for(unsigned int i=0;i<other.tables.size();i++){
			tables.push_back(new Table(*other.tables[i]));
		}
		for(unsigned int i=0;i<other.actionsLog.size();i++){
			actionsLog.push_back(other.actionsLog[i]->clone());
		}
	}
	return *this;
}

//move constructor
Restaurant::Restaurant(Restaurant&& other):open(other.open),tables{},menu{},actionsLog{},cCount(0){
	for(unsigned int i=0;i<other.menu.size();i++)
		menu.push_back(other.menu[i]);
	for(unsigned int i=0;i<other.tables.size();i++){
		Table* toAddT=new Table(*other.tables[i]);
		tables.push_back(toAddT);
	}
	for(unsigned int i=0;i<other.actionsLog.size();i++){
		BaseAction* toAddA=other.actionsLog[i]->clone();
		actionsLog.push_back(toAddA);
	}
	for(unsigned int i=0;i<other.tables.size();i++)
		delete other.tables[i];
    	other.tables.clear();
   	other.menu.clear();
    	for(unsigned int i=0;i<other.actionsLog.size();i++)
			delete other.actionsLog[i];
 	other.actionsLog.clear();
}

//move operator
Restaurant& Restaurant::operator=(Restaurant&& other){
	if(&other!=this){
		menu.clear();
		for(unsigned int i=0;i<actionsLog.size();i++)
			delete actionsLog[i];
    		actionsLog.clear();
		for(unsigned int i=0;i<tables.size();i++)
			delete tables[i];
    		tables.clear();
		for(unsigned int i=0;i<other.menu.size();i++)
			menu.push_back(other.menu[i]);
		for(unsigned int i=0;i<other.tables.size();i++){
			Table* toAddT=new Table(*other.tables[i]);
		tables.push_back(toAddT);
		}
		for(unsigned int i=0;i<other.actionsLog.size();i++){
			BaseAction* toAddA=other.actionsLog[i]->clone();
		actionsLog.push_back(toAddA);
		}
		for(unsigned int i=0;i<other.tables.size();i++)
			delete other.tables[i];
    		other.tables.clear();
   		other.menu.clear();
    		for(unsigned int i=0;i<other.actionsLog.size();i++)
			delete other.actionsLog[i];
 		other.actionsLog.clear();
	}
	return *this;
}

void Restaurant::start(){
    open=true;
    std::cout<< "Restaurant is now open!" <<std::endl;
    while (open==true){
        std::string newAction;
        std::getline(std::cin, newAction);
        std::string actionType;
        size_t space;
        std::string sti;
        space = newAction.find(' ');
        actionType = newAction.substr(0,space);
        //---------------------------open-----------------------------------------
        if (actionType=="open"){
            newAction = newAction.substr(space+1,newAction.size()-space-1);
            space = newAction.find(' ');
            sti = newAction.substr(0,space);
            int t_id = std::stoi(sti);
            newAction = newAction.substr(space+1,newAction.size()-space-1);
            std::vector<Customer *> newCustomers;
            std::string cName;
            std::string cType;
            while (newAction!=""){
                space = newAction.find(' ');
                size_t comma = newAction.find(',');
                if (space!=std::string::npos){
                    cName = newAction.substr(0,comma);
                    cType = newAction.substr(comma+1,space-comma-1);
                }
                else{
                    cName = newAction.substr(0,comma);
                    cType = newAction.substr(comma+1,newAction.size()-comma-1);
                }
                if (cType=="veg"){
                    VegetarianCustomer *vc=new VegetarianCustomer(cName, cCount);
                    newCustomers.push_back(vc);
                    cCount++;
                }
                if (cType=="chp"){
                    CheapCustomer *cc=new CheapCustomer (cName, cCount);
                    newCustomers.push_back(cc);
                    cCount++;
                }
                if (cType=="spc"){
                    SpicyCustomer *sc=new SpicyCustomer(cName, cCount);
                    newCustomers.push_back(sc);
                    cCount++;
                }
                if (cType=="alc"){
                    AlchoholicCustomer *ac=new AlchoholicCustomer(cName, cCount);
                    newCustomers.push_back(ac);
                    cCount++;
                }
                if (space!=std::string::npos)
                    newAction = newAction.substr(space+1,newAction.size()-space-1);
                else
                    newAction="";
            }
            OpenTable *ot=new OpenTable(t_id, newCustomers);
            ot->act(*this);
            actionsLog.push_back(ot);
            //if(ot->getStatus()==ERROR)
            //  delete ot;
        }
        //---------------------------order-----------------------------------------
        if (actionType=="order"){
            newAction = newAction.substr(space+1,newAction.size()-space-1);
            int id = std::stoi(newAction);
            Order *o=new Order(id);
            o->act(*this);
            actionsLog.push_back(o);
        }
        //---------------------------move customer-----------------------------------------
        if (actionType=="move"){
            newAction = newAction.substr(space+1,newAction.size()-space-1);
            int srcT=-1;
            int dstT=-1;
            int idC=-1;
            for (int i=1 ; i<4 ; i++){
                space = newAction.find(' ');
                sti = newAction.substr(0,space);
                if(i==1)
                    srcT = std::stoi(sti);
                if(i==2)
                    dstT = std::stoi(sti);
                if(i==3)
                    idC = std::stoi(sti);
                newAction = newAction.substr(space+1,newAction.size()-space-1);
            }
            MoveCustomer *mc= new MoveCustomer(srcT, dstT, idC);
            mc->act(*this);
            actionsLog.push_back(mc);
        }
        //---------------------------close-----------------------------------------
        if (actionType=="close"){
            newAction = newAction.substr(space+1,newAction.size()-space-1);
            int id = std::stoi(newAction);
            Close *c=new Close(id);
            c->act(*this);
            actionsLog.push_back(c);
        }
        //---------------------------close all-----------------------------------------
        if (actionType=="closeall"){
            CloseAll *ca=new CloseAll();
            ca->act(*this);
            actionsLog.push_back(ca);
            //for(unsigned int i=0;i<actionsLog.size();i++)
           //     delete actionsLog[i];
           /* actionsLog.clear();
            for(unsigned int i=0;i<tables.size();i++)
                if(tables[i]->isOpen())
                    delete tables[i];
            tables.clear();
            menu.clear();*/
        }
        //---------------------------print menu-----------------------------------------
        if (actionType=="menu"){
            PrintMenu *pm=new PrintMenu();
            pm->act(*this);
            actionsLog.push_back(pm);
        }
        //---------------------------Print table status-----------------------------------------
        if (actionType=="status"){
            newAction = newAction.substr(space+1,newAction.size()-space-1);
            int id = std::stoi(newAction);
            PrintTableStatus *pts=new PrintTableStatus(id);
            pts->act(*this);
            actionsLog.push_back(pts);
        }
        //---------------------------Print actions log –-----------------------------------------
        if (actionType=="log"){
            PrintActionsLog *pal=new PrintActionsLog();
            pal->act(*this);
            actionsLog.push_back(pal);
        }
        //---------------------------Backup restaurant–-----------------------------------------
        if (actionType=="backup"){
            BackupRestaurant *br=new BackupRestaurant();
            br->act(*this);
            actionsLog.push_back(br);
        }
        //---------------------------Restore restaurant–-----------------------------------------
        if (actionType=="restore"){
            RestoreResturant *rr=new RestoreResturant();
            rr->act(*this);
            actionsLog.push_back(rr);

        }
    }
}

int Restaurant::getNumOfTables() const{
    return static_cast <int> (tables.size());
}	    

void Restaurant::closeRest(){
	open=false;
}

Table* Restaurant::getTable(int ind){return tables[ind];}

const std::vector<BaseAction*>& Restaurant::getActionsLog() const{
	return actionsLog;
}

std::vector<Dish>& Restaurant::getMenu(){
    return menu;
}

 unsigned int Restaurant::getNumOfActions() const{return actionsLog.size();}

