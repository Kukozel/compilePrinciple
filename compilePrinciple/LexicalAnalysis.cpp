//
//  LexicalAnalysis.cpp
//  compilePrinciple
//
//  Created by 秦源 on 2018/4/10.
//  Copyright © 2018年 Abfahrt. All rights reserved.
//

#include "LexicalAnalysis.hpp"

Automata::elem Automata::single(char c){
    //添加两个新的状态
    int s=stateID++;
    int e=stateID++;
    //添加新的转移函数
    int edgeID=edgeNum++;
    edges[edgeID].start=s;
    edges[edgeID].end=e;
    edges[edgeID].condition=c;
    //设置返回参数并返回
    elem newElem;
    newElem.start=s;
    newElem.end=e;
    return newElem;
}

Automata::elem Automata::Branch(elem a, elem b,char c){
    //添加两个新的状态
    int s=stateID++;
    int e=stateID++;
    //添加新的转移函数,四个空转移
    int edgeID=edgeNum++;
    edges[edgeID].start=s;
    edges[edgeID].end=a.start;
    edges[edgeID].condition=c;
    edgeID=edgeNum++;
    edges[edgeID].start=s;
    edges[edgeID].end=b.start;
    edges[edgeID].condition=c;
    edgeID=edgeNum++;
    edges[edgeID].start=a.end;
    edges[edgeID].end=e;
    edges[edgeID].condition=c;
    edgeID=edgeNum++;
    edges[edgeID].start=b.end;
    edges[edgeID].end=e;
    edges[edgeID].condition=c;
    //设置返回参数并返回
    elem newElem;
    newElem.start=s;
    newElem.end=e;
    return newElem;
}

Automata::elem Automata::Join(elem a, elem b,char c){
    //添加新的转移函数,一个空转移
    int edgeID=edgeNum++;
    edges[edgeID].start=a.end;
    edges[edgeID].end=b.start;
    edges[edgeID].condition=c;
    //设置返回参数并返回
    elem newElem;
    newElem.start=a.start;
    newElem.end=b.end;
    return newElem;
}

Automata::elem Automata::Closures(elem a,char c){
    //添加两个新的状态
    int s=stateID++;
    int e=stateID++;
    //添加新的转移函数,四个空转移
    int edgeID=edgeNum++;
    edges[edgeID].start=s;
    edges[edgeID].end=a.start;
    edges[edgeID].condition=c;
    edgeID=edgeNum++;
    edges[edgeID].start=s;
    edges[edgeID].end=e;
    edges[edgeID].condition=c;
    edgeID=edgeNum++;
    edges[edgeID].start=a.end;
    edges[edgeID].end=e;
    edges[edgeID].condition=c;
    edgeID=edgeNum++;
    edges[edgeID].start=a.end;
    edges[edgeID].end=a.start;
    edges[edgeID].condition=c;
    //设置返回参数并返回
    elem newElem;
    newElem.start=s;
    newElem.end=e;
    return newElem;
}

void Automata::writeNowState(string filename){
    cout<<"正规式转化NFA结果，将写入文件："<<filename<<endl<<endl;
    ofstream SaveFile(filename);
    if(!SaveFile.is_open()){
        cout<<"文件打开/创建失败!";
        exit(-1);
    }
    //写入正规式
    SaveFile<<"正规式为:"<<RegularExpression<<endl<<endl;
    //写入字符集情况
    SaveFile<<"字符集个数:"<<charNum<<endl;
    SaveFile<<"字符集内容:"<<endl;
    for(int i=0;i<charNum;i++)
        SaveFile<<chars[i]<<"  ";
    SaveFile<<endl<<endl;
    //写入状态集情况
    SaveFile<<"状态集个数:"<<stateNum<<endl;
    SaveFile<<"状态集内容:"<<endl;
    for(int i=0;i<stateNum;i++)
        SaveFile<<S[i]<<"  ";
    SaveFile<<endl<<endl;
    //写入转移函数情况
    SaveFile<<"转移函数个数:"<<edgeNum<<endl;
    SaveFile<<"转移函数内容:"<<endl;
    for(int i=0;i<edgeNum;i++)
        SaveFile<<"开始状态:"<<edges[i].start<<" 结束状态:"<<edges[i].end<<"  转移条件:"<<edges[i].condition<<endl;
    SaveFile<<endl;
    //写入初态,终态情况
    SaveFile<<"初态:"<<S0<<endl;
    SaveFile<<"终态:"<<F<<endl;
    SaveFile.close();
}

