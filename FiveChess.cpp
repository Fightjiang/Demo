#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <windows.h>

using namespace std;

const int N=20;                 //20*20的棋盘
const char ChessBoardflag = ' ';          //棋盘标志
const char flag1='o';              //玩家1或电脑的棋子标志
const char flag2='X';              //玩家2的棋子标志

typedef struct Position          //坐标类
{
    int x;                         //代表行
    int y;                         //代表列
}Position;


class SixChess                    //六子棋类
{
public:
    SixChess()                //初始化
    {
        InitChessBoard();
    }


    void Play()               //下棋
    {
        Position Pos1;      // 玩家1或电脑
        Position Pos2;      //玩家2
        int n = 0;
        while (1)
        {
            int mode = ChoiceMode();
            while (1)
            {
                if (mode == 1)       //电脑vs玩家
                {
                    PlayChess(Pos2, 1, flag2);     //玩家下棋
                    if (GetVictory(Pos2, 1, flag2))     //表示玩家
                        break;
                    ComputerChess(Pos1,flag1,Pos2);     // 电脑下棋
                    if (GetVictory(Pos1, 0, flag1) == 1)     //0表示电脑,真表示获胜
                        break;


                }
                else            //玩家1vs玩家2
                {
                    PlayChess(Pos1, 1, flag1);     // 玩家1下棋
                    if (GetVictory(Pos1, 1, flag1))      //1表示玩家1
                        break;

                    PlayChess(Pos2, 2, flag2);     //玩家2下棋
                    if (GetVictory(Pos2, 2, flag2))  //2表示玩家2
                        break;
                }
            }

            cout << "***再来一局***" << endl;
            cout << "y or n :";
            char c = 'y';
            cin >> c;
            if (c == 'n')
                break;
        }
    }

protected:
    int ChoiceMode()           //选择模式
    {
        int i = 0;

        system("cls");        //系统调用，清屏
        InitChessBoard();       //重新初始化棋盘

        cout << "***0、退出  1、电脑vs玩家  2、玩家vs玩家***" << endl;
        while (1)
        {
            cout << "请选择：";
            cin >> i;
            if (i == 0)         //选择0退出
                exit(1);
            if (i == 1 || i == 2)
                return i;
            cout << "输入不合法" << endl;
        }
    }


    void InitChessBoard()      //初始化棋盘
    {
        for (int i = 0; i < N + 1; ++i)
        {
            for (int j = 0; j < N + 1; ++j)
            {
                _ChessBoard[i][j] = ChessBoardflag;
            }
        }
    }



    void PrintChessBoard()    //20 * 20棋盘
    {
        system("cls");                //系统调用，清空屏幕
        for (int i = 0; i < N+1; ++i)
        {
            for (int j = 0; j < N+1; ++j)
            {
                if (i == 0)                               //打印列数字
                {
                    if (j!=0)
                        printf("%3d ", j);
                    else
                        printf("   ");
                }
                else if (j == 0)                //打印行数字
                    printf("%3d", i);
                else
                {
                    if (i < N+1)
                    {
                        printf(" %c |",_ChessBoard[i][j]);
                    }
                }
            }
            cout << endl;
            cout << "   ";
            for (int m = 0; m < N; m++)
            {
                printf("---|");
            }
            cout << endl;
        }
    }

    void PlayChess(Position& pos, int player, int flag)       //玩家下棋
    {
        PrintChessBoard();         //打印棋盘
        while (1)
        {
            printf("玩家%d输入坐标：", player);
            cin >> pos.x >> pos.y;
            if (JudgeValue(pos) == 1)          //坐标合法
                break;
            cout << "坐标不合法，重新输入" << endl;
        }
        _ChessBoard[pos.x][pos.y] = flag;
    }


