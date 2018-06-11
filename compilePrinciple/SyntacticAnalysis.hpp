//
//  SyntacticAnalysis.hpp
//  compilePrinciple
//
//  Created by 秦源 on 2018/4/22.
//  Copyright © 2018年 Abfahrt. All rights reserved.
//

#ifndef SyntacticAnalysis_hpp
#define SyntacticAnalysis_hpp

#include <iostream>
#include <stack>
#include <string>
#include <queue>
#include <iostream>
#include <fstream>
#include "tableElemsToSe.pb.h"



using namespace std;

//基本数据情况
class BaseData{

public:
    //显示过程，显示打印，输出打印
    bool printProcess=false;
    bool showDetails=true;
    bool printOut=false;
    //手动设置变量
    static const int terminalSignals_MaxSize=42;//终结符最大数量，默认含有a-z
    string terminalSignals_add=".()*";//补充的终结符集合
    static const int unterminalSignals_MaxSize=42;//非终结符最大数量，默认A-Z
    static const int unterminalSignalsPool_MaxSize=26;//非终结符地址池，默认A-Z
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
    int startSignalIndex;//开始符Index
    
    //定义推导式结构体
    struct derivative{
        char LeftUnterminalSignal;//推导式左侧非终结符
        string signalsSetUnit[signalsSetUnit_MaxSize];//推导式右侧推导式集单元
        int signalsSetUnit_size=0;//推导式右侧推导式集单元个数
    };
    
    derivative derivativeSet[derivativeSet_MaxSize];//推导式集合
    int derivativeSet_size=0;//推导式集合大小
    
    //基本信息处理部分
    void init_Signals();//基本变量初始化
    void inputHandle(string const &inputStr);//输入处理
    void removeFromUnterminalSignalFromPool(char c);//从非终结符池中移除一个非终结符
    char getUnterminalSignalFromPool();//从非终结符池中获取一个非终结符
    bool isATerminalSignal(char c);//判断是否是一个终结符
    bool isAnUnterminalSignal(char c);//判断是否是一个非终结符
    
    //测试函数部分
    void printDerivativeSet();
    
    //构造函数
    BaseData(string& SourceFile);
};

//消除左递归及提取左因子
class DataPreprocessing{
private:
    BaseData *baseData=nullptr;
    string *maxLeftFator=nullptr;
    int maxLeftFator_size=-1;
public:
    //构造函数
    DataPreprocessing(BaseData* baseData);
    
    //消除左递归部分
    void ReomveLeftRecursion_single(const int index);//消除左递归子函数
    void ReomveLeftRecursion_all();//消除左递归总体
    
    //提取左因子部分
    bool FindMaxLeftFator(const int index);//是否含有左因子，没有返回false，有返回true并完成最长左因子
    bool leftFactorExtract_single(int index);//提取左因子子函数
    void leftFactorExtract_all();//提取左因子总体
};

class FirstAndFllow{
private:
    BaseData *baseData=nullptr;
    
    //First集和Fllow集的存储数据结构(注:setElems_size!=0 或 hasNull=true时为收敛状态)
    struct FirstStruct{
        char cID;//列标
        string setElems;
        bool hasNull;//是否有空集
    };
    struct FllowStruct{
        char cID;//列标
        string setElems;
    };
    int F_size=-1;
    struct FllowTrans{
        char src;
        char dst;
    };
    
public:
    //基本数据
    FirstStruct* FirstSet=nullptr;
    FllowStruct* FllowSet=nullptr;
    queue<FllowTrans> TransSet;
    
    //构造函数
    FirstAndFllow(BaseData* baseData);
    
    //计算First集
    void calFirst();
    bool ifFirstSetRein(int index);//根据index判断是否收敛
    string mergeString(string a,string b);//合并两个String
    string mergeString(string a,char b);
    
    //计算Fllow集
    void calFllow();
    string getFirst(const string& aim);
    void initTransSet();
    
    //通用函数
    void initFirstAndFllowSet();
    bool IfTwoStringsHaveSameElem(const string& a,const string& b);//判断两个字符串是否含有相同元素
    int FindIndexByLeftCharInSets(char c);//根据非终结符返回Index
    void printSetState(int id);//0打印First集，1打印Fllow集
};

class LL1Table{
private:
    BaseData *baseData=nullptr;
    FirstAndFllow* firstAndFllow=nullptr;
    const int defaultMaxSize=30;//默认定长数组大小
public:
    //基本信息
    struct TableElem{//分离的推导式单元
        char row_char;
        string cloumn_string;
    };
    TableElem *tableElems;//推导式单元集,大小2倍defaultMaxSize
    int tableElems_size=0;
    char *columns;//列标
    int columns_size;
    char *rows;//行标
    int rows_size;
    int *table_LL1;//LL(1)表
    int table_LL1_size;
    const int null_inLL1=-2;//LL(1)表中为空的项
    char NullCharSignal;//空符号
    //通用函数
    string mergeTerminalSignals(string& src,char dst);//忽略空转移符号
    void initTable();
    void printTable();

    //填表
    int getRowIndex(char c);
    int getColumnIndex(char c);
    void FillIndex(char row,char col,int Index);//在同一位置填两个元素会报错
    string GetFirstToFill(const string& str,bool& haveNull);
    void FillTable();

    //用于序列化
    void initLL1Serialize(string filename);
    
    //构造函数
    LL1Table(BaseData* baseData,FirstAndFllow* firstAndFllow);
};

//下推自动机
class PushdownAutomataLL1{
private:
    string *targetString=nullptr;
    LL1Table *table;
public:
    PushdownAutomataLL1(LL1Table * ll1Table);
    //读取目标字符串
    bool getTargetString(string& target);
    //判断是都符合语法
    bool runPushdownAutomata();
    
    
};


//定义语法分析LL1基本调用
class FundamentalLL1{
private:
    BaseData *baseData;
    FirstAndFllow *firstAndFllow;
    LL1Table *ll1Table;
    PushdownAutomataLL1 * pushdownAutomataLL;
    string *targetString;
    
    void processRawData();//包括读取数据，消除左递归，提取左因子
    void calFirstAndFllow();//求First集和Fllow集
    void calLL1Table();//求LL(1)预测分析表
    void calPushdownAutomataLL1();//LL1下推自动机
    
public:
    //构造函数部分
    FundamentalLL1(string SourceFile);
    void testPushdownAutomataLL1(string tString);
    void LL1TableSe(string filename);
    
};
 
 

#endif /* SyntacticAnalysis_hpp */
