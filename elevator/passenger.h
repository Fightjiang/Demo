
#include<iostream>
#include <time.h>
class passenger {
public:
	passenger();
	void set_information(int ID);
	void set_fromfloor(int fromfloor);
	void set_id(int ID) ;
	void set_tofloor(int tofloor);
	int get_fromfloor();
	int get_tofloor();
	char get_ID();
	int Gotime  ; // 乘客乘坐的时间
	int WaitTime ; // 乘客等待时间
private:
	char ID;//编号
	int fromfloor;//所在楼层
	int tofloor; //要去楼层
};

passenger::passenger() {
	ID = 'A';
	fromfloor = 1;
	tofloor = 1;
	Gotime = 0 ; 
	WaitTime = 0 ; 
 }

 void passenger::set_id(int ID) {
    this->ID = 'A' + ID ;
 }

 void passenger::set_information(int ID) {
	this->ID = 'A' + ID;
	std::cout << "这是第" << ID + 1 << "个乘客的信息, " ;
	std::cout << "该乘客目前在哪一层: ";
    while(1) {
        std::cin >> fromfloor;
        if (fromfloor > 9 || fromfloor < 0) {
            std::cout << "\n乘客目前楼层有误，请重新输入:" ;
            continue ;
        }
        break;
    }
	std::cout << "该乘客去哪一层: ";
    while(1) {
        std::cin >> tofloor;
        if (tofloor > 9 || tofloor < 0) {
            std::cout << "\n乘客目前楼层有误，请重新输入:" ;
            continue ;
        }
        break;
    }
    std::cout<<std::endl ;
}

void passenger::set_fromfloor(int fromfloor1) {
	this->fromfloor = fromfloor;
}

void passenger::set_tofloor(int tofloor) {
	this->tofloor = tofloor;
}

int passenger::get_fromfloor() {
	return fromfloor;
}

int passenger::get_tofloor() {
	return tofloor;
}

char passenger::get_ID() {
	return ID;
}

