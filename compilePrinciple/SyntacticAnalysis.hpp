//
//  SyntacticAnalysis.hpp
//  compilePrinciple
//
//  Created by 秦源 on 2018/4/22.
//  Copyright © 2018年 Abfahrt. All rights reserved.
//

#ifndef SyntacticAnalysis_hpp
#define SyntacticAnalysis_hpp

#include <stdio.h>
#include <stdio.h>
#include <stack>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

/*TODO：
 实现：
 1.消除左递归
 2.提取左因子
 要求：
 1.暂无
 */

//定义语法分析数据结构
class SyntacticTree{
private:
    //手动设置变量
    static const int terminalSignals_MaxSize=42;//终结符最大数量，默认含有a-z
    string terminalSignals_add=".()*";//补充的终结符集合
    static const int unterminalSignals_MaxSize=42;//非终结符最大数量，默认A-Z
    static const int unterminalSignalsPool_MaxSize=26;//终结符地址池，默认A-Z
    static const int signalsSetUnit_MaxSize=10;//推导式右侧推导式集单元最大个数
    static const int derivativeSet_MaxSize=42;//推导式集合最大个数
    static const char derivative_empty='#';//空条件符号
    static const char derivative_join='>';//输入推导式连接符号
    static const char derivative_or='|';//输入推导式或符号
    static const char derivative_carve='$';//输入推导式终止符号
    
    
    //定义终结符,非终结符，终结符地址池，开始符，及相关变量
    char *terminalSignals=NULL;//终结符集
    int terminalSignals_size=26;
    char *unterminalSignals=NULL;//非终结符集
    int unterminalSignals_size=26;
    char *unterminalSignalsPool=NULL;//非终结符池
    int unterminalSignalsPool_size=26;
    char startSignal;//开始符
    
    //定义推导式结构体
    struct derivative{
        char LeftUnterminalSignal;//推导式左侧非终结符
        string signalsSetUnit[signalsSetUnit_MaxSize];//推导式右侧推导式集单元
        int signalsSetUnit_size=0;//推导式右侧推导式集单元个数
    };
    
    derivative derivativeSet[derivativeSet_MaxSize];//推导式集合
    int derivativeSet_size=0;//推导式集合大小
    
public:
    //打开/关闭过程显示
    bool printProcess=true;
    bool showDetails=true;
    
    //构造函数部分
    SyntacticTree();//无参构造函数，自动初始化
    SyntacticTree(string SourceFile);
    
    //测试函数部分
    void testFunction();//测试函数
    void printDerivativeSet();
    
    //基本信息处理部分
    void init_Signals();//基本变量初始化
    void inputHandle(string const &inputStr);//输入处理
    void removeFromUnterminalSignalFromPool(char c);//从非终结符池中移除一个非终结符
    char getUnterminalSignalFromPool();//从非终结符池中获取一个非终结符
    bool isATerminalSignal(char c);//判断是否是一个终结符
    bool isAnUnterminalSignal(char c);//判断是否是一个非终结符
    
    //消除左递归部分
    void ReomveLeftRecursion_single(int index);//消除左递归子函数
    void ReomveLeftRecursion_all();//消除左递归总体
    
    //提取左因子部分
    char ifHasleftFactor();//是否含有左因子，有返回左因子，没有返回空条件符号
    void leftFactorExtract_single(int index);//提取左因子子函数
    void leftFactorExtract_all();//提取左因子总体
};

#endif /* SyntacticAnalysis_hpp */
