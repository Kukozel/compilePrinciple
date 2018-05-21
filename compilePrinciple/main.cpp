//
//  main.cpp
//  compilePrinciple
//
//  Created by 秦源 on 2018/4/2.
//  Copyright © 2018年 Abfahrt. All rights reserved.
//

#include <iostream>
#include "HandleFiles.hpp"
#include "LexicalAnalysis.hpp"
#include "LexicalCombination.hpp"
using namespace std;

int main(int argc, const char * argv[]) {
    //实验1:
    
    //part 1
    //传参调用。参数1为字符串类型的正规式；参数2:是否打印详细结果到屏幕；参数3:是否结果写入文件。
//    Automata *showPart1ForExample=new Automata("(a|b)*cd(e|f)*",true,false);
    //无参数调用，调用测试函数testFunction，现实并输出测试NFA
//        Automata *useTestFunction=new Automata;
//        useTestFunction->testFunction();
    
    //part 2
    //读文件选项：读取sorceText.txt文件
    handleFile *readfile=new handleFile;
    string testString=readfile->readFileIntoString("sorceText.txt");
    //传参调用。参数1为目标文件（含检测的文本）；参数2:是否打印详细结果到屏幕；参数3:是否结果写入文件。
    //其他:记号流分类的正规式过长，应用时不再输出到文件。这里输出文件仅指代记号流的输出。
    SignStream *showPart2ForExample=new SignStream(testString,true,true);

    
    //实验二:
    
    //part 1    未完成
//    SyntacticTree *Target2=new SyntacticTree();
//    string w="abcdef";
//    string c(w,1);
//    cout<<c<<endl;
}
