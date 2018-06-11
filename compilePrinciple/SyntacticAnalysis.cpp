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
void BaseData::init_Signals(){
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
    startSignalIndex=0;//默认第一个是开始符
}

void BaseData::inputHandle(string const &inputStr){
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

void BaseData::removeFromUnterminalSignalFromPool(char c){
    for(int i=0;i<unterminalSignals_size;i++)
        if(unterminalSignals[i]==c){
            if(BaseData::printProcess)
                cout<<"移除非终结符:"<<unterminalSignals[i]<<" 非终结符池剩余符号数量:"<<unterminalSignalsPool_size-1<<endl;
            unterminalSignalsPool[i]='#';
            unterminalSignalsPool_size--;
            break;
        }
}

char BaseData::getUnterminalSignalFromPool(){
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

bool BaseData::isATerminalSignal(char c){
    for(int i=0;i<terminalSignals_size;i++)
        if(terminalSignals[i]==c)
            return true;
    return false;
}

bool BaseData::isAnUnterminalSignal(char c){
    for(int i=0;i<unterminalSignals_size;i++)
        if(unterminalSignals[i]==c)
            return true;
    return false;
}

//测试函数部分
void BaseData::printDerivativeSet(){
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
BaseData::BaseData(string& SourceFile){
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
    if(BaseData::printProcess)
        printDerivativeSet();
}


//消除左递归
void DataPreprocessing::ReomveLeftRecursion_single(int index){
    BaseData::derivative* derivative_aim=&baseData->derivativeSet[index];
    char LeftUnterminalSignal=derivative_aim->LeftUnterminalSignal;
    int length=derivative_aim->signalsSetUnit_size;
    stack<string>LeftRecursionUnit;//左递归项
    stack<string>OtherUnit;//其他项(非空项)
    bool HasNullUnit=false;
    //完成LeftRecursionUnit,OtherUnit,HasNullUnit分类
    for (int i=0; i<length; i++) {
        string unit(derivative_aim->signalsSetUnit[i]);
        if (unit[0]==LeftUnterminalSignal){
            if(unit.length()==1){
                cerr<<"A->A形式左递归无法消除!"<<endl;
                exit(-1);
            }
            LeftRecursionUnit.push(unit);
        }
        else if (unit[0]==baseData->derivative_empty)
            HasNullUnit=true;
        else
            OtherUnit.push(unit);
    }
    //没有递归项
    if(LeftRecursionUnit.empty())
        return;
    //递归项无法完全消除
    if(OtherUnit.empty() && HasNullUnit==false){
        cerr<<"左递归无法消除,缺乏平常项!"<<endl;
        exit(-1);
    }
    //消除左递归流程
    char LeftRecursionUnit_duplicate=baseData->getUnterminalSignalFromPool();
    if(baseData->printProcess)
        cout<<"获取到非终结符:"<<LeftRecursionUnit_duplicate<<endl;
    //增加新的推导式
    baseData->derivativeSet[baseData->derivativeSet_size].LeftUnterminalSignal=LeftRecursionUnit_duplicate;
    baseData->derivativeSet[baseData->derivativeSet_size].signalsSetUnit_size=0;
    while (!LeftRecursionUnit.empty()) {
        string a(LeftRecursionUnit.top(),1);//Αα中的α
        baseData->derivativeSet[baseData->derivativeSet_size].signalsSetUnit[baseData->derivativeSet[baseData->derivativeSet_size].signalsSetUnit_size++]=(a+LeftRecursionUnit_duplicate);
        LeftRecursionUnit.pop();
    }
    //添加空转移
    baseData->derivativeSet[baseData->derivativeSet_size].signalsSetUnit[baseData->derivativeSet[baseData->derivativeSet_size].signalsSetUnit_size++]=baseData->derivative_empty;
    baseData->derivativeSet_size++;
    //重构推导式A
    derivative_aim->signalsSetUnit_size=0;
    while (!OtherUnit.empty()) {
        string a(OtherUnit.top());
        derivative_aim->signalsSetUnit[derivative_aim->signalsSetUnit_size++]=(a+LeftRecursionUnit_duplicate);
        OtherUnit.pop();
    }
    if (HasNullUnit)    //空转移存在情况下
        derivative_aim->signalsSetUnit[derivative_aim->signalsSetUnit_size++]=LeftRecursionUnit_duplicate;
}

void DataPreprocessing::ReomveLeftRecursion_all(){
    //新增项不存在左递归
    int cycleTimes=baseData->derivativeSet_size;
    for(int i=0;i<cycleTimes;i++){
        ReomveLeftRecursion_single(i);
    }
}

//提取左因子
int compareTwoString(const string& a,const string& b){
    int start=0;
    while(a[start]!='\0' && a[start]==b[start])
        ++start;
    return start==0?-1:start;
}

bool DataPreprocessing::FindMaxLeftFator(const int index){
    BaseData::derivative* derivative_aim=&baseData->derivativeSet[index];
    int length=derivative_aim->signalsSetUnit_size;
    string *NeedBeFound=new string[length];
    int NeedBeFound_size=0;
    //填充因子集
    for(int i=0;i<length;i++){
        int factSize=0;
        for (int j=0; j<derivative_aim->signalsSetUnit[i].length(); j++) {
            if(baseData->isATerminalSignal(derivative_aim->signalsSetUnit[i][j]))
                ++factSize;
            else
                break;
        }
        if(factSize==0)
            continue;
        NeedBeFound[NeedBeFound_size++]=string(derivative_aim->signalsSetUnit[i],0,factSize);
    }
    if (NeedBeFound_size<2) {
        delete [] NeedBeFound;
        return false;
    }
    //查找最长字符串
    for (int i=0; i<NeedBeFound_size-1; i++) {
        for(int j=i+1;j<NeedBeFound_size;j++){
            int maxSize=compareTwoString(NeedBeFound[i],NeedBeFound[j]);
            if (maxSize>maxLeftFator_size){
                string temp(NeedBeFound[i],0,maxSize);
                maxLeftFator=&temp;
                maxLeftFator_size=maxSize;
            }
        }
    }
    if(maxLeftFator_size>0)
        return true;
    return false;
}

bool DataPreprocessing::leftFactorExtract_single(int index){
    maxLeftFator_size=-1;
    FindMaxLeftFator(index);
    if(maxLeftFator_size<1)
        return false;
    string tempStr(*maxLeftFator);
    BaseData::derivative* derivative_aim=&baseData->derivativeSet[index];
    int length=derivative_aim->signalsSetUnit_size;
    //增加新的推导式
    char LeftRecursionUnit_duplicate=baseData->getUnterminalSignalFromPool();
    if(baseData->printProcess)
        cout<<"获取到非终结符:"<<LeftRecursionUnit_duplicate<<endl;
    baseData->derivativeSet[baseData->derivativeSet_size].LeftUnterminalSignal=LeftRecursionUnit_duplicate;
    baseData->derivativeSet[baseData->derivativeSet_size].signalsSetUnit_size=0;
    //修改原推导式
    stack<string> newItems;
    for (int i=0; i<length; i++) {
        newItems.push(derivative_aim->signalsSetUnit[i]);
    }
    bool hasNullUnit=false;     //为了方便把空集符号放在最后，并且能去掉重复的空集符号
    bool hasSameLenth=false;
    derivative_aim->signalsSetUnit_size=0;
    derivative_aim->signalsSetUnit[derivative_aim->signalsSetUnit_size++]=(tempStr+LeftRecursionUnit_duplicate); //原推导式第一项(合并项)
    while (!newItems.empty()) {
        string a(newItems.top());
        newItems.pop();
        //空转移
        if (a[0]==baseData->derivative_empty) {
            hasNullUnit=true;
            continue;
        }
        //字符串长度不够
        if(a.length()<maxLeftFator_size){
            derivative_aim->signalsSetUnit[derivative_aim->signalsSetUnit_size++]=a;
            continue;
        }
        //比较前缀
        string b(a,0,maxLeftFator_size);
        if(b!=tempStr){
            derivative_aim->signalsSetUnit[derivative_aim->signalsSetUnit_size++]=a;
            continue;
        }
        //前缀相同
        if(a.length()==maxLeftFator_size){
            hasSameLenth=true;
            continue;
        }
        string c(a,maxLeftFator_size);
        baseData->derivativeSet[baseData->derivativeSet_size].signalsSetUnit[baseData->derivativeSet[baseData->derivativeSet_size].signalsSetUnit_size++]=c;
    }
    if (hasNullUnit)    //空转移存在情况下
        derivative_aim->signalsSetUnit[derivative_aim->signalsSetUnit_size++]=baseData->derivative_empty;
    if(hasSameLenth)
        baseData->derivativeSet[baseData->derivativeSet_size].signalsSetUnit[baseData->derivativeSet[baseData->derivativeSet_size].signalsSetUnit_size++]=baseData->derivative_empty;
    baseData->derivativeSet_size++;
    return true;
}

void DataPreprocessing::leftFactorExtract_all(){
    int cycleTimes=baseData->derivativeSet_size;
    for(int i=0;i<cycleTimes;i++){
        bool HadBeenExtract=leftFactorExtract_single(i);
        if(HadBeenExtract)
            --i;
    }
}

//构造函数
DataPreprocessing::DataPreprocessing(BaseData* baseData):baseData(baseData){
    if(baseData==nullptr){
        cerr<<"传入数据指针错误!"<<endl;
        exit(-1);
    }
}


//通用函数
bool FirstAndFllow::IfTwoStringsHaveSameElem(const string& a,const string& b){
    unsigned long aLen=a.length();
    unsigned long bLen=b.length();
    for (int i=0; i<aLen; i++) {
        char aChar=a[i];
        for (int j=0; j<bLen; j++)
            if(aChar==b[j])
                return true;
    }
    return false;
}

void FirstAndFllow::initFirstAndFllowSet(){
    int SetSize=baseData->derivativeSet_size;
    F_size=SetSize;
    FirstSet=new FirstStruct[SetSize];
    FllowSet=new FllowStruct[SetSize];
    for (int i=0; i<SetSize; i++) {
        //First
        FirstSet[i].cID=baseData->derivativeSet[i].LeftUnterminalSignal;
        FirstSet[i].setElems="";
        FirstSet[i].hasNull=false;
        
        //Fllow
        FllowSet[i].cID=baseData->derivativeSet[i].LeftUnterminalSignal;
        FllowSet[i].setElems="";
    }
    if (baseData->printProcess)
        cout<<carveUpLine<<endl<<"FirstFllow初始化完成!"<<endl<<carveUpLine<<endl;
}

int FirstAndFllow::FindIndexByLeftCharInSets(char c){
    if(FirstSet==nullptr || FllowSet==nullptr){
        cerr<<"空指针错误!(@FindIndexByLeftCharInSets Function!)"<<endl;
        exit(-1);
    }
    for (int i=0; i<F_size; i++)
        if(FirstSet[i].cID==c)
            return i;
    cerr<<"查找错误!(@FindIndexByLeftCharInSets Function!)"<<endl;
    exit(-1);
}

void FirstAndFllow::printSetState(int id){
    if(id!=0 && id !=1){
        cerr<<"打印First/Fllow集错误!"<<endl;
        exit(-1);
    }
    if(id==0)
        cout<<"First集状态:"<<endl;
    else
        cout<<"Fllow集状态:"<<endl;
    for (int i=0; i<baseData->derivativeSet_size; i++) {
        if(id==0){
            cout<<FirstSet[i].cID<<":"<<FirstSet[i].setElems;
            if(FirstSet[i].hasNull)
                cout<<baseData->derivative_empty;
            cout<<endl;
        }
        else{
            cout<<FllowSet[i].cID<<":"<<FllowSet[i].setElems;
            cout<<endl;
        }
    }
}

//First集
bool FirstAndFllow::ifFirstSetRein(int index){
    bool rein=false;
    if(FirstSet[index].setElems.length()>0)
        rein=true;
    if (FirstSet[index].hasNull)
        rein=true;
    return rein;
}

string FirstAndFllow::mergeString(string a,string b){
    unsigned long LenA=a.length();
    for (int i=0; i<b.length(); i++) {
        bool hasTheChar=false;
        for (int j=0; j<LenA; j++) {
            if(b[i]==a[j]){
                hasTheChar=true;
                break;
            }
        }
        if(hasTheChar)
            continue;
        a+=b[i];
    }
    return a;
}

string FirstAndFllow::mergeString(string a,char b){
    bool hasTheChar=false;
    for (int i=0; i<a.length(); i++) {
        if(a[i]==b){
            hasTheChar=true;
            break;
        }
    }
    if(!hasTheChar)
        a+=b;
    return a;
}

void FirstAndFllow::calFirst(){
    queue<BaseData::derivative> UnReinSet;
    //第一次遍历寻找直接收敛的非终结符
    for (int i=0; i<baseData->derivativeSet_size; i++) {
        BaseData::derivative temp=baseData->derivativeSet[i];
        //不收敛收入Quene
        bool IfRein=true;
        bool IfHasNullUnion=false;
        string tempString="";
        for(int j=0;j<temp.signalsSetUnit_size;j++){
            char tempChar=temp.signalsSetUnit[j][0];
            if (baseData->isAnUnterminalSignal(tempChar)){//非终结符号开头
                IfRein=false;
                break;
            }
            if(tempChar==baseData->derivative_empty){//存在空转移
                IfHasNullUnion=true;
                continue;
            }
            tempString=mergeString(tempString,tempChar);
        }
        if(!IfRein){
            UnReinSet.push(temp);
            continue;
        }
        //收敛写入数据体
        int Lchar_index=FindIndexByLeftCharInSets(temp.LeftUnterminalSignal);
        FirstSet[Lchar_index].setElems=tempString;
        if(IfHasNullUnion)
            FirstSet[Lchar_index].hasNull=true;
    }
    //循环遍历直至全部收敛
    while(!UnReinSet.empty()){
        BaseData::derivative temp=UnReinSet.front();
        string tempString="";
        bool hasNullUnion=false;
        bool setNotRein=false;
        for (int i=0; i<temp.signalsSetUnit_size; i++) {
            char tempChar=temp.signalsSetUnit[i][0];
            if(baseData->isATerminalSignal(tempChar)){//是终结符
                tempString=mergeString(tempString,tempChar);
                continue;
            }
            if(tempChar==baseData->derivative_empty){
                hasNullUnion=true;
                continue;
            }
            //非终结符开头
            setNotRein=false;
            for(int j=0;j<temp.signalsSetUnit[i].length();j++){
                tempChar=temp.signalsSetUnit[i][j];
                int index=FindIndexByLeftCharInSets(tempChar);
                if(ifFirstSetRein(index) && temp.LeftUnterminalSignal!=tempChar){
                    tempString=mergeString(tempString,FirstSet[index].setElems);
                }else{//不收敛
                    setNotRein=true;
                    break;
                }
                if(FirstSet[index].hasNull){//存在空
                    if(j==temp.signalsSetUnit[i].length()-1){
                        hasNullUnion=true;
                        break;
                    }
                    char t=temp.signalsSetUnit[i][j+1];
                    if(baseData->isATerminalSignal(t)){
                        tempString=mergeString(tempString,t);
                        break;
                    }
                }else
                    break;
            }
            if(setNotRein)
                break;
        }//结束闭合所有单元的循环
        if(setNotRein){
            UnReinSet.pop();
            UnReinSet.push(temp);
        }else{
            int Lchar_index=FindIndexByLeftCharInSets(temp.LeftUnterminalSignal);
            FirstSet[Lchar_index].setElems=tempString;
            if(hasNullUnion)
                FirstSet[Lchar_index].hasNull=true;
            UnReinSet.pop();
        }
    }
    if (baseData->printProcess)
        cout<<carveUpLine<<endl<<"First集计算完成!"<<endl<<carveUpLine<<endl;
}

//Fllow集
string FirstAndFllow::getFirst(const string& aim){
    unsigned long len=aim.length();
    string result="";
    for (int i=0; i<len; i++) {
        char t=aim[i];
        if(baseData->isATerminalSignal(t)){//终结符
            result=mergeString(result, t);
            break;
        }
        int Char_index=FindIndexByLeftCharInSets(t);
        result=mergeString(result,FirstSet[Char_index].setElems);
        if(!FirstSet[Char_index].hasNull)
            break;
    }
    return result;
}

void FirstAndFllow::initTransSet(){
    for (int i=0; i<baseData->derivativeSet_size; i++) {
        BaseData::derivative tempDerivative=baseData->derivativeSet[i];
        char char_src=tempDerivative.LeftUnterminalSignal;
        for (int j=0; j<tempDerivative.signalsSetUnit_size; j++) {
            string tempString(tempDerivative.signalsSetUnit[j]);
            for (int k=(int)tempString.length()-1; k>=0; k--) {
                if (baseData->isAnUnterminalSignal(tempString[k])) {
                    FllowTrans tempFllowTrans;
                    tempFllowTrans.src=char_src;
                    tempFllowTrans.dst=tempString[k];
                    TransSet.push(tempFllowTrans);
                    //判断最后一个是否为含空
                    int lastindex=FindIndexByLeftCharInSets(tempString[k]);
                    if(!FirstSet[lastindex].hasNull)
                        break;
                }else
                    break;
            }
        }
    }
}

void FirstAndFllow::calFllow(){
    //条件1
    FllowSet[baseData->startSignalIndex].setElems=baseData->derivative_carve;
    //条件2
    for(int i=0;i<baseData->derivativeSet_size;i++){
        BaseData::derivative tempDerivative=baseData->derivativeSet[i];
        for(int j=0;j<tempDerivative.signalsSetUnit_size;j++){
            string tempString(tempDerivative.signalsSetUnit[j]);
            for(int k=0;k<tempString.length()-1;k++){
                if(baseData->isAnUnterminalSignal(tempString[k])){//非结尾元素为非终结符
                    string left(tempString,k+1);
                    string s=getFirst(left);
                    int stateIndex=FindIndexByLeftCharInSets(tempString[k]);
                    FllowSet[stateIndex].setElems=mergeString(FllowSet[stateIndex].setElems, s);
                }
            }
        }
    }
    //条件3
    initTransSet();
    for (int i=0; i<TransSet.size(); i++) {
        FllowTrans tempFllowTran=TransSet.front();
        int t1=FindIndexByLeftCharInSets(tempFllowTran.dst);
        int t2=FindIndexByLeftCharInSets(tempFllowTran.src);
        int first=(int)FllowSet[t1].setElems.length();
        FllowSet[t1].setElems=mergeString(FllowSet[t1].setElems, FllowSet[t2].setElems);
        int then=(int)FllowSet[t1].setElems.length();
        if(first!=then)
            i=0;
        TransSet.pop();
        TransSet.push(tempFllowTran);
    }
}

//构造函数
FirstAndFllow::FirstAndFllow(BaseData* baseData):baseData(baseData){
    if(baseData==nullptr){
        cerr<<"传入数据指针错误!"<<endl;
        exit(-1);
    }
}


//LL(1)预测分析表
//通用函数
string LL1Table::mergeTerminalSignals(string& src,char dst){
    if (dst==baseData->derivative_empty)    //忽略空转移符号
        return src;
    bool ifNewTerminalSignal=true;
    for(int i=0;i<src.length();i++)
        if(src[i]==dst){
            ifNewTerminalSignal=false;
            break;
        }
    if(ifNewTerminalSignal)
        return src+dst;
    return src;
}

void LL1Table::initTable(){
    rows_size=baseData->derivativeSet_size;
    rows=new char[rows_size];//行坐标
    string column_string="";
    tableElems=new TableElem[defaultMaxSize*2];//装载
    tableElems_size=0;
    for(int i=0;i<rows_size;i++){
        char L_char=baseData->derivativeSet[i].LeftUnterminalSignal;
        rows[i]=L_char;
        for (int j=0; j<baseData->derivativeSet[i].signalsSetUnit_size; j++) {
            tableElems[tableElems_size].row_char=L_char;
            tableElems[tableElems_size++].cloumn_string=baseData->derivativeSet[i].signalsSetUnit[j];
            for(int k=0;k<baseData->derivativeSet[i].signalsSetUnit[j].length();k++){
                char tk=baseData->derivativeSet[i].signalsSetUnit[j][k];
                if(!baseData->isAnUnterminalSignal(tk))
                    column_string=mergeTerminalSignals(column_string,tk);
            }
        }
    }
    //处理列标
    columns_size=(int)column_string.length()+1;
    columns=new char[columns_size];
    for (int i=0; i<column_string.length(); i++)
        columns[i]=column_string[i];
    columns[column_string.length()]=baseData->derivative_carve;
    //初始化LL1表(-1为初始，意为不填)
    table_LL1_size=rows_size*columns_size;
    table_LL1=new int[table_LL1_size];
    for (int i=0; i<table_LL1_size; i++)
        table_LL1[i]=-1;
    //处理空符号
    NullCharSignal=baseData->derivative_empty;
}

void LL1Table::printTable(){
    cout<<"Index 对照表:"<<endl;
    for (int i=0; i<tableElems_size; i++)
        cout<<tableElems[i].row_char<<">"<<tableElems[i].cloumn_string<<" \tIndex: "<<i<<endl;
    cout<<"空转移 \tIndex:"<<null_inLL1<<endl;
    cout<<"默认不填 \tIndex:-1"<<endl<<endl;
    cout<<"LL(1)预测分析表:"<<endl;
    cout<<" ";
    for (int i=0; i<columns_size; i++)
        cout<<" \t"<<columns[i];
    cout<<endl;
    for(int i=0;i<rows_size;i++){
        cout<<rows[i];
        for (int j=0; j<columns_size; j++)
            cout<<"\t"<<table_LL1[i*columns_size+j];
        cout<<endl;
    }
}

//填表
int LL1Table::getRowIndex(char c){
    for (int i=0; i<rows_size; i++)
        if (rows[i]==c)
            return i;
    cerr<<"无法获取列标Index!"<<endl;
    exit(-1);
}

int LL1Table::getColumnIndex(char c){
    for (int i=0; i<columns_size; i++)
        if (columns[i]==c)
            return i;
    cerr<<"无法获取行标Index!"<<endl;
    exit(-1);
}

void LL1Table::FillIndex(char row,char col,int Index){
    int rowID=getRowIndex(row);
    int colID=getColumnIndex(col);
    int whereID=rowID*columns_size+colID;
    if (table_LL1[whereID]!=-1) {
        cerr<<"错误!同一位置填写了两个Index!非LL(1)文法!"<<endl;
        exit(-1);
    }else
        table_LL1[whereID]=Index;
}

string LL1Table::GetFirstToFill(const string& str,bool& haveNull){
    string result="";
    if(str[0]==baseData->derivative_empty){
        haveNull=true;
        return result;
    }
    unsigned long len=str.length();
    for (int i=0; i<len; i++) {
        char t=str[i];
        if(baseData->isATerminalSignal(t)){//终结符
            result=firstAndFllow->mergeString(result, t);
            haveNull=false;
            break;
        }
        int Char_index=firstAndFllow->FindIndexByLeftCharInSets(t);
        result=firstAndFllow->mergeString(result,firstAndFllow->FirstSet[Char_index].setElems);
        if(!firstAndFllow->FirstSet[Char_index].hasNull){
            haveNull=false;
            break;
        }
        haveNull=true;
    }
    return result;
}

void LL1Table::FillTable(){
    for(int i=0;i<tableElems_size;i++){
        //当是空转移时，往Fllow集合中填
        string tempChar=tableElems[i].cloumn_string;
        bool hasAnNull=false;
        string tempAim=GetFirstToFill(tempChar,hasAnNull);
        if(tempAim.length()>0)
            for (int j=0; j<tempAim.length(); j++)
                FillIndex(tableElems[i].row_char,tempAim[j],i);
        //填写空项
        if(hasAnNull){
            int fllowIndex=firstAndFllow->FindIndexByLeftCharInSets(tableElems[i].row_char);
            string fllowString=firstAndFllow->FllowSet[fllowIndex].setElems;
            for (int j=0; j<fllowString.length(); j++)
                FillIndex(tableElems[i].row_char,fllowString[j],null_inLL1);
        }
    }
}

//构造函数
LL1Table::LL1Table(BaseData* baseData,FirstAndFllow* firstAndFllow):baseData(baseData),firstAndFllow(firstAndFllow){
    if(baseData==nullptr||firstAndFllow==nullptr){
        cerr<<"传入数据指针错误!"<<endl;
        exit(-1);
    }
}


//LL1下推自动机

bool PushdownAutomataLL1::getTargetString(string& target){
    targetString=&target;
    if(targetString==nullptr){
        cerr<<"传入空指针"<<endl;
        return false;
    }
    if(targetString->length()<=0){
        cerr<<"传入空字符串"<<endl;
        return false;
    }
    return true;
}

bool PushdownAutomataLL1::runPushdownAutomata(){
    stack<char> Signal;
    Signal.push(table->rows[0]);
    int Sentence_len=(int)targetString->length();
    int Index=0;
    char *Sentence=new char[Sentence_len+1];
    strcpy(Sentence, targetString->c_str());
    while(!Signal.empty() || Index!=Sentence_len){
        if(Signal.empty() || Index<0 || Index>=Sentence_len){
            if(!Signal.empty() && Index==Sentence_len){
                Sentence[Index]='$';
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
        int rowID=table->getRowIndex(Signal.top());
        int colID=table->getColumnIndex(Sentence[Index]);
        int aimID=table->table_LL1[rowID*table->columns_size+colID];
        if(aimID==-1){
            cerr<<"查表错误!"<<endl;
            return false;
        }
        if(aimID==table->null_inLL1){//推出空
            Signal.pop();
            continue;
        }
        string *tempSignalString=&table->tableElems[aimID].cloumn_string;
        char *tempNewSentence=new char[tempSignalString->length()+1];
        strcpy(tempNewSentence, tempSignalString->c_str());
        if (tempNewSentence[0] == table->NullCharSignal) {
            Signal.pop();
            continue;
        }
        //倒叙进栈
        Signal.pop();
        for(int i=(int)tempSignalString->length()-1;i>=0;--i)
            Signal.push(tempNewSentence[i]);
    }
    cout<<"匹配成功!"<<endl;
    return true;
}


//构造函数
PushdownAutomataLL1::PushdownAutomataLL1(LL1Table * ll1Table):table(ll1Table){
    if(ll1Table==nullptr){
        cerr<<"传入数据指针错误!"<<endl;
        exit(-1);
    }
}


//用于序列化
void LL1Table::initLL1Serialize(string filename){
    tableElems::tableElems result;
    //推导式单元集
    result.set_elems_length(tableElems_size);
    for (int i=0; i<tableElems_size; ++i) {
        tableElems::tableElems::elem *newElem= result.add_elems();
        string temp="";
        temp+=tableElems[i].row_char;
        newElem->set_row_char(temp);
        newElem->set_cloumn_string(tableElems[i].cloumn_string);
    }
    //列标
    result.set_columns_length(columns_size);
    for (int i=0; i<columns_size; ++i){
        string temp="";
        temp+=columns[i];
        result.add_columns(temp);
    }
    
    //行标
    result.set_rows_length(rows_size);
    for (int i=0; i<rows_size; ++i){
        string temp="";
        temp+=rows[i];
        result.add_rows(temp);
    }
    
    //LL(1)表
    result.set_table_ll1_length(table_LL1_size);
    for (int i=0; i<table_LL1_size; ++i)
        result.add_table_ll1(table_LL1[i]);
    //其他
    result.set_null_inll1(null_inLL1);
    result.set_error_inll1(-1);
    string temp="";
    temp+=NullCharSignal;
    result.set_nullcharsignal(temp);
    //序列化到文件
    fstream output(filename,ios::out|ios::trunc|ios::binary);
    if(!result.SerializeToOstream(&output)){
        cerr<<"序列化失败!"<<endl;
        exit(-1);
    }
    output.close();
}

////////////////////////////////////////////////////////////////////////////////////////////
void FundamentalLL1::processRawData(){
    //消除左递归
    DataPreprocessing* dataPreprocessing=new DataPreprocessing(baseData);
    dataPreprocessing->ReomveLeftRecursion_all();
    if(baseData->printProcess){
        cout<<"执行消除左递归ing..."<<endl;
        baseData->printDerivativeSet();
        cout<<carveUpLine<<endl;
    }
    //提取左因子
    dataPreprocessing->leftFactorExtract_all();
    if(baseData->printProcess){
        cout<<"执行提取左因子ing..."<<endl<<carveUpLine<<endl;
        baseData->printDerivativeSet();
        cout<<carveUpLine<<endl;
    }
    if (baseData->showDetails) {
        cout<<"已执行消除左递归,提取左因子!"<<endl;
        baseData->printDerivativeSet();
    }
}

void FundamentalLL1::calFirstAndFllow(){
    firstAndFllow->initFirstAndFllowSet();
    //求First集
    firstAndFllow->calFirst();
    //求Fllow集
    firstAndFllow->calFllow();
    if(baseData->showDetails){
        cout<<"执行First集与Fllow集运算ing..."<<endl<<carveUpLine<<endl;
        firstAndFllow->printSetState(0);
        firstAndFllow->printSetState(1);
    }
}

void FundamentalLL1::calLL1Table(){
    ll1Table->initTable();
    ll1Table->FillTable();
    if (baseData->showDetails) {
        cout<<carveUpLine<<endl<<"完成LL(1)预测分析表!"<<endl<<carveUpLine<<endl;
        ll1Table->printTable();
        cout<<carveUpLine<<endl;
    }
}

void FundamentalLL1::calPushdownAutomataLL1(){
    if(targetString==nullptr || targetString->length()<1){
        cerr<<"字符串存在错误!"<<endl;
        exit(-1);
    }
    pushdownAutomataLL->getTargetString(*targetString);
    pushdownAutomataLL->runPushdownAutomata();
}

void FundamentalLL1::testPushdownAutomataLL1(string tString){
    //LL1下推自动机
    targetString=&tString;
    pushdownAutomataLL=new PushdownAutomataLL1(ll1Table);
    calPushdownAutomataLL1();
}

void FundamentalLL1::LL1TableSe(string filename){
     ll1Table->initLL1Serialize(filename);
}

FundamentalLL1::FundamentalLL1(string SourceFile){
    //初始化基本数据
    baseData=new BaseData(SourceFile);
    //读取数据，消除左递归，提取左因子
    processRawData();
    //计算First集和Fllow集
    firstAndFllow=new FirstAndFllow(baseData);
    calFirstAndFllow();
    //构造LL(1)预测分析表
    ll1Table=new LL1Table(baseData,firstAndFllow);
    calLL1Table();
}