    void ComputerChess(Position& pos, char flag , Position& pos2)       //电脑下棋
    {

        //确定位置
        //扫描当前棋子
        //对当前棋子进行处理
        //扫描行，上下左右四个方向扫描

        int westEast = 0;	//横向有多少棋子
        int putX = 0, putY = 0;		//要下子的位置
        Position maxDeriction;	//最大方向值
        int MaxValueDeriction = 0;
        //扫描右侧：
        for (int j = pos2.y,empty=0; j <= N && empty <2; j++) {
            if (_ChessBoard[pos2.x][j] == _ChessBoard[pos2.x][pos2.y])
                westEast++;
            else if (_ChessBoard[pos2.x][j] == ChessBoardflag)
            {
                empty++;
                putX = pos2.x;
                putY = j;

            }// 一样的棋子
            else if (_ChessBoard[pos2.x][j] != 0 && _ChessBoard[pos2.x][j] != _ChessBoard[pos2.x][pos2.y]){
                //westEast--;
                break;
            }
            //连续两个空位
            if (empty == 2 && _ChessBoard[pos2.x][j - 1] == ChessBoardflag) {
                putX = pos2.x;
                putY = j - 1;
            }
        }
        //扫描左侧：
        for (int j = pos2.y, empty = 0; j >0 && empty < 2; j--) {
            if (_ChessBoard[pos2.x][j] == _ChessBoard[pos2.x][pos2.y])
                westEast++;
            else if (_ChessBoard[pos2.x][j] == ChessBoardflag){	//空位，可以下子，根据连续棋子数cont给出下子权重
                empty++;
                putX = pos2.x;
                putY = j;
                //_ChessBoard[x][j]=powReturn(westEast);
            }
            // 有自己的棋了
            else if (_ChessBoard[pos2.x][j] != ChessBoardflag && _ChessBoard[pos2.x][j] != _ChessBoard[pos2.x][pos2.y]) {
                //westEast--;
                break;
            }
            //连续两个空位
            if (empty == 2 && _ChessBoard[pos2.x][j + 1] == ChessBoardflag) {
                putX = pos2.x;
                putY = j + 1;
            }
        }

        //最大方向值赋值
        MaxValueDeriction = --westEast;
        maxDeriction.x = putX ;
        maxDeriction.y = putY ;
        //检查横向是否有3个或者4个棋子
        if (westEast >= 3) {		//大于等于3个直接下子
           pos.x = putX ;
           pos.y = putY ;
            _ChessBoard[pos.x][pos.y] = flag;
           return ;
        }
        //上下扫描
        int northSouth = 0;
        //扫描上侧：
        for (int i = pos2.x, empty = 0; i >= 0 && empty < 2; i--) {
            if (_ChessBoard[i][pos2.y] == _ChessBoard[pos2.x][pos2.y])
                northSouth++;
            else if (_ChessBoard[i][pos2.y] == ChessBoardflag)
            {
                empty++;
                putX = i;
                putY = pos2.y;
                //_ChessBoard[i][y] = powReturn(northSouth);
            }
            else if (_ChessBoard[i][pos2.y] != ChessBoardflag && _ChessBoard[i][pos2.y] != _ChessBoard[pos2.x][pos2.y]) {
                //northSouth--;
                break;
            }
            //连续两个空位
            if (empty == 2 && _ChessBoard[i + 1][pos2.y] == ChessBoardflag) {
                putX = i + 1;
                putY = pos2.y;
            }
        }
        //扫描下侧：
        for (int i = pos2.x, empty = 0; i <= N && empty < 2; i++) {
            if (_ChessBoard[i][pos2.y] == _ChessBoard[pos2.x][pos2.y])
                northSouth++;
            else if (_ChessBoard[i][pos2.y] == ChessBoardflag)
            {
                empty++;
                putX = i;
                putY = pos2.y;
                //_ChessBoard[i][y]= powReturn(northSouth);
            }
            else if (_ChessBoard[i][pos2.y] != ChessBoardflag && _ChessBoard[i][pos2.y] != _ChessBoard[pos2.x][pos2.y]) {
                //northSouth--;
                break;
            }
            //连续两个空位
            if (empty == 2 && _ChessBoard[i - 1][pos2.y] == ChessBoardflag) {
                putX = i - 1;
                putY = pos2.y;
            }
        }
        if (--northSouth > MaxValueDeriction) {
            MaxValueDeriction = northSouth;
            maxDeriction.x = putX ;
            maxDeriction.y = putY ;
        }
        //检查纵向一共有多少个棋子
        if (northSouth >= 3) {
            pos.x = putX ;
            pos.y = putY ;
             _ChessBoard[pos.x][pos.y] = flag;
            return ;
        }
        //扫描斜向
        int southEast_northWest = 0;
        //扫描东南
        for (int i = pos2.x, j = pos2.y, empty = 0; i <= N && j <= N && empty < 2; i++, j++) {
            if (_ChessBoard[i][j] == _ChessBoard[pos2.x][pos2.y])
                southEast_northWest++;
            else if (_ChessBoard[i][j] == ChessBoardflag)
            {
                empty++;
                putX = i;
                putY = j;
            }
            else if (_ChessBoard[i][j] != ChessBoardflag && _ChessBoard[i][j] != _ChessBoard[pos2.x][pos2.y]) {
                //southEast_northWest--;
                break;
            }
            //连续两个空位
            if (empty == 2 && _ChessBoard[i - 1][j - 1] == ChessBoardflag) {
                putX = i - 1;
                putY = j - 1;
            }
        }
        //扫描西北
        for (int i = pos2.x, j = pos2.y, empty = 0; i > 0 && j >0 && empty < 2; i--, j--) {
            if (_ChessBoard[i][j] == _ChessBoard[pos2.x][pos2.y])
                southEast_northWest++;
            else if (_ChessBoard[i][j] == ChessBoardflag)
            {
                empty++;
                putX = i;
                putY = j;
                //_ChessBoard[i][j] = powReturn(southEast_northWest);
            }
            else if (_ChessBoard[i][j] != ChessBoardflag && _ChessBoard[i][j] != _ChessBoard[pos2.x][pos2.y]) {
                //southEast_northWest--;
                break;
            }
            //连续两个空位
            if (empty == 2 && _ChessBoard[i + 1][j + 1] == ChessBoardflag) {
                putX = i + 1;
                putY = j + 1;
            }
        }
        if (--southEast_northWest > MaxValueDeriction) {
            MaxValueDeriction = southEast_northWest;
            maxDeriction.x = putX ;
            maxDeriction.y = putY ;

        }
        //检查东南 西北多少个棋子
        if (southEast_northWest >= 3) {
            pos.x = putX ;
            pos.y = putY ;
            _ChessBoard[pos.x][pos.y] = flag;
            return ;
        }
        //扫描东北 西南
        int northEast_southWest = 0;
        //扫描东北
        for (int i = pos2.x, j = pos2.y , empty = 0; i > 0 && j <= N && empty < 2; i--, j++) {
            if (_ChessBoard[i][j] == _ChessBoard[pos2.x][pos2.y])
                northEast_southWest++;
            else if (_ChessBoard[i][j] == ChessBoardflag)
            {
                empty++;
                putX = i;
                putY = j;
                //_ChessBoard[i][j] = powReturn(northEast_southWest);
            }
            else if (_ChessBoard[i][j] != ChessBoardflag && _ChessBoard[i][j] != _ChessBoard[pos2.x][pos2.y]) {
                //northEast_southWest--;
                break;
            }
            //连续两个空位
            if (empty == 2 && _ChessBoard[i + 1][j - 1] == ChessBoardflag) {
                putX = i + 1;
                putY = j - 1;
            }
        }
        //扫描西南
        for (int i = pos2.x, j = pos2.y, empty = 0; i <= N && j >0 && empty < 2; i++, j--) {
            if (_ChessBoard[i][j] == _ChessBoard[pos2.x][pos2.y])
                northEast_southWest++;
            else if (_ChessBoard[i][j] == ChessBoardflag)
            {
                empty++;
                putX = i;
                putY = j;
                //_ChessBoard[i][j] = powReturn(northEast_southWest);
            }
            else if (_ChessBoard[i][j] != ChessBoardflag && _ChessBoard[i][j] != _ChessBoard[pos2.x][pos2.y]) {
                //northEast_southWest--;
                break;
            }
            //连续两个空位
            if (empty == 2 && _ChessBoard[i - 1][j + 1] == ChessBoardflag) {
                putX = i-1;
                putY = j+1;
            }

        }
        if (--northEast_southWest > MaxValueDeriction) {
            MaxValueDeriction = northEast_southWest;
            maxDeriction.x = putX ;
            maxDeriction.y = putY ;
        }
        //检查东北 西南有没有三个以上的棋子位置
        if (northEast_southWest >= 3) {
            pos.x = putX ;
            pos.y = putY ;
             _ChessBoard[pos.x][pos.y] = flag;
            return ;
        }
        cout<<"test1 "<<putX <<" "<<maxDeriction.x<<" "<<putY<<endl ;
        //都没有大于等于3个的连续棋子,找最大的方向值
        if(putX != 0 && putY != 0) {
            pos.x = maxDeriction.x ;
            pos.y = maxDeriction.y ;
            _ChessBoard[pos.x][pos.y] = flag;
        }else {
            //如果初始都为 0 ， 则自己随机下一个地方
            while (1)
            {
                putX = (rand() % N) + 1;      //产生1~N的随机数
                srand((unsigned int) time(NULL));
                putY = (rand() % N) + 1;     //产生1~N的随机数
                srand((unsigned int) time(NULL));
                if (_ChessBoard[putX][putY] == ChessBoardflag)      //如果这个位置是空的，也就是没有棋子
                    break;
            }
            pos.x = putX;
            pos.y = putY;
            _ChessBoard[pos.x][pos.y] = flag;

        }
        return ;

    }


