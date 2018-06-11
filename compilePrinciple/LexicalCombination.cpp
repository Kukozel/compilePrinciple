//
//  LexicalCombination.cpp
//  compilePrinciple
//
//  Created by 秦源 on 2018/5/7.
//  Copyright © 2018年 Abfahrt. All rights reserved.
//

#include "LexicalCombination.hpp"
#include "LexicalAnalysis.hpp"
#include <sstream>
using namespace std;

void SignStream::init_keywordSignal(){
    keywordSignal_number=8;
    keywordSignal=new string[keywordSignal_number];
    keywordSignal[0]="int";
    keywordSignal[1]="char";
    keywordSignal[2]="float";
    keywordSignal[3]="double";
    keywordSignal[4]="void";
    keywordSignal[5]="bool";
    keywordSignal[6]="true";
    keywordSignal[7]="false";
    if(printProcess)
        cout<<"关键字初始化成功! 数量:"<<keywordSignal_number<<endl;
}

void SignStream::init_identifierSignal(string regularFormula){
    Automata *Target=new Automata(regularFormula,false,false);
    identifierSignal=Target->getResultOfMinDFA();
}

void SignStream::init_operateSignal(string regularFormula){
    Automata *Target=new Automata(regularFormula,false,false);
    operateSignal=Target->getResultOfMinDFA();
}

void SignStream::init_integerSignal(string regularFormula){
    Automata *Target=new Automata(regularFormula,false,false);
    integerSignal=Target->getResultOfMinDFA();
}

void SignStream::init_floatSignalSignal(string regularFormula){
    Automata *Target=new Automata(regularFormula,false,false);
    floatSignal=Target->getResultOfMinDFA();
}

void SignStream::init_characterSignal(string regularFormula){
    Automata *Target=new Automata(regularFormula,false,false);
    characterSignal=Target->getResultOfMinDFA();
}

void SignStream::init_characterStringSignal(string regularFormula){
    Automata *Target=new Automata(regularFormula,false,false);
    characterStringSignal=Target->getResultOfMinDFA();
}

void SignStream::init_commonSignals(){
    commonSignals_p_size=26*2;
    commonSignals_p=new string[commonSignals_p_size];
    for(int i=0;i<26;i++){
        commonSignals_p[i]=(char)((int)'A'+i);
        if(printProcess)
            cout<<commonSignals_p[i];
    }
    for(int i=0;i<26;i++){
        commonSignals_p[i+26]=(char)((int)'a'+i);
        if(printProcess)
            cout<<commonSignals_p[i];
    }
    if(printProcess)
        cout<<endl;
}

bool SignStream::isCommonSignals(string s){
    bool result=false;
    for(int i=0;i<commonSignals_p_size;i++)
        if(s==commonSignals_p[i]){
            result=true;
            break;
        }
    return result;
}

//解决符号冲突
void SignStream::solveOperateFormulaCrash(){
    int size=operateSignal->itemsNum;
    for(int i=0;i<size;i++){
        if(operateSignal->items.condition[i]=='a')
            operateSignal->items.condition[i]='*';
        else if (operateSignal->items.condition[i]=='b')
            operateSignal->items.condition[i]='&';
        else if (operateSignal->items.condition[i]=='c')
            operateSignal->items.condition[i]='|';
        else if (operateSignal->items.condition[i]=='d')
            operateSignal->items.condition[i]='(';
        else if (operateSignal->items.condition[i]=='e')
            operateSignal->items.condition[i]=')';
    }
}

//处理目标串部分
void SignStream::init_targetString_trans(){
    targetString_trans=new string;
    *targetString_trans="";
    string q="q";
    for(int i=0;i<(*targetString).length();i++){
        string t(*targetString,i,1);
        if(isCommonSignals(t)){
            *targetString_trans+=q;
        }else{
            *targetString_trans+=t;
        }
    }
    if(printProcess){
        cout<<"转换后的字符串"<<endl;
        cout<<*targetString_trans<<endl;
    }
    
}

void SignStream::init_signalSaver(){
    resultSignalSaver=new signalSaver;
    resultSignalSaver->numbers=0;
    resultSignalSaver->NoIndex=new int[signalSaver_MaxSize];
    resultSignalSaver->NoItem=new string[signalSaver_MaxSize];
}