void Automata::ScanRegularExpression(string newString){
    int stringLength=(int)newString.length();
    //非法退出
    if(stringLength<1){
        cout<<"正规式字符串大小为空！"<<endl;
        exit(-1);
    }
    //对字符串中的元素筛查，完成字符集的定义
    for(int i=0;i<stringLength;i++){
        //检查不是运算符号
        if(newString[i]=='(' || newString[i]==')' || newString[i]=='|' ||  newString[i]=='*')
            continue;
        //检查是否已经存在
        bool charExist=false;
        for(int j=0;j<charNum;j++)
            if(chars[j]==newString[i])
                charExist=true;
        //向定义字符集添加新的字符
        if(!charExist)
            chars[charNum++]=newString[i];
                
    }
    //修正正规式：在正规式外侧补充括号
    newString='('+newString+')';
    stringLength+=2;
    //处理正规式
    for(int i=0;i<stringLength;i++){
        if(newString[i]=='('){
            //'('直接入符号栈
            signals.push('(');
        }
        else if(newString[i]==')'){
            //运算上一个'('开始到此为止之间的或运算和连接运算
            //先处理连接运算
            while (signals.top()=='&') {
                if(elements.size()<2){
                    cout<<"正规式存在错误!(数据栈元素少于二元运算需求!)-1"<<endl;
                    exit(-1);
                }
                elem temp1=elements.top();
                elements.pop();
                elem temp2=elements.top();
                elements.pop();
                elements.push(Join(temp1, temp2, EmptyShift));
                signals.pop();
            }
            //再处理或运算
            while (signals.top()=='|') {
                if(elements.size()<2){
                    cout<<"正规式存在错误!(数据栈元素少于二元运算需求!)-2"<<endl;
                    exit(-1);
                }
                elem temp1=elements.top();
                elements.pop();
                elem temp2=elements.top();
                elements.pop();
                elements.push(Branch(temp1, temp2, EmptyShift));
                signals.pop();
            }
            //判定符号闭合
            if(signals.top()!='('){
                cout<<"正规式存在错误!(括号运算闭合失败!)"<<endl;
                exit(-1);
            }else{
               signals.pop();
            }
        }
        else if(newString[i]=='|'){
            //运算上一个'('之前的连接运算，并将'|'压入符号栈
            if(elements.empty()){
                cout<<"正规式存在错误!(或运算时，数据栈为空!)"<<endl;
                exit(-1);
            }
            while (signals.top()=='&') {
                if(elements.size()<2){
                    cout<<"正规式存在错误!(数据栈元素少于二元运算需求!)-3"<<endl;
                    exit(-1);
                }
                elem temp1=elements.top();
                elements.pop();
                elem temp2=elements.top();
                elements.pop();
                elements.push(Join(temp1, temp2, EmptyShift));
                signals.pop();
            }
            signals.push('|');
        }
        else if(newString[i]=='*'){
            //闭包运算优先级最高，直接运算
            if(elements.empty()){
                cout<<"正规式存在错误!(闭包运算时，数据栈为空!)"<<endl;
                exit(-1);
            }
            elem topElem=elements.top();
            elements.pop();
            elements.push(Closures(topElem,EmptyShift));
        }else{
            //不是运算符号时，视为连接，数据直接推入数据栈，并添加运算符号'&'入符号栈
            elements.push(single(newString[i]));
            if(i!=stringLength-1)//不是结尾元素
                if(newString[i+1]!='(' && newString[i+1]!=')' && newString[i+1]!='|' && newString[i+1]!='*')//下一个不是运算符号
                    signals.push('&');
        }
    }
    
    //处理状态集
    stateNum=stateID;
    for(int i=0;i<stateNum;i++)
        S[i]=i;
    
    //处理初态和终态
    S0=elements.top().start;
    F=elements.top().end;
    
    //输出当前NFA状态到文件 "ResultOfNFA_RegularExpression.txt"
    writeNowState("ResultOfNFA_"+RegularExpression+".txt");
}

void Automata::printNowState(){
    //输出正规式
    cout<<"正规式为:"<<RegularExpression<<endl<<endl;
    //输出字符集情况
    cout<<"字符集个数:"<<charNum<<endl;
    cout<<"字符集内容:"<<endl;
    for(int i=0;i<charNum;i++)
        cout<<chars[i]<<"  ";
    cout<<endl<<endl;
    //输出状态集情况
    cout<<"状态集个数:"<<stateNum<<endl;
    cout<<"状态集内容:"<<endl;
    for(int i=0;i<stateNum;i++)
        cout<<S[i]<<"  ";
    cout<<endl<<endl;
    //输出转移函数情况
    cout<<"转移函数个数:"<<edgeNum<<endl;
    cout<<"转移函数内容:"<<endl;
    for(int i=0;i<edgeNum;i++)
        cout<<"开始状态:"<<edges[i].start<<" 结束状态:"<<edges[i].end<<"  转移条件:"<<edges[i].condition<<endl;
    cout<<endl;
    //输出初态,终态情况
    cout<<"初态:"<<S0<<endl;
    cout<<"终态:"<<F<<endl;
}


Automata::Automata(){
    cout<<"已调用Automata无参构造函数..."<<endl;
}

Automata::Automata(string RegularExpression){
    ScanRegularExpression(RegularExpression);
    printNowState();
}

void Automata::testFunction1(){
    cout<<endl<<"--------------------执行测试函数--------------------"<<endl;
    string testString="(a|b)*";
    RegularExpression=testString;
    ScanRegularExpression(testString);
    printNowState();
    cout<<endl<<"--------------------测试函数终止--------------------"<<endl;
}















