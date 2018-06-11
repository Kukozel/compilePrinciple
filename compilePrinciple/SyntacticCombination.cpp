//
//  SyntacticCombination.cpp
//  compilePrinciple
//
//  Created by 秦源 on 2018/6/10.
//  Copyright © 2018年 Abfahrt. All rights reserved.
//

#include "SyntacticCombination.hpp"


void LL1Automata::initSignalStream(){
    //载入记号流文件
    signalSource_name="记号流文件1";
    signalSource=new signalElems();
    fstream input("SignalSource",ios::in|ios::binary);
    if(!signalSource->ParseFromIstream(&input)){
        cerr<<"读取失败!"<<endl;
        exit(-1);
    }
    input.close();
}

void LL1Automata::showSignalStreamSource(){
    int times=signalSource->noindex_explain_size();
    for (int i=0; i<times; ++i) {
        cout<<"记号类别 No."<<i<<"  表示:"<<signalSource->noindex_explain(i)<<endl;
    }
}

void LL1Automata::initTables(){
    //载入预测分析表1
    table1_name="整数声明定义文法";
    table1=new tableElems();
    fstream input1("IntegerGrammer",ios::in|ios::binary);
    if(!table1->ParseFromIstream(&input1)){
        cerr<<"读取失败!"<<endl;
        exit(-1);
    }
    input1.close();
    //载入预测分析表2
    table2_name="浮点数声明定义文法";
    table2=new tableElems();
    fstream input2("FloatGrammer",ios::in|ios::binary);
    if(!table2->ParseFromIstream(&input2)){
        cerr<<"读取失败!"<<endl;
        exit(-1);
    }
    input2.close();
}

//文法测试
int LL1Automata::getRowIndex(const tableElems &table,char c){
    string cStr="";
    cStr+=c;
    for (int i=0; i<table.rows_length(); i++)
        if (table.rows(i)==cStr)
            return i;
    if(printError)
        cerr<<"无法获取列标Index!"<<endl;
    return -9;
}

int LL1Automata::getColumnIndex(const tableElems &table,char c){
    string cStr="";
    cStr+=c;
    for (int i=0; i<table.columns_length(); i++)
        if (table.columns(i)==cStr)
            return i;
    if(printError)
        cerr<<"无法获取行标Index!"<<endl;
    return -9;
}

bool LL1Automata::runPushdownAutomata(const tableElems &table,string &targetString){
    stack<char> Signal;
    string row0String="";
    row0String+=table.rows(0);
    char *row0=new char[2];
    strcpy(row0, row0String.c_str());
    Signal.push(row0[0]);
    int Sentence_len=(int)targetString.length();
    int Index=0;
    char *Sentence=new char[Sentence_len+1];
    strcpy(Sentence, targetString.c_str());
    while(!Signal.empty() || Index!=Sentence_len){
        if(Signal.empty() || Index<0 || Index>=Sentence_len){
            if(!Signal.empty() && Index==Sentence_len){
                char *terminalSignal=new char[2];
                int lastColId=table.columns_length();
                string terminalSignalString=table.columns(lastColId-1);
                strcpy(terminalSignal, terminalSignalString.c_str());
                Sentence[Index]=terminalSignal[0];
            }else{
            cerr<<"发生越界错误!"<<endl;
                return false;
            }
        }
        if(Signal.top()==Sentence[Index]){//match
            Signal.pop();
            ++Index;
            continue;
        }
        int rowID=getRowIndex(table,Signal.top());
        int colID=getColumnIndex(table,Sentence[Index]);
        if(rowID==-9 || colID==-9)
            return false;
        int aimID=table.table_ll1(rowID*table.columns_length()+colID);
        if(aimID==-1){
            cerr<<"查表错误!"<<endl;
            return false;
        }
        if(aimID==table.null_inll1()){//推出空
            Signal.pop();
            continue;
        }
        string tempSignalString=table.elems(aimID).cloumn_string();
        char *tempNewSentence=new char[tempSignalString.length()+1];
        strcpy(tempNewSentence, tempSignalString.c_str());
        string NullCharSignalString="";
        row0String+=table.nullcharsignal();
        char *NullCharSignal=new char[2];
        strcpy(NullCharSignal, NullCharSignalString.c_str());
        if (tempNewSentence[0] == NullCharSignal[0]) {
            Signal.pop();
            continue;
        }
        //倒叙进栈
        Signal.pop();
        for(int i=(int)tempSignalString.length()-1;i>=0;--i)
            Signal.push(tempNewSentence[i]);
    }
    if(printError)
        cout<<"匹配成功!"<<endl;
    return true;
}

string LL1Automata::reflectFunction(const int &t){
    if(t==1)
        return "u";
    if(t==3)
        return "v";
    if(t==4)
        return "w";
    if(t==5)
        return "x";
    if(t==6)
        return "y";
    if(t==7)
        return "z";
    cerr<<"映射失败!"<<endl;
    exit(-1);
}

bool LL1Automata::judgeIntegerGrammer(const string & targetString){
    string t(targetString);
    return runPushdownAutomata(*table1,t);
}

bool LL1Automata::judgeFloatGrammer(const string & targetString){
    string t(targetString);
    return runPushdownAutomata(*table2,t);
}

void LL1Automata::PushdownAutomata(){
    int signal_Length=signalSource->elems_length();
    string originString="";//原句
    string targetString="";//用于LL1判断的
    bool hasNo3Member=false;//存在赋值符号
    bool hasequalMember=false;//存在等号
    for (int i=0; i<signal_Length; ++i) {
        string noStr=signalSource->noitem(i);
        int noId=signalSource->noindex(i);
        //关键词判断
        if(noId==0){
            originString+=noStr+" ";
            targetString+=noStr;
            continue;
        }
        //分隔符判断
        if(noId==2){
            originString+=";";
            //判断类属
            bool hasjudge=false;
            //整数
            if(!hasjudge){
                bool j=judgeIntegerGrammer(targetString);
                if(j==true && hasNo3Member==true)
                    if(hasequalMember==false)
                        hasjudge=false;
                if(j){
                    cout<<originString<<" \t类属:"<<table1_name<<"."<<endl;
                    hasjudge=true;
                }
            }
            //浮点数
            if(!hasjudge){
                bool j=judgeFloatGrammer(targetString);
                if(j==true && hasNo3Member==true)
                    if(hasequalMember==false)
                        hasjudge=false;
                if(j){
                    cout<<originString<<" \t类属:"<<table2_name<<"."<<endl;
                    hasjudge=true;
                }
            }
            //其他
            if(!hasjudge)
                cout<<originString<<" \t类属:其他."<<endl;
            //还原
            originString="";
            targetString="";
            hasNo3Member=false;
            hasequalMember=false;
            continue;
        }
        //普通字符判断
        if(noId==3){
            hasNo3Member=true;
            if(noStr=="=")
                hasequalMember=true;
        }
        originString+=noStr;
        targetString+=reflectFunction(noId);
    }
}


//构造函数
LL1Automata::LL1Automata(){
    initSignalStream();
    initTables();
}