int SignStream::match_keywordSignal(string s){
    for(int i=0;i<keywordSignal_number;i++)
        if(keywordSignal[i]==s)
            return i;
    return -1;
}

bool SignStream::match_splitSignal(string s){
    if(s==splitSignal)
        return true;
    return false;
}

int SignStream::match_MinDFA_sub(Automata::ReturnMinDFA *characterSignal,int beforeState,char c){
    int nextState=-1;
    for(int i=0;i<characterSignal->itemsNum;i++)
        if(characterSignal->items.start[i]==beforeState)
            if (characterSignal->items.condition[i]==c){
                nextState=characterSignal->items.end[i];
                break;
            }
    return nextState;
}

int SignStream::match_MinDFA(Automata::ReturnMinDFA *characterSignal,string s){
    int beforeState=characterSignal->S0;
    int nowState=characterSignal->S0;
    bool beforeIsFinal=false;
    bool nowIsFinal=false;
    //字符串开始
    for (int i=0; i<s.length(); i++) {
        beforeState=nowState;
        nowState=match_MinDFA_sub(characterSignal,nowState,s[i]);
    }
    //返回1情况
    if(nowState==-1 && s.length()==1)
        return 1;
    
    for(int i=0;i<characterSignal->FinalsNum;i++)
        if(beforeState==characterSignal->Finals[i]){
            beforeIsFinal=true;
            break;
        }
    
    //返回3情况
    if(nowState==-1 && beforeIsFinal)
        return 3;
    //返回4情况
    if(nowState==-1)
        return 4;
    
    for(int i=0;i<characterSignal->FinalsNum;i++)
        if(nowState==characterSignal->Finals[i]){
            nowIsFinal=true;
            break;
        }
    
    //返回2情况
    if(nowIsFinal)
        return 2;
    
    //到结尾符合但不是终结位置返回5
    return 5;
    
}

int SignStream::match_MinDFA_byChars(Automata::ReturnMinDFA *characterSignal,int NoId,int nowIndex){
    int length=0;
    int stateId=1;
    
    for(int i=nowIndex;i<(*targetString).length();i++){
        length++;
        string aString(*targetString_trans,nowIndex,length);
        stateId=match_MinDFA(characterSignal,aString);
        if(stateId!=2 && stateId!=5)
            break;
    }
    
    //开头不符合
    if(stateId==1)
        return -1;

    //到截止都符合
    if(stateId==2){
        string rString(*targetString,nowIndex,length);
         int num=resultSignalSaver->numbers++;
        resultSignalSaver->NoIndex[num]=NoId;
        resultSignalSaver->NoItem[num]=rString;
        if(printProcess)
            cout<<"*"<<rString<<endl;
        return nowIndex+length;
        return -2;
    }
    
    //符合且为终态
    if(stateId==3){
        string rString(*targetString,nowIndex,length-1);
        int num=resultSignalSaver->numbers++;
        resultSignalSaver->NoIndex[num]=NoId;
        resultSignalSaver->NoItem[num]=rString;
        if(printProcess)
            cout<<"**"<<rString<<endl;
        return nowIndex+length-1;
    }
    
    //符合但不为终态
    if(stateId==4 || stateId==5)
        return -1;
    
    return -255;
}

