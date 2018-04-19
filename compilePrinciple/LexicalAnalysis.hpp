//
//  LexicalAnalysis.hpp
//  compilePrinciple
//
//  Created by 秦源 on 2018/4/10.
//  Copyright © 2018年 Abfahrt. All rights reserved.
//

#ifndef LexicalAnalysis_hpp
#define LexicalAnalysis_hpp

#include <stdio.h>
#include <stack>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

/*TODO：
 实现：
 1.正规式转换成NFA，使用Thompson算法。
 2.NFA转换成DFA，使用自己构造法。
 3.最小化DFA，使用子集划分法。
 要求：
 1.每一部分以文件形式输出。
 */

//定义自动机数据结构
class Automata{
    
private:
    //为方便，这里不使用动态分配空间大小，直接分配大小。
    //参数调整统一在这里调整。
    static const int StateMaxSize=128;//状态集最大数量
    static const int CharsMaxSize=64;//字符集最大数量
    static const int EdgeMaxSize=128;//转移函数最大数量
    static const char EmptyShift='#';//设置空转移字符
    static const int EmptyClosureSize=64;//空转移集合最大状态
    
    //打开/关闭部分状态信息显示
    bool showSomeProcess=false;
    
    //定义Edge结构体
    struct Edge{
        int start;//开始状态
        int end;//终止状态
        char condition;//转移条件
    };
    
    //定义状态集
    int S[StateMaxSize];
    int stateNum=0;//存在状态个数
    int stateID=0;//状态开始计数
    
    //定义字符集
    char chars[CharsMaxSize];
    int charNum=0;//存在字符个数
    
    //定义move转移函数
    Edge edges[EdgeMaxSize];
    int edgeNum=0;//存在转移函数个数
    
    //定义初态和终止状态
    int S0,F;
    
    //定义终止状态集(DFA中可能存在多个终止状态)
    int Fs[StateMaxSize];
    int FsNum=0;
    
    //定义正规式,数据栈,符号栈
    string RegularExpression="";
    struct elem{
        int start;//开始状态
        int end;//终止状态
    };
    stack<elem> elements;//数据栈
    stack<char> signals;//符号栈
    
    //定义私有函数
    void writeNowState(string filename,int Fstate);//结果写入文件
    void printNowState(int choose);//结果输出
    
    //TODO：正规式转换NFA
    elem single(char c);//新增节点
    elem Branch(elem a, elem b, char c);//或运算
    elem Join(elem a, elem b,char c);//链接运算
    elem Closures(elem a,char c);//闭包运算
    
    //TODO：NFA转换DFA
    struct emptyClosure{
        int state=-1;   //当前状态
        int e_Closure[EmptyClosureSize];        //当前状态空闭包
        int e_Closure_len=0;                    //当前状态空闭包长度
        bool ifFinal=false;                     //初始默认不是终态
    };
    
    struct DFAelem{
        int newState;//新的状态编号
        int Closure[EmptyClosureSize];//转移到新状态的空转移集合
        int ClosureSize=0;//Closure的大小
    };
    
    //TODO：最小化DFA
    struct MinDFAelem{
        int minDFA_state;//当前状态
        Edge *minDFA_edges;//当前状态具有的转移边
        int minDFA_class;//属于那一类,初始化时非终态为0，终态为1
    };
    
public:
    //构造函数部分
    Automata();//无参构造函数，自动初始化
    Automata(string RegularExpressionIn);//传入正规式
   
    //测试函数部分
    void testFunction1();//测试函数,测试正规式(a|b)*到NFA的转换
    
    //正规式转换DFA部分
    void ScanRegularExpression(string newString);//扫描正规式并转换
    
    //NFA转换DFA部分
    emptyClosure init_emptyClosure_noTransfer(int state);     //非传递闭包运算初始化
    emptyClosure* cal_AllState_emptyClosure();   //全部状态进行非转移直接闭包运算初始化
    void init_emptyClosure_Transfer(emptyClosure * &AllState_emptyClosure);     //传递闭包运算,在全部状态进行非传递直接闭包运算初始化基础上进行
    int ifNewStateExist(DFAelem a,DFAelem b);//判断新规划的状态是否已经存在
    void improveDFAClosure(DFAelem &elem,int sID,emptyClosure* all);//DFA中的Closure包含直接转移后的状态，和这些状态的空闭包两部分
    void mergeNFA(emptyClosure * &AllState_emptyClosure);//DFA合并，消除空闭包
    void NFAtoDFA();//NFA转换DFA流程控制
    
    //最小化DFA部分
    MinDFAelem* init_divideStartOrFinal();//将转移函数划分为初态和终态两部分
    
    void DFAtoMinDFA();//DFA转换MinDFA流程控制
};

#endif /* LexicalAnalysis_hpp */
