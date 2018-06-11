//
//  SyntacticCombination.hpp
//  compilePrinciple
//
//  Created by 秦源 on 2018/6/10.
//  Copyright © 2018年 Abfahrt. All rights reserved.
//

#ifndef SyntacticCombination_hpp
#define SyntacticCombination_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include "tableElemsToSe.pb.h"
#include "signalElemsToSe.pb.h"
using namespace std;
//实现结合第一部分记号流的下推自动机

class LL1Automata{
private:
    bool printError=false;
    //记号流
    signalElems *signalSource;
    string signalSource_name;
    //LL1预测分析表
    tableElems *table1;//整数定义赋值文法
    string table1_name;
    tableElems *table2;//浮点数定义赋值文法
    string table2_name;
public:
    //初始化记号流
    void initSignalStream();
    void showSignalStreamSource();
    //初始化预测分析表
    void initTables();
    
    //进行文法测试
    int getRowIndex(const tableElems &table,char c);
    int getColumnIndex(const tableElems &table,char c);
    bool runPushdownAutomata(const tableElems &table,string &targetString);
    string reflectFunction(const int &t);//映射函数对照表
    bool judgeIntegerGrammer(const string & targetString);//判断整数文法
    bool judgeFloatGrammer(const string & targetString);//判断浮点数文法
    void PushdownAutomata();
    
    
    //构造函数
    LL1Automata();

};

#endif /* SyntacticCombination_hpp */