void SignStream::scanner_targetString(){
    int nowIndex=0;//现在索引所指向的位置
    int stateId=-1;
    
    while(nowIndex<(*targetString).length()){
        
        //遇到空格符号跳过
        if((*targetString)[nowIndex]==' '){
            nowIndex++;
            if(nowIndex==(*targetString).length()-1)
                break;
            continue;
        }

        //非空格情况
        //匹配常量
        
        //匹配浮点数
        stateId=match_MinDFA_byChars(floatSignal,No_floatSignal,nowIndex);
        if(stateId==-2)
            break;
        if(stateId>0){
            nowIndex=stateId;
            continue;
        }
        if(stateId==-255){
            cerr<<"字符串越界！"<<endl;
        }
        
        //匹配整数
        stateId=match_MinDFA_byChars(integerSignal,No_integerSignal,nowIndex);
        if(stateId==-2)
            break;
        if(stateId>0){
            nowIndex=stateId;
            continue;
        }
        if(stateId==-255){
            cerr<<"字符串越界！"<<endl;
        }

        //匹配运算符
        stateId=match_MinDFA_byChars(operateSignal,No_operateSignal,nowIndex);
        if(stateId==-2)
            break;
        if(stateId>0){
            nowIndex=stateId;
            continue;
        }
        if(stateId==-255){
            cerr<<"字符串越界！"<<endl;
        }
        
        //匹配字符集
        stateId=match_MinDFA_byChars(characterSignal,No_characterSignal,nowIndex);
        if(stateId==-2)
            break;
        if(stateId>0){
            nowIndex=stateId;
            continue;
        }
        if(stateId==-255){
            cerr<<"字符串越界！"<<endl;
        }
        
        //匹配字符串
        stateId=match_MinDFA_byChars(characterStringSignal,No_characterStringSignal,nowIndex);
        if(stateId==-2)
            break;
        if(stateId>0){
            nowIndex=stateId;
            continue;
        }
        if(stateId==-255){
            cerr<<"字符串越界！"<<endl;
        }
        
        //匹配标识符
        stateId=match_MinDFA_byChars(identifierSignal,No_identifierSignal,nowIndex);
        if(stateId==-2)
            break;
        if(stateId>0){
            nowIndex=stateId;
            continue;
        }
        if(stateId==-255){
            cerr<<"字符串越界！"<<endl;
        }
      
        //判断是否为分隔符
        bool ifSp=false;
        string sp(*targetString,nowIndex,1);
        ifSp=match_splitSignal(sp);
        if(ifSp){
            int num=resultSignalSaver->numbers++;
            resultSignalSaver->NoIndex[num]=No_splitSignal;
            resultSignalSaver->NoItem[num]=sp;
            nowIndex++;
            continue;
        }
        
        cerr<<"error: "<<stateId<<endl;
        cerr<<"error: "<<nowIndex<<endl;
        exit(-1);
    }
}

void SignStream::findKeywordSignal(){
    int number=resultSignalSaver->numbers;
    for(int i=0;i<number;i++){
        if(resultSignalSaver->NoIndex[i]==1)
            if(match_keywordSignal(resultSignalSaver->NoItem[i])>=0)
                resultSignalSaver->NoIndex[i]=0;
    }
}

void SignStream::printOrWriteResult(bool ifPrint,bool ifWrite){
    int number=resultSignalSaver->numbers;
    if(ifPrint){
        //TODO::结果显示
        cout<<"生成记号流个数: "<<number<<endl;
        cout<<"------------------------------------------"<<endl;
        cout<<"记号类别说明:"<<endl;
        cout<<"No.0 代表关键词             No.1 代表标识符"<<endl;
        cout<<"No.2 代表分隔符             No.3 代表运算符"<<endl;
        cout<<"No.4 代表整型数             No.5 代表浮点数"<<endl;
        cout<<"No.6 代表单字符             No.7 代表字符串"<<endl;
        cout<<"------------------------------------------"<<endl;
        cout<<"记号流按顺序产生:"<<endl;
        for(int i=0;i<number;i++){
            cout<<"记号类别 No."<<resultSignalSaver->NoIndex[i];
            cout<<"  内容："<<resultSignalSaver->NoItem[i]<<endl;
        }
        cout<<"------------------------------------------"<<endl;
    }
    if(ifWrite){
        //TODO::结果输出
        stringstream fn;
        fn<<number;
        string filename="ResultOfSignStream_"+fn.str()+".txt";
        
        cout<<"记号流输出结果写入到文件: "<<filename<<endl;
        ofstream SaveFile(filename);
        if(!SaveFile.is_open()){
            cout<<"文件打开/创建失败!";
            exit(-1);
        }
        
        //说明形叙述
        SaveFile<<"记号类别说明:"<<endl;
        SaveFile<<"No.0 代表关键词             No.1 代表标识符"<<endl;
        SaveFile<<"No.2 代表分隔符             No.3 代表运算符"<<endl;
        SaveFile<<"No.4 代表整型数             No.5 代表浮点数"<<endl;
        SaveFile<<"No.6 代表单字符             No.7 代表字符串"<<endl;
        SaveFile<<"------------------------------------------"<<endl;
        SaveFile<<"记号流形式为(类型号,内容). "<<endl;
        SaveFile<<"eg.(0,int) 表示：关键词 int"<<endl;
        SaveFile<<"目标文本为:"<<*targetString<<endl;
        SaveFile<<"生成记号流个数: "<<number<<endl;
        SaveFile<<"------------------------------------------"<<endl;
        SaveFile<<"记号流: "<<endl;
        
        //写入记号流
        for(int i=0;i<number;i++){
            SaveFile<<"("<<resultSignalSaver->NoIndex[i]<<","<<resultSignalSaver->NoItem[i]<<")   ";
        }
        
        //关闭写入文件
        SaveFile.close();
    }
}

