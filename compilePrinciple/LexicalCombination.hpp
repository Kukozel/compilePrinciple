//
//  LexicalCombination.hpp
//  compilePrinciple
//
//  Created by 秦源 on 2018/5/7.
//  Copyright © 2018年 Abfahrt. All rights reserved.
//

#ifndef LexicalCombination_hpp
#define LexicalCombination_hpp

#include <stdio.h>
#include <string.h>
#include "LexicalAnalysis.hpp"
using namespace std;

class   SignStream{
private:
    //手动参数设置部分
    int signalSaver_MaxSize=128;
    
    //打开/关闭部分状态信息显示
    bool printProcess=false;
    bool showDetails=true;
    
    //记号流部分
    //关键词 No.0
    string *keywordSignal;
    int keywordSignal_number=0;
    
    //标识符 No.1
    const int No_identifierSignal=1;
    Automata::ReturnMinDFA *identifierSignal;
    
    //分隔符 No.2
    const int No_splitSignal=2;
    string splitSignal=";";
    
    //运算符 No.3
    const int No_operateSignal=3;
    Automata::ReturnMinDFA *operateSignal;
    
    //常量:
    //整数 No.4
    const int No_integerSignal=4;
    Automata::ReturnMinDFA *integerSignal;
    
    //浮点数 No.5
    const int No_floatSignal=5;
    Automata::ReturnMinDFA *floatSignal;
    
    //字符 No.6
    const int No_characterSignal=6;
    Automata::ReturnMinDFA *characterSignal;
    
    //字符串 No.7
    const int No_characterStringSignal=7;
    Automata::ReturnMinDFA *characterStringSignal;
    
    //其他
    //目标串
    string *targetString=NULL;
    string *targetString_trans=NULL;//简化后的目标串
    //为解决正规式过长内存不够的情况，引入通配符p(p为字符集）
    string *commonSignals_p;
    int commonSignals_p_size;
    
public:
    //记号流结果存储结构体
    struct signalSaver{
        int numbers;
        int * NoIndex;//0-7
        string * NoItem;
    };
    signalSaver *resultSignalSaver;
    
    //初始化各种记号流
    void init_keywordSignal();
    void init_othersSignal();
    void init_identifierSignal(string regularFormula);
    void init_operateSignal(string regularFormula);
    void solveOperateFormulaCrash(); //解决符号冲突;* & | ( )五个符号冲突 依次替换为 a b c d e;这里再转换回来
    void init_integerSignal(string regularFormula);
    void init_floatSignalSignal(string regularFormula);
    void init_characterSignal(string regularFormula);
    void init_characterStringSignal(string regularFormula);
    void init_commonSignals();
    bool isCommonSignals(string s);
    
    //处理目标串部分
    void init_targetString_trans();
    void init_signalSaver();//初始化记号流结果存储的结构体
    int match_keywordSignal(string s);//匹配返回相应ID，不匹配返回-1
    bool match_splitSignal(string s);//匹配返回true，否则，返回false
    int match_MinDFA_sub(Automata::ReturnMinDFA *characterSignal,int beforeState,char c);//返回下一个状态，不存在则返回-1
    int match_MinDFA(Automata::ReturnMinDFA *characterSignal,string s);//当前字符串首字符不匹配，返回1；当前字符串匹配，结尾终态返回2；当前字符串最后一个不匹配，倒数第二个匹配，且为终态，返回3；当前字符串最后一个不匹配，倒数第二个匹配，且不为终态，返回4；当前字符串匹配，结尾非终态返回5；
    int match_MinDFA_byChars(Automata::ReturnMinDFA *characterSignal,int NoId,int nowIndex);//不符合返回-1；符合无后续返回-2；符合有后续返回nowIndex；错误返回-255
    void scanner_targetString();//扫描目标串，完成记号流
    void findKeywordSignal();//在标识符中寻找关键词，更改其No
    void printOrWriteResult(bool ifPrint,bool ifWrite);//处处结果到屏幕和输出结果到文件
    
    //构造函数
    SignStream();
    SignStream(string targetString,bool showDetail,bool writer);
};

#endif /* LexicalCombination_hpp */
