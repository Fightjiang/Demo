#include <iostream>
#include <vector>
#include "elevatororign.h"
#include <algorithm>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <ctime>
#include <queue>

using namespace std;

bool cmp1(passenger* A, passenger* B) {
    return A->get_fromfloor() < B->get_fromfloor();
}

bool cmp2(passenger* A, passenger* B) {
    return A->get_fromfloor() > B->get_fromfloor();
}

// 输入乘客信息
void inputPassenger(vector<passenger*>& Up_people, vector<passenger*>& Down_people,
    vector<passenger*> &people , int& UpMaxHigh, int& DownMaxHigh) {
    int PassengerTotal = 0;
    cout << "请输入乘客数量："; cin >> PassengerTotal;  cout << endl;
    for (int i = 0; i < PassengerTotal; ++i) {
        passenger* Tmp = new passenger();
        Tmp->set_information(i);
        people.push_back(Tmp);//每次首先存入乘客信息
    }
    //分流，实现两波人，上去一队，下去一队
    for (int i = 0; i < people.size(); ++i) {
        if (people[i]->get_tofloor() - people[i]->get_fromfloor() > 0) {
            Up_people.push_back(people[i]); // 要坐上升的电梯
            UpMaxHigh = max(UpMaxHigh, people[i]->get_tofloor());
        }
        else {
            DownMaxHigh = max(DownMaxHigh, people[i]->get_fromfloor());
            Down_people.push_back(people[i]); // 要坐下降的电梯
        }
    }

    sort(Up_people.begin(), Up_people.end(), cmp1); // 重载排序，往上走的，按照当前所在楼层低的，按序进入电梯
    sort(Down_people.begin(), Down_people.end(), cmp2); // 重载排序，往下走的，按照当前所在楼层高的，按序进入电梯
    system("cls");
}


//模式1场景2 现在要修改的地方
void Run_elevatorpro(elevator* E, vector<passenger*>& Up_people, vector<passenger*>& Down_people,
     vector<passenger*> &people , int& UpMaxHigh, int& DownMaxHigh)
{
    queue<passenger*> elevator_People ;// 电梯里的乘客开一个队列
    int  Totaltime = 0 ; // 总时间
    for (int i = 1; i <= max(UpMaxHigh, DownMaxHigh); ++i) {
        // 要在当前楼层坐电梯的人，进入电梯内,上升函数
        // 电梯门是否打开 ， 这一楼层一共上了多少人
        int OpenDoorFlag = 0 , Floorpassenger = 0 ;
        for (size_t j = 0; i <= UpMaxHigh && j < Up_people.size(); ++j) {
            if (Up_people[j]->get_fromfloor() == i) {
                if(OpenDoorFlag == 0) {
                    OpenDoorFlag = 1 ; // 电梯门打开了
                    Totaltime = Totaltime + 2 ; // 打开电梯门的时间 2 s
                }
                Up_people[i]->WaitTime = Totaltime ; //上电梯了
                elevator_People.push(Up_people[i]) ;
                Totaltime = Totaltime + 1 ; // 乘客上电梯的时间为 1 s
            }
            else if (Up_people[j]->get_fromfloor() > i) {
                break;
            }
        }

        // 特别注意与第一个要下降的乘客所在楼层高度比较，看电梯是否要继续上去
        for (size_t j = 0; i == DownMaxHigh && DownMaxHigh >= UpMaxHigh && j < Down_people.size(); ++j) {
            if (Down_people[j]->get_fromfloor() == i) {
                if(OpenDoorFlag == 0) {
                    OpenDoorFlag = 1 ; // 电梯门打开了
                    Totaltime = Totaltime + 2 ; // 打开电梯门的时间 2 s
                }
                Down_people[j]->WaitTime = Totaltime ; //上电梯了
                elevator_People.push(Down_people[j]);
                E->set_button(Down_people[j]->get_tofloor(), true);
                Totaltime = Totaltime + 1 ; // 乘客上电梯的时间为 1 s
            }
            else if (Down_people[j]->get_fromfloor() < i) {
                break;
            }
        }
        // 已经到达当前楼层的人，出电梯
        queue<passenger*> Tmp_people ; swap(Tmp_people , elevator_People) ;
        while(!Tmp_people.empty()) {
            passenger* people = Tmp_people.front() ;
            if(people->get_tofloor() == i) {
                people[i].Gotime = Totaltime ;
                Totaltime = Totaltime + 1 ; // 乘客出电梯
            }else {
                elevator_People.push(people) ;
            }
            Tmp_people.pop() ;
        }
        if(OpenDoorFlag == 1) {
            Totaltime = Totaltime + 2 ; // 关电梯门 2 s
            OpenDoorFlag = 0 ;
        }
        if( i != max(UpMaxHigh, DownMaxHigh)) // 如果已经到达最高层了就不用再上升
            Totaltime = Totaltime + 5 ; // 电梯上升一层
    }
    // 电梯开始下降
    E->set_status(2);
    for (int i = E->get_nowFloor() - 1; i >= 1; --i) {
        int OpenDoorFlag = 0 , Floorpassenger = 0 ;
        // 要在当前楼层坐电梯的人，进入电梯内
        for (size_t j = 0; j < Down_people.size(); ++j) {
            if (Down_people[j]->get_fromfloor() == i) {
                if(OpenDoorFlag == 0) {
                    OpenDoorFlag = 1 ; // 电梯门打开了
                    Totaltime = Totaltime + 2 ; // 打开电梯门的时间 2 s
                }
                Down_people[i]->WaitTime = Totaltime ;
                elevator_People.push(Down_people[j]);
                Totaltime = Totaltime + 1 ; // 乘客上电梯的时间为 1 s
            }
            else if (Down_people[j]->get_fromfloor() < i) {
                break;
            }
        }
        // 已经到达当前楼层的人，出电梯
        queue<passenger*> Tmp_people ; swap(Tmp_people , elevator_People) ;
        while(!Tmp_people.empty()) {
            passenger* people = Tmp_people.front() ;
            if(people->get_tofloor() == i) {
                people[i].Gotime = Totaltime ;
                Totaltime = Totaltime + 1 ; // 乘客出电梯
            }else {
                elevator_People.push(people) ;
            }
            Tmp_people.pop() ;
        }
        if(OpenDoorFlag == 1) {
            Totaltime = Totaltime + 2 ; // 关电梯门 2 s
            OpenDoorFlag = 0 ;
        }
        if( i != 1) // 如果已经到达一楼就不用再下降
            Totaltime = Totaltime + 5 ; // 电梯下降一层
    }

}

void outputInfo(vector<passenger*> Total_people) {
    for(size_t i = 0 ; i < Total_people.size() ; ++i) {
        cout<<Total_people[i]->get_ID()<<"从"<<Total_people[i]->get_fromfloor()<<"楼到"<<Total_people[i]->get_tofloor()
        <<"楼 , "<<"乘坐的时间为："<<Total_people[i]->Gotime - Total_people[i]->WaitTime <<"s"
        <<" , 等待时间为："<<Total_people[i]->WaitTime <<"s";
    }
}
int main(){
     // 坐电梯上升、下降乘客信息，
    vector<passenger*> Up_people , Down_people , Total_people ;
    int UpMaxHigh = 0 , DownMaxHigh = 0 ;
    inputPassenger(Up_people , Down_people , Total_people , UpMaxHigh , DownMaxHigh) ;

    elevator *E = new elevator(9) ;
    Run_elevatorpro(E , Up_people , Down_people , Total_people , UpMaxHigh , DownMaxHigh) ;
    outputInfo(Total_people) ;
    return 0 ;
}