void SignStream::init_othersSignal(){
    string identifierOperateFormula="(q|_|@)(q|1|2|3|4|5|6|7|8|9|0|!|_|@)*";
    init_identifierSignal(identifierOperateFormula);
    if(printProcess)
        cout<<"标识符初始化成功! 数量:"<<identifierSignal->itemsNum<<endl;
    
    //* & | ( )五个符号冲突 依次替换为 a b c d e
    string operateFormula="+|++|-|--|=|a|/|%|<|>|b|c|d|e|{|}";
    init_operateSignal(operateFormula);
    solveOperateFormulaCrash();
    if(printProcess)
        cout<<"运算符初始化成功! 数量:"<<operateSignal->itemsNum<<endl;
    
    string integerFormula="(1|2|3|4|5|6|7|8|9|0)*";
    init_integerSignal(integerFormula);
    if(printProcess)
        cout<<"整数常量初始化成功! 数量:"<<integerSignal->itemsNum<<endl;
    
    string floatSignalFormula="(1|2|3|4|5|6|7|8|9|0)(1|2|3|4|5|6|7|8|9|0)*.(1|2|3|4|5|6|7|8|9|0)*";
    init_floatSignalSignal(floatSignalFormula);
    if(printProcess)
        cout<<"浮点数常量初始化成功! 数量:"<<floatSignal->itemsNum<<endl;
    
    string characterFormula="'(1|2|3|4|5|6|7|8|9|0|q| |!)'";
    init_characterSignal(characterFormula);
    if(printProcess)
        cout<<"字符常量初始化成功! 数量:"<<characterSignal->itemsNum<<endl;
    
    string characterStringFormula="'(1|2|3|4|5|6|7|8|9|0| |!|q)*'";
    init_characterStringSignal(characterStringFormula);
    if(printProcess)
        cout<<"字符串常量初始化成功! 数量:"<<characterStringSignal->itemsNum<<endl;
    
    init_commonSignals();
    
    cout<<"----------------------------------------"<<endl;
    cout<<"----------------------------------------"<<endl;
    cout<<"输出记号流准备工作就绪！"<<endl;
    cout<<"----------------------------------------"<<endl;
    cout<<"----------------------------------------"<<endl;
}
//序列化操作
void SignStream::signalsSe(string filename){
    signalElems::signalElems result;
    int number=resultSignalSaver->numbers;
    result.set_elems_length(number);
    for (int i=0; i<number; ++i) {
        result.add_noindex(resultSignalSaver->NoIndex[i]);
        result.add_noitem(resultSignalSaver->NoItem[i]);
    }
    result.add_noindex_explain("关键词");
    result.add_noindex_explain("标识符");
    result.add_noindex_explain("分隔符");
    result.add_noindex_explain("运算符");
    result.add_noindex_explain("整数");
    result.add_noindex_explain("浮点数");
    result.add_noindex_explain("字符");
    result.add_noindex_explain("字符串");
    //序列化到文件
    fstream output(filename,ios::out|ios::trunc|ios::binary);
    if(!result.SerializeToOstream(&output)){
        cerr<<"序列化失败!"<<endl;
        exit(-1);
    }
    output.close();
}

//构造函数
SignStream::SignStream(){
    init_keywordSignal();
    init_othersSignal();
}

SignStream::SignStream(string targetString,bool showDetail,bool writer){
    init_keywordSignal();
    init_othersSignal();
    init_signalSaver();
    this->targetString=&targetString;
    *(this->targetString)+=" ";
    if(this->targetString==NULL){
        cerr<<"传入目标串错误！目标串不能为空！"<<endl;
        exit(-1);
    }
    init_targetString_trans();
    if(showDetail)
        cout<<"传入目标串为： "<<*(this->targetString)<<endl;
    scanner_targetString();
    findKeywordSignal();
    printOrWriteResult(true,true);
}
