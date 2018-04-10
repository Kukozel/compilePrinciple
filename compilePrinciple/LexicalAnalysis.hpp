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
    static const int StateMaxSize=512;//状态集最大数量
    static const int CharsMaxSize=64;//字符集最大数量
    static const int EdgeMaxSize=512;//转移函数最大数量
    static const char EmptyShift='#';//设置空转移字符
    
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
    
    //定义正规式,数据栈,符号栈
    string RegularExpression="";
    struct elem{
        int start;//开始状态
        int end;//终止状态
    };
    stack<elem> elements;//数据栈
    stack<char> signals;//符号栈
    
    //定义私有函数
    void writeNowState(string filename);//结果写入文件
    void printNowState();//结果输出
    //TODO：正规式转换NFA
    void ScanRegularExpression(string newString);//扫描正规式
    elem single(char c);//新增节点
    elem Branch(elem a, elem b, char c);//或运算
    elem Join(elem a, elem b,char c);//链接运算
    elem Closures(elem a,char c);//闭包运算
    //TODO：NFA转换DFA
    
    //TODO：最小化DFA
    
public:
    Automata();//无参构造函数，自动初始化
    Automata(string RegularExpression);//传入正规式
    //正规式转换NFA部分
    void testFunction1();//测试函数,测试正规式(a|b)*到NFA的转换
    //NFA转换DFA部分
    
    //最小化DFA部分
    
    
    
};

#endif /* LexicalAnalysis_hpp */
