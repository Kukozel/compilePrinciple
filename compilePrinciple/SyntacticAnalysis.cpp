//
//  SyntacticAnalysis.cpp
//  compilePrinciple
//
//  Created by 秦源 on 2018/4/22.
//  Copyright © 2018年 Abfahrt. All rights reserved.
//

#include "SyntacticAnalysis.hpp"
#include "HandleFiles.hpp"

//定义显示状态时的分割横线
string carveUpLine="-------------------------------------------";

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

void SyntacticTree::inputHandle(string const &inputStr){
    //对输入字符串进行处理
    int length=(int)inputStr.length();
    for(int i=0;i<length;i++){
        if(inputStr[i]==derivative_carve)
            i++;
        //处理推导式左端
        if(!isAnUnterminalSignal(inputStr[i])){
            cerr<<"推导式错误！ @推导式左端应为非终结符！"<<endl;
            exit(-1);
        }
        derivativeSet[derivativeSet_size].LeftUnterminalSignal=inputStr[i];
        //移除非终结符
        removeFromUnterminalSignalFromPool(inputStr[i++]);
        //判断推导式符号
        if(inputStr[i++]!=derivative_join){
            cerr<<"推导式错误！ @\'>\'"<<endl;
            exit(-1);
        }
        //处理推导式右端
        while(inputStr[i]!=derivative_carve){
            string temp(inputStr,i++,1);
            while(inputStr[i]!=derivative_or && inputStr[i]!=derivative_carve)
                temp+=inputStr[i++];
            derivativeSet[derivativeSet_size].signalsSetUnit[derivativeSet[derivativeSet_size].signalsSetUnit_size++]=temp;
            if(inputStr[i]==derivative_or)
                i++;
        }
        derivativeSet_size++;
    }
}

void SyntacticTree::removeFromUnterminalSignalFromPool(char c){
    for(int i=0;i<unterminalSignals_size;i++)
        if(unterminalSignals[i]==c){
            if(printProcess)
                cout<<"移除非终结符:"<<unterminalSignals[i]<<" 非终结符池剩余符号数量:"<<unterminalSignalsPool_size-1<<endl;
            unterminalSignalsPool[i]='#';
            unterminalSignalsPool_size--;
            break;
        }
}

char SyntacticTree::getUnterminalSignalFromPool(){
    if(unterminalSignalsPool_size<1){
        cerr<<"非终结符池为空!"<<endl;
        exit(-1);
    }
    char result = '\0';
    for(int i=0;i<unterminalSignals_size;i++)
        if(unterminalSignalsPool[i]!='#'){
            result=unterminalSignalsPool[i];
            unterminalSignalsPool[i]='#';
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
    if(printProcess)
        cout<<"获取到非终结符:"<<LeftRecursionUnit_duplicate<<endl;
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
    
    return '\0';
}

void SyntacticTree::leftFactorExtract_single(int index){
    
}

void SyntacticTree::leftFactorExtract_all(){
    
}

//测试函数部分
void SyntacticTree::printDerivativeSet(){
    cout<<carveUpLine<<endl<<"当前推导式集合情况："<<endl;
    cout<<"推导式个数为："<<derivativeSet_size<<endl;
    cout<<"推导式内容为："<<endl;
    for(int i=0;i<derivativeSet_size;i++){
        cout<<derivativeSet[i].LeftUnterminalSignal<<derivative_join;
        int jMax=derivativeSet[i].signalsSetUnit_size;
        for(int j=0;j<jMax;j++){
            cout<<derivativeSet[i].signalsSetUnit[j];
            if(j<jMax-1)
                cout<<derivative_or;
        }
        cout<<endl;
    }
    cout<<carveUpLine<<endl;
}

//构造函数

SyntacticTree::SyntacticTree(string SourceFile){
    //处理传入字符串
    handleFile *readfile=new handleFile;
    string RegularExpressionIn=readfile->readFileIntoString_forSyntactic(SourceFile);
    if(RegularExpressionIn.length()<2){
        cerr<<"传入推导式存在错误!"<<endl;
        exit(-1);
    }
    //初始化
    init_Signals();
    cout<<carveUpLine<<endl<<"传入推导式为:"<<endl<<RegularExpressionIn<<endl<<carveUpLine<<endl;
    inputHandle(RegularExpressionIn);
    if(printProcess)
        printDerivativeSet();
    //消除左递归
    ReomveLeftRecursion_all();
    if(printProcess)
        printDerivativeSet();
    //提取左因子
    leftFactorExtract_all();
}
