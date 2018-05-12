//
//  main.cpp
//  compilePrinciple
//
//  Created by 秦源 on 2018/4/2.
//  Copyright © 2018年 Abfahrt. All rights reserved.
//

#include <iostream>
#include "LexicalAnalysis.hpp"
#include "LexicalCombination.hpp"
using namespace std;

int main(int argc, const char * argv[]) {
    //实验1:
    
    //part 1
    //传参调用。参数1为字符串类型的正规式；参数2:是否打印详细结果到屏幕；参数3:是否结果写入文件。
//    Automata *showPart1ForExample=new Automata("(a)(b)",true,false);
    //无参数调用，调用测试函数testFunction，现实并输出测试NFA
//        Automata *useTestFunction=new Automata;
//        useTestFunction->testFunction();
    
    //part 2
    //传参调用。参数1为字符串类型的目标字符串；参数2:是否打印详细结果到屏幕；参数3:是否结果写入文件。
    //其他:记号流分类的正规式过长，应用时不再输出到文件。这里输出文件仅指代记号流的输出。
    string testString=" int a=1.2; float b= 123; char c1='c'; string s1='Hello World'; bool b1=true; int cal=(123%23)+(456*432);";
    SignStream *showPart2ForExample=new SignStream(testString,true,true);

    
    
    
    //实验二:
    
    //part 1    未完成
//    SyntacticTree *Target2=new SyntacticTree();
//    string w="abcdef";
//    string c(w,1);
//    cout<<c<<endl;
}
