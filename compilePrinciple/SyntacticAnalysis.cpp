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
    FllowSet[baseData->startSignalIndex].setElems="$";
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


////////////////////////////////////////////////////////////////////////////////////////////
void SyntacticTree::processRawData(){
    //消除左递归
    DataPreprocessing* dataPreprocessing=new DataPreprocessing(baseData);
    dataPreprocessing->ReomveLeftRecursion_all();
    if(baseData->printProcess){
        cout<<carveUpLine<<endl<<"执行消除左递归ing..."<<endl;
        baseData->printDerivativeSet();
        cout<<carveUpLine<<endl;
    }
    //提取左因子
    dataPreprocessing->leftFactorExtract_all();
    if(baseData->printProcess){
        cout<<carveUpLine<<endl<<"执行提取左因子ing..."<<endl;
        baseData->printDerivativeSet();
        cout<<carveUpLine<<endl;
    }
    if (baseData->showDetails) {
        cout<<carveUpLine<<endl<<"已执行消除左递归,提取左因子!"<<endl;
        baseData->printDerivativeSet();
        cout<<carveUpLine<<endl;
    }
}

void SyntacticTree::calFirstAndFllow(){
    //初始化
    FirstAndFllow *FF=new FirstAndFllow(baseData);
    FF->initFirstAndFllowSet();
    //求First集
    FF->calFirst();
    //求Fllow集
    FF->calFllow();
    if(baseData->showDetails){
        cout<<carveUpLine<<endl<<"执行First集与Fllow集运算ing..."<<endl;
        FF->printSetState(0);
        FF->printSetState(1);
        cout<<carveUpLine<<endl;
    }
}

SyntacticTree::SyntacticTree(string SourceFile){
    //初始化基本数据
    baseData=new BaseData(SourceFile);
    //读取数据，消除左递归，提取左因子
    //processRawData();
    //计算First集和Fllow集
    calFirstAndFllow();
    //构造LL(1)预测分析表
    
}





