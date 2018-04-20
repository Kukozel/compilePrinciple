//
//  main.cpp
//  compilePrinciple
//
//  Created by 秦源 on 2018/4/2.
//  Copyright © 2018年 Abfahrt. All rights reserved.
//

#include <iostream>
#include "LexicalAnalysis.hpp"
using namespace std;

int main(int argc, const char * argv[]) {
    
    //传参调用，参数为字符串类型的正规式
    Automata *Target=new Automata("(a|b)*");
    //TODO::后期识别文本时，可增加方法，将最小DFA转化为链表形式
    
    //无参数调用，调用测试函数testFunction1，现实并输出测试NFA
//    Automata *test=new Automata;
//    test->testFunction();

    
}
