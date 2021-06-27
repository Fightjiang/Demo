#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

int TotalStudent = 0; // 总学生人数
string file ; // 文件目录
struct Student{
	int number ; // 学号
	string name ; // 姓名
    int age ; // 年龄
	string sex ; // 性别
    string brith ; // 生日
    string address ; // 地址
	string tel ; // 电话
    string E_mail ; // 邮箱
}stu[1000];

void Input() {
	system("cls");
    while(true) {
        
        cout<<"请输入文件路径: " ; cin >> file ; 
        ifstream infile(file , ios::in) ;
        if(!infile.is_open()) {
            cout<<"该文件不存在 , 请重新输入"<<endl ; continue ; 
        } 
        int i = 0 ;
        while(!infile.eof()) {
            infile>>stu[i].number>>stu[i].name>>stu[i].age>>stu[i].sex>>stu[i].brith>>stu[i].address>>stu[i].tel>>stu[i].E_mail ; 
            ++i ; 
        }
        TotalStudent = i ; 
        infile.close() ;
        break ; 
    }
}

void LookupForNumber() {
	system("cls");
	cout << endl << "======>>    按学号查找学生信息    <<======" << endl;
	cout << "请输入要查找学生的学号：";
	int tmpNumber; cin >> tmpNumber; getchar() ;
    int i = 0;
	for(i = 0 ; i < TotalStudent ; ++i) {
        if(stu[i].number == tmpNumber) break ; 
    }
    
	if (i == TotalStudent) {
		cout << "======>>    对不起，没有找到该学生......    <<======" << endl;
	} else {
        cout << "------------------------------------------------------------------------------------" << endl;
		cout << "学号"  << "\t" << "姓名"  << "\t" << "年龄" << "\t" 
             << "性别"  << "\t" << "   生日" << "\t\t"  << "地址" << "\t"
			 << "  电话"  << "\t" << "  邮箱" << endl;
		cout << "------------------------------------------------------------------------------------" << endl;
        cout << stu[i].number  << "\t" << stu[i].name  << "\t" << stu[i].age  << "\t"
                << stu[i].sex    << "\t" << stu[i].brith << "\t" << stu[i].address << "\t"
                << stu[i].tel  << "\t" << stu[i].E_mail   << endl;
		cout << "------------------------------------------------------------------------------------" << endl;
	
	}
}

void LookupForName() {
	system("cls");
	cout << endl << "======>>    按姓名查找学生信息    <<======" << endl;
	cout << "请输入要查找学生的姓名：";
	string tmpName; cin >> tmpName; getchar() ;
    int i = 0;
	for(i = 0 ; i < TotalStudent ; ++i) {
        if(stu[i].name == tmpName) break ; 
    }
    
	if (i == TotalStudent) {
		cout << "======>>    对不起，没有找到该学生......    <<======" << endl;
	} else {
        cout << "------------------------------------------------------------------------------------" << endl;
		cout << "学号"  << "\t" << "姓名"  << "\t" << "年龄" << "\t" 
             << "性别"  << "\t" << "   生日" << "\t\t"  << "地址" << "\t"
			 << "  电话"  << "\t" << "  邮箱" << endl;
		cout << "------------------------------------------------------------------------------------" << endl;
        cout << stu[i].number  << "\t" << stu[i].name  << "\t" << stu[i].age  << "\t"
                << stu[i].sex    << "\t" << stu[i].brith << "\t" << stu[i].address << "\t"
                << stu[i].tel  << "\t" << stu[i].E_mail   << endl;
		cout << "------------------------------------------------------------------------------------" << endl;
	
	}
}

void Update() {
    ofstream outfile(file , ios::out) ;
    for(int i = 0 ; i < TotalStudent ; ++i) {
        outfile << stu[i].number  << " " << stu[i].name  << " " << stu[i].age  << " "
                << stu[i].sex     << " " << stu[i].brith << " " << stu[i].address << " "
                << stu[i].tel     << " " << stu[i].E_mail ;
        if(i != TotalStudent - 1) {
            outfile << endl ; 
        }
    }
    outfile.close();
}
 