    int JudgeValue(const Position& pos)       //判断输入坐标是不是合法
    {
        if (pos.x > 0 && pos.x <= N&&pos.y > 0 && pos.y <= N)
        {
            if (_ChessBoard[pos.x][pos.y] == ChessBoardflag)
            {
                return 1;    //合法
            }
        }
        return 0;        //非法
    }

     int JudgeVictory(Position pos, char flag)           //判断有没有人胜负(底层判断)
    {
        int begin = 0;
        int end = 0;

        int begin1 = 0;
        int end1 = 0;

        //判断行是否满足条件
        (pos.y - 5) > 0 ? begin = (pos.y - 5) : begin = 1;
        (pos.y + 5) >N ? end = N : end = (pos.y + 5);

        for (int i = pos.x, j = begin; j + 5 <= end; j++)
        {
            if (_ChessBoard[i][j] == flag&&_ChessBoard[i][j + 1] == flag&&
                _ChessBoard[i][j + 2] == flag&&_ChessBoard[i][j + 3] == flag&&
                _ChessBoard[i][j + 4] == flag && _ChessBoard[i][j + 5] == flag)
                return 1;
        }

        //判断列是否满足条件
        (pos.x - 5) > 0 ? begin = (pos.x - 5) : begin = 1;
        (pos.x + 5) > N ? end = N : end = (pos.x + 5);

        for (int j = pos.y, i = begin; i + 5 <= end; i++)
        {
            if (_ChessBoard[i][j] == flag&&_ChessBoard[i + 1][j] == flag&&
                _ChessBoard[i + 2][j] == flag&&_ChessBoard[i + 3][j] == flag&&
                _ChessBoard[i + 4][j] == flag && _ChessBoard[i + 5][j] == flag)
                return 1;
        }

        int len = 0;

        //判断主对角线是否满足条件
        pos.x > pos.y ? len = pos.y - 1 : len = pos.x - 1;
        if (len > 5) len = 5;
        begin = pos.x - len;       //横坐标的起始位置
        begin1 = pos.y - len;      //纵坐标的起始位置

        pos.x > pos.y ? len = (N - pos.x) : len = (N - pos.y);
        if (len>5)   len = 5;
        end = pos.x + len;       //横坐标的结束位置
        end1 = pos.y + len;      //纵坐标的结束位置

        for (int i = begin, j = begin1; (i + 5 <= end) && (j + 5 <= end1); ++i, ++j)
        {
            if (_ChessBoard[i][j] == flag&&_ChessBoard[i + 1][j + 1] == flag&&
                _ChessBoard[i + 2][j + 2] == flag&&_ChessBoard[i + 3][j + 3] == flag&&
                _ChessBoard[i + 4][j + 4] == flag && _ChessBoard[i + 5][j + 5] == flag)
                return 1;
        }


        //判断副对角线是否满足条件
        (pos.x - 1) >(N - pos.y) ? len = (N - pos.y) : len = pos.x - 1;
        if (len > 5) len = 5;
        begin = pos.x - len;       //横坐标的起始位置
        begin1 = pos.y + len;      //纵坐标的起始位置

        (N - pos.x) > (pos.y - 1) ? len = (pos.y - 1) : len = (N - pos.x);
        if (len > 5)   len = 5;
        end = pos.x + len;       //横坐标的结束位置
        end1 = pos.y - len;      //纵坐标的结束位置

        for (int i = begin, j = begin1; (i + 5 <= end) && (j - 5 >= end1); ++i, --j)
        {
            if (_ChessBoard[i][j] == flag&&_ChessBoard[i + 1][j - 1] == flag&&
                _ChessBoard[i + 2][j - 2] == flag&&_ChessBoard[i + 3][j - 3] == flag&&
                _ChessBoard[i + 4][j - 4] == flag&& _ChessBoard[i + 5][j - 5] == flag)
                return 1;
        }


        for (int i = 1; i < N + 1; ++i)           //棋盘有没有下满
        {
            for (int j =1; j < N + 1; ++j)
            {
                if (_ChessBoard[i][j] == ChessBoardflag)
                  return 0;                      //0表示棋盘没满
            }
        }

        return -1;      //和棋
    }

    bool GetVictory(Position& pos, int player, int flag)   //对JudgeVictory的一层封装，得到具体那个玩家获胜
    {
        int n = JudgeVictory(pos, flag);   //判断有没有人获胜
        if (n != 0)                    //有人获胜，0表示没有人获胜
        {
            PrintChessBoard();
            if (n == 1)                //有玩家赢棋
            {
                if (player == 0)     //0表示电脑获胜，1表示玩家1,2表示玩家2
                    printf("***电脑获胜***\n");
                else
                    printf("***恭喜玩家%d获胜***\n", player);
            }
            else
                printf("***双方和棋***\n");

            return true;      //已经有人获胜
        }
        return false;   //没有人获胜
    }
private:
    char _ChessBoard[N+1][N+1];
};

int main (){
    SixChess start ;
    start.Play() ;
    return 0 ;

}