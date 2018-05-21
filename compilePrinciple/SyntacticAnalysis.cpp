//
//  SyntacticAnalysis.cpp
//  compilePrinciple
//
//  Created by 秦源 on 2018/4/22.
//  Copyright © 2018年 Abfahrt. All rights reserved.
//

#include "SyntacticAnalysis.hpp"

//基本信息处理部分
void SyntacticTree::init_Signals(){
    //初始化终结符,非终结符，终结符地址池
    //a:97  A:65
    terminalSignals=new char[terminalSignals_MaxSize];
    unterminalSignals=new char[unterminalSignals_MaxSize];
    unterminalSignalsPool=new char[unterminalSignalsPool_MaxSize];
    for(int i=0;i<26;i++){
        terminalSignals[i]=(char)(i+97);
        unterminalSignals[i]=(char)(i+65);
        unterminalSignalsPool[i]=(char)(i+65);
    }
    if(terminalSignals_add.length()>0)
        for(int i=0;i<terminalSignals_add.length();i++)
            terminalSignals[terminalSignals_size++]=terminalSignals_add[i];
}

void SyntacticTree::inputHandle(string inputStr){
    //TODO::对输入字符串进行处理
}

void SyntacticTree::removeFromUnterminalSignalFromPool(char c){
    bool unterminalSignals_error=true;
    for(int i=0;i<unterminalSignalsPool_MaxSize;i++)
        if(unterminalSignals[i]==c){
            unterminalSignals[i]='#';
            unterminalSignalsPool_size--;
            unterminalSignals_error=false;
            break;
        }
    if(unterminalSignals_error){
        cerr<<"推导式格式存在错误!重复使用非终结符！"<<endl;
        exit(-1);
    }
}

char SyntacticTree::getUnterminalSignalFromPool(){
    if(unterminalSignalsPool_size<1){
        cerr<<"非终结符池为空!"<<endl;
        exit(-1);
    }
    char result = '\0';
    for(int i=0;i<unterminalSignalsPool_MaxSize;i++)
        if(unterminalSignalsPool[i]!='#'){
            result=unterminalSignalsPool[i];
            unterminalSignalsPool[i]='#';;
            break;
        }
    unterminalSignalsPool_size--;
    return result;
}

bool SyntacticTree::isATerminalSignal(char c){
    for(int i=0;i<terminalSignals_size;i++)
        if(terminalSignals[i]==c)
            return true;
    return false;
}

bool SyntacticTree::isAnUnterminalSignal(char c){
    for(int i=0;i<unterminalSignals_size;i++)
        if(unterminalSignals[i]==c)
            return true;
    return false;
}

//消除左递归
void SyntacticTree::ReomveLeftRecursion_single(int index){
    derivative* derivative_aim=&derivativeSet[index];
    char LeftUnterminalSignal=derivative_aim->LeftUnterminalSignal;
    int length=derivative_aim->signalsSetUnit_size;
    stack<string>LeftRecursionUnit;//左递归项
    stack<string>TerminalSignalStartUnit;//终结符开头项
    stack<string>OtherUnit;//其他项，包括空项
    int LeftRecursionUnit_size=0;
    int TerminalSignalStartUnit_size=0;
    for(int i=0;i<length;i++){
        if(isATerminalSignal(derivative_aim->signalsSetUnit[i][0])){
            TerminalSignalStartUnit.push(derivative_aim->signalsSetUnit[i]);
            TerminalSignalStartUnit_size++;
            continue;
        }
        if(derivative_aim->signalsSetUnit[i][0]==LeftUnterminalSignal){
            LeftRecursionUnit.push(derivative_aim->signalsSetUnit[i]);
            LeftRecursionUnit_size++;
            continue;
        }
        OtherUnit.push(derivative_aim->signalsSetUnit[i]);
    }
    //没有递归项
    if(LeftRecursionUnit_size==0)
        return;
    //递归项无法完全消除
    if(LeftRecursionUnit_size>TerminalSignalStartUnit_size){
        cerr<<"非终结符开始项不足，无法消除存在的所有左递归"<<endl;
        exit(-1);
    }
    //消除左递归流程
    char LeftRecursionUnit_duplicate=getUnterminalSignalFromPool();
    derivativeSet[derivativeSet_size].LeftUnterminalSignal=LeftRecursionUnit_duplicate;
    derivativeSet[derivativeSet_size].signalsSetUnit_size=0;
    while (!LeftRecursionUnit.empty()) {
        string a(LeftRecursionUnit.top(),1);//Αα中的α
        string b=TerminalSignalStartUnit.top();//β
        LeftRecursionUnit.pop();
        TerminalSignalStartUnit.pop();
        OtherUnit.push((b+LeftRecursionUnit_duplicate));
        derivativeSet[derivativeSet_size].signalsSetUnit[derivativeSet[derivativeSet_size].signalsSetUnit_size++]=((a+LeftRecursionUnit_duplicate));
    }
    derivativeSet[derivativeSet_size].signalsSetUnit[derivativeSet[derivativeSet_size].signalsSetUnit_size++]=derivative_empty;
    derivativeSet_size++;
    //重构推导式A
    derivative_aim->signalsSetUnit_size=0;
    while (!TerminalSignalStartUnit.empty()) {
        derivative_aim->signalsSetUnit[derivative_aim->signalsSetUnit_size++]=TerminalSignalStartUnit.top();
        TerminalSignalStartUnit.pop();
    }
    while (!OtherUnit.empty()) {
        derivative_aim->signalsSetUnit[derivative_aim->signalsSetUnit_size++]=OtherUnit.top();
        OtherUnit.pop();
    }
}

void SyntacticTree::ReomveLeftRecursion_all(){
    //新增项不存在左递归
    int cycleTimes=derivativeSet_size;
    for(int i=0;i<cycleTimes;i++){
        ReomveLeftRecursion_single(i);
    }
}

//提取左因子
char SyntacticTree::ifHasleftFactor(){
    char a='a';
    return a;
}

void SyntacticTree::leftFactorExtract_single(int index){
    
}

void SyntacticTree::leftFactorExtract_all(){
    
}

//构造函数
SyntacticTree::SyntacticTree(){
    init_Signals();
}

SyntacticTree::SyntacticTree(string RegularExpressionIn){
    //初始化
    SyntacticTree();
    inputHandle(RegularExpressionIn);
    //消除左递归
    ReomveLeftRecursion_all();
    //提取左因子
    leftFactorExtract_all();
}