void Modify() {
	system("cls");	
	cout << endl << "======>>    修改学生信息    <<======" << endl;
	cout << "请输入要修改信息的学生学号：";
	int s; cin >> s; 
    int i = 0;
	while ((stu[i].number - s) != 0 && i < TotalStudent) i++;
	if (i == TotalStudent) {
		cout << "======>>    对不起，无法找到该学生......    <<======" << endl;
	}
	else {
		cout << "------------------------------------------------------------------------------------" << endl;
		cout << "学号"  << "\t" << "姓名"  << "\t" << "年龄" << "\t" 
             << "性别"  << "\t" << "   生日" << "\t\t"  << "地址" << "\t"
			 << "  电话"  << "\t" << "  邮箱" << endl;
		cout << "------------------------------------------------------------------------------------" << endl;
        cout << stu[i].number << "\t" << stu[i].name  << "\t" << stu[i].age  << "\t"
             << stu[i].sex    << "\t" << stu[i].brith << "\t" << stu[i].address << "\t"
             << stu[i].tel    << "\t" << stu[i].E_mail   << endl;
		cout << "------------------------------------------------------------------------------------" << endl;
        while(true) {
            cout << endl << "请重新输入需要修改哪一项信息： " << endl;
            string tmpStr ; cin>>tmpStr ; 
            if(tmpStr == "学号") {
                cout<<"\n请重新输入学号为： " ; 
                cin>>stu[i].number ; 
            }else if(tmpStr == "姓名") {
                cout<<"\n请重新输入姓名为： " ; 
                cin>>stu[i].name ; 
            }else if(tmpStr == "年龄") {
                cout<<"\n请重新输入年龄为： " ; 
                cin>>stu[i].age ; 
            }else if(tmpStr == "性别") {
                cout<<"\n请重新输入性别为： " ; 
                cin>>stu[i].sex ; 
            }else if(tmpStr == "生日") {
                cout<<"\n请重新输入生日为： " ; 
                cin>>stu[i].brith ; 
            }else if(tmpStr == "地址") {
                cout<<"\n请重新输入地址为： " ; 
                cin>>stu[i].address ; 
            }else if(tmpStr == "电话") {
                cout<<"\n请重新输入电话为： " ; 
                cin>>stu[i].tel ; 
            }else if(tmpStr == "邮箱") {
                cout<<"\n请重新输入邮箱为： " ; 
                cin>>stu[i].E_mail ; 
            }else {
                continue ; 
            }
            break; 
        } getchar() ; 
       cout << "======>>    该学生信息已更新，请查看    <<======" << endl;
        cout << "------------------------------------------------------------------------------------" << endl;
		cout << "学号"  << "\t" << "姓名"  << "\t" << "年龄" << "\t" 
             << "性别"  << "\t" << "   生日" << "\t\t"  << "地址" << "\t"
			 << "  电话"  << "\t" << "  邮箱" << endl;
		cout << "------------------------------------------------------------------------------------" << endl;
        cout << stu[i].number << "\t" << stu[i].name  << "\t" << stu[i].age  << "\t"
             << stu[i].sex    << "\t" << stu[i].brith << "\t" << stu[i].address << "\t"
             << stu[i].tel    << "\t" << stu[i].E_mail   << endl;
		cout << "------------------------------------------------------------------------------------" << endl;
	}
    Update() ; 
}
 
void Output() {
	system("cls");
	cout << endl << "======>>    显示全部学生信息     <<======" << endl;
	if (!stu) {
		cout << "没有记录" <<endl ;;
	}
	else {
		cout << "------------------------------------------------------------------------------------" << endl;
		cout << "学号"  << "\t" << "姓名"  << "\t" << "年龄" << "\t" 
             << "性别"  << "\t" << "   生日" << "\t\t"  << "地址" << "\t"
			 << "  电话"  << "\t" << "  邮箱" << endl;
		cout << "------------------------------------------------------------------------------------" << endl;
		for (int i = 0; i < TotalStudent; i++) {
			cout << stu[i].number  << "\t" << stu[i].name     << "\t" << stu[i].age  << "\t"
				 << stu[i].sex     << "\t" << stu[i].brith    << "\t" << stu[i].address << "\t"
				 << stu[i].tel     << "\t" << stu[i].E_mail   << endl;
		}
		cout << "------------------------------------------------------------------------------------" << endl;
	}
}

void del() {
	system("cls");
	cout << endl << "======>>    删除学生信息    <<======" << endl;
	cout << "请输入要删除学生的学号：";
	int tmpNumber; cin >> tmpNumber; getchar() ;
    int i = 0;
	for(i = 0 ; i < TotalStudent ; ++i) {
        if(stu[i].number == tmpNumber) break ; 
    }
    
	if (i == TotalStudent) {
		cout << "======>>    对不起，没有找到该学生......    <<======" << endl;
	} else {
        cout << "======>>    该学生信息已删除    <<======" << endl;
        cout << "------------------------------------------------------------------------------------" << endl;
		cout << "学号"  << "\t" << "姓名"  << "\t" << "年龄" << "\t" 
             << "性别"  << "\t" << "   生日" << "\t\t"  << "地址" << "\t"
			 << "  电话"  << "\t" << "  邮箱" << endl;
		cout << "------------------------------------------------------------------------------------" << endl;
        cout << stu[i].number  << "\t" << stu[i].name  << "\t" << stu[i].age  << "\t"
                << stu[i].sex    << "\t" << stu[i].brith << "\t" << stu[i].address << "\t"
                << stu[i].tel  << "\t" << stu[i].E_mail   << endl;
		cout << "------------------------------------------------------------------------------------" << endl;
        while(i < TotalStudent - 1) {
            stu[i] = stu[i + 1] ; 
            ++i ; 
        }
	}
    Update() ; 
}
int menu() {
	char c;
	do {
		system("cls");// 清屏
		cout << "欢迎使用学生信息管理系统!" << endl;
		cout << " 1 -- 学生信息录入功能                    "  << endl;
        cout << " 2 -- 学生信息浏览功能                    "  << endl;
        cout << " 3 -- 按学号查询学生信息                  "  << endl;
        cout << " 4 -- 按姓名查询学生信息                  "  << endl;
        cout << " 5 -- 修改学生信息                       "  << endl;
        cout << " 6 -- 删除学生信息                       "  << endl;
		cout << " 0 -- 退出                                " << endl;
		cin>>c;  getchar() ; //读入一个字符
	} while (c < '0' || c > '6');

	return (c - '0');
}
 
int main() {
	while(1) {
		switch (menu()) {
			case 1:
				Input() ;
                getchar() ; 
				break;
			case 2:
                Output() ; 
                getchar() ; 
				break;
            case 3:
                LookupForNumber(); 
                getchar() ; 
				break;
            case 4:
                LookupForName();
                getchar() ; 
				break;
            case 5:
                Modify();
				getchar() ; 
				break;
			case 6:
				del();
				getchar() ; 
				break;
			case 0:
				cout << endl << "================感谢您使用学生成绩管理系统==============\n" << endl;
				exit(0);
		}
	}
	return 0;
}