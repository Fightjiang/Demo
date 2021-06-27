
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
	int Gotime  ; // �˿ͳ�����ʱ��
	int WaitTime ; // �˿͵ȴ�ʱ��
private:
	char ID;//���
	int fromfloor;//����¥��
	int tofloor; //Ҫȥ¥��
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
	std::cout << "���ǵ�" << ID + 1 << "���˿͵���Ϣ, " ;
	std::cout << "�ó˿�Ŀǰ����һ��: ";
    while(1) {
        std::cin >> fromfloor;
        if (fromfloor > 9 || fromfloor < 0) {
            std::cout << "\n�˿�Ŀǰ¥����������������:" ;
            continue ;
        }
        break;
    }
	std::cout << "�ó˿�ȥ��һ��: ";
    while(1) {
        std::cin >> tofloor;
        if (tofloor > 9 || tofloor < 0) {
            std::cout << "\n�˿�Ŀǰ¥����������������:" ;
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

