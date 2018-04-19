//
//  LexicalAnalysis.cpp
//  compilePrinciple
//
//  Created by 秦源 on 2018/4/10.
//  Copyright © 2018年 Abfahrt. All rights reserved.
//

#include "LexicalAnalysis.hpp"

//正规式转NFA部分：

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

void Automata::writeNowState(string filename,int Fstate){
    if(Fstate==1)
        cout<<"正规式转化NFA结果将写入文件："<<filename<<endl<<endl;
    if(Fstate==2)
        cout<<"NFA转化DFA结果将写入文件："<<filename<<endl<<endl;
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
    if(Fstate==1)
        SaveFile<<"终态:"<<F<<endl;
    if(Fstate==2){
        SaveFile<<"终态集:";
        for(int q=0;q<FsNum;q++)
            SaveFile<<Fs[q]<<"  ";
        SaveFile<<endl;
    }
    SaveFile.close();
}

void Automata::ScanRegularExpression(string newString){
    int stringLength=(int)newString.length();
    //非法退出
    if(stringLength<1){
        cout<<"正规式字符串大小为空！"<<endl;
        exit(-1);
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
                elements.push(Join(temp2, temp1, EmptyShift));
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
                elements.push(Join(temp2, temp1, EmptyShift));
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
            //改错:闭包运算后需压栈'&'
            if(i!=stringLength-1)//不是结尾元素
                if(newString[i+1]!='(' && newString[i+1]!=')' && newString[i+1]!='|' && newString[i+1]!='*')//下一个不是运算符号
                    signals.push('&');
        }else{
            //不是运算符号时，视为连接，数据直接推入数据栈，并添加运算符号'&'入符号栈
            elements.push(single(newString[i]));
            if(i!=stringLength-1)//不是结尾元素
                if(newString[i+1]!='(' && newString[i+1]!=')' && newString[i+1]!='|' && newString[i+1]!='*')//下一个不是运算符号
                    signals.push('&');
            //检查定义字符集是否包含
            bool charExist=false;
            for(int j=0;j<charNum;j++)
                if(chars[j]==newString[i])
                    charExist=true;
            //向定义字符集添加新的字符
            if(!charExist)
                chars[charNum++]=newString[i];
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
    writeNowState("ResultOfNFA_"+RegularExpression+".txt",1);
}

void Automata::printNowState(int choose){
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
    //choose=1时输出终态；choose=2时输出终态集。
    if(choose==1)
        cout<<"终态:"<<F<<endl;
    if(choose==2){
        cout<<"终态集:";
        for(int p=0;p<FsNum;p++)
            cout<<Fs[p]<<"  ";
        cout<<endl;
    }
    cout<<"-----------------------------------------"<<endl;
}

//NFA转DFA部分：

Automata::emptyClosure Automata::init_emptyClosure_noTransfer(int state){
    emptyClosure eC;
    //确定当前状态
    eC.state=state;
    //空包运算
    for(int i=0;i<edgeNum;i++){
        if(edges[i].start != state)
            continue;
        if(edges[i].condition!=EmptyShift)
            continue;
        bool ifHasThisEnd=false;
        for(int j=0;j<eC.e_Closure_len;j++)
            if(eC.e_Closure[j]==edges[i].end)
                ifHasThisEnd=true;
        if(!ifHasThisEnd)
            eC.e_Closure[eC.e_Closure_len++]=edges[i].end;
    }
    return eC;
}

Automata::emptyClosure* Automata::cal_AllState_emptyClosure(){
    emptyClosure *eC_S=new emptyClosure[stateNum];
    for(int i=0;i<stateNum;i++)
        eC_S[i]=init_emptyClosure_noTransfer(S[i]);
    return  eC_S;
}

//用于sort的排序函数
bool complare(int a,int b){
    return a<b;
}

void Automata::init_emptyClosure_Transfer(emptyClosure * &AllState_emptyClosure){
    //对每一个空闭包扫描进行传递空闭包的运算
    for(int i=0;i<stateNum;i++){
        if(AllState_emptyClosure[i].e_Closure_len==0)
            continue;
    //扫描空转移数组，加入传递闭包
    //前提：Thompson算法不会产生空闭包Loop
    for(int j=0;j<AllState_emptyClosure[i].e_Closure_len;j++){
        //不会产生传递闭包
        emptyClosure aimClosure=AllState_emptyClosure[AllState_emptyClosure[i].e_Closure[j]];
        if(aimClosure.e_Closure_len==0)
            continue;
        //会产生传递闭包
        for(int k=0;k<aimClosure.e_Closure_len;k++){
            int temp=aimClosure.e_Closure[k];
            bool ifHasTemp=false;
            for(int l=0;l<AllState_emptyClosure[i].e_Closure_len;l++)
                if(AllState_emptyClosure[i].e_Closure[l]==temp)
                    ifHasTemp=true;
            if(!ifHasTemp)
                AllState_emptyClosure[i].e_Closure[AllState_emptyClosure[i].e_Closure_len++]=temp;
        }
    }
}
    //显示所有状态的空闭包集合
    if(showSomeProcess){
        for(int i=0;i<stateNum;i++){
            cout<<"当前状态: "<<i<<endl;
            cout<<"空闭包: "<<endl;
            if(AllState_emptyClosure[i].e_Closure_len==0){
                cout<<"不存在!"<<endl;
                cout<<endl<<"------------------------------------"<<endl;
                continue;
            }
            for(int j=0;j<AllState_emptyClosure[i].e_Closure_len;j++)
                cout<<AllState_emptyClosure[i].e_Closure[j]<<"  ";
            cout<<endl<<"------------------------------------"<<endl;
        }
    }
}

int Automata::ifNewStateExist(DFAelem a,DFAelem b){
    //大小不符合返回-2
    if(a.ClosureSize!=b.ClosureSize)
        return -2;
    //对空闭包进行排序：从小到大
    sort(a.Closure, a.Closure+a.ClosureSize, complare);
    sort(b.Closure, b.Closure+b.ClosureSize, complare);
    //内容不符合返回-3
    for(int i=0;i<a.ClosureSize;i++)
        if(a.Closure[i]!=b.Closure[i])
            return -3;
    //一样，返回存在的编号
    return a.newState;
}

void Automata::improveDFAClosure(DFAelem &elem,int sID,emptyClosure* all){
    //添加本元素，如果本元素在，则终止；不在还需要添加本元素的闭包。
    bool exist=false;
    for(int i=0;i<elem.ClosureSize;i++)
        if(sID==elem.Closure[i])
            exist=true;
    if(exist)
        return;
    for(int i=0;i<all[sID].e_Closure_len;i++){
        bool eExit=false;
        for(int q=0;q<elem.ClosureSize;q++)
            if(elem.Closure[q]==all[sID].e_Closure[i])
                eExit=true;
        if(!eExit)
            elem.Closure[elem.ClosureSize++]=all[sID].e_Closure[i];
    }
    elem.Closure[elem.ClosureSize++]=sID;
}

void Automata::mergeNFA(emptyClosure * &AllState_emptyClosure){
    //新的move函数用栈存储,最后更新进edges中
    stack<Edge> newEdges;
    //新的状态集数组
    DFAelem * newSList=new DFAelem[stateNum];
    //新的状态集个数
    int newstateNum=0;
    //初始化新的初态
    newSList[0].newState=0;
    improveDFAClosure(newSList[0],S0,AllState_emptyClosure);
    newstateNum++;
    if(showSomeProcess){
        cout<<endl<<"-----------First NFA Unit-----------"<<endl;
        cout<<"Old S0:"<<S0<<endl;
        cout<<"本状态集内容:"<<endl;
        for(int p=0;p<newSList[0].ClosureSize;p++)
            cout<<newSList[0].Closure[p]<<"  ";
        cout<<endl<<"------------------------------------"<<endl;
    }
    //开始扫描，生成完整的新的DFA
    for(int j=0;j<newstateNum;j++){
        //对字符集扫描遍历,针对每一个非空转移条件
        for(int k=0;k<charNum;k++){
            //对本状态的空闭包扫描
            //是否存在该字符的转移
            bool existS=false;
            for(int l=0;l<newSList[j].ClosureSize;l++){
                //该状态在该字符集下的转移情况
                for(int m=0;m<edgeNum;m++){
                    //存在转移条件
                    if(newSList[j].Closure[l]==edges[m].start && chars[k]==edges[m].condition){
                        existS=true;
                        improveDFAClosure(newSList[newstateNum],edges[m].end,AllState_emptyClosure);
                    }
                }
            }
            //存在转移
            if(existS){
                //判断是否产生新的状态，新的状态还要判断是否是新的终态
                int endId=-9;
                for(int n=0;n<newstateNum;n++){
                    endId=ifNewStateExist(newSList[n],newSList[newstateNum]);
                    if(endId>=0)
                        break;
                }
                //检错
                if(endId==-9){
                    cout<<"DFA判断越界!";
                    exit(-1);
                }
                //是新的状态
                if(endId<0){
                    newSList[newstateNum].newState=newstateNum;
                    endId=newstateNum++;
                }else{
                    //销毁newSList[newstateNum]
                    newSList[newstateNum].ClosureSize=0;
                }
                //添加转移的边
                Edge newE;
                newE.start=j;
                newE.end=endId;
                newE.condition=chars[k];
                newEdges.push(newE);
            }
        }
    }
    //设置新的起点
    S0=0;
    //设置新的终点集
    for(int p=0;p<newstateNum;p++){
        bool ifNewF=false;
        for(int q=0;q<newSList[p].ClosureSize;q++)
            if(newSList[p].Closure[q]==F){
                ifNewF=true;
                break;
            }
        if(ifNewF)
            Fs[FsNum++]=newSList[p].newState;
    }
    //更新状态集
    stateNum=newstateNum;
    //设置新的转移函数
    edgeNum=0;
    while (!newEdges.empty()) {
        edges[edgeNum++]=newEdges.top();
        newEdges.pop();
    }
    //输出当前NFA状态到文件 "ResultOfDFA_RegularExpression.txt"
    writeNowState("ResultOfDFA_"+RegularExpression+".txt",2);
    //销毁分配的空间
    delete [] newSList;
    //显示部分过程
    if(showSomeProcess){
        cout<<endl<<"-------------NFA Unit-------------"<<endl;
        cout<<"新状态集个数:"<<newstateNum<<endl<<endl;
        for(int p=0;p<newstateNum;p++){
            cout<<"New State ID:"<<newSList[p].newState<<endl;
            cout<<"空闭包集合:";
            for(int q=0;q<newSList[p].ClosureSize;q++)
                cout<<newSList[p].Closure[q]<<"  ";
            cout<<endl<<endl;
        }
        cout<<"新的转移函数:"<<endl;
        for(int q=0;q<edgeNum;q++)
            cout<<"start: "<<edges[q].start<<" end: "<<edges[q].end<<" condition: "<<edges[q].condition<<endl;
        cout<<endl;
        cout<<"新的起点:"<<S0<<endl;
        cout<<"新的终态集:";
        for(int p=0;p<FsNum;p++)
            cout<<Fs[p]<<"  ";
            cout<<endl;
        cout<<"----------------------------------"<<endl;
    }
}

void Automata::NFAtoDFA(){
    emptyClosure* all_emptyClosure=cal_AllState_emptyClosure();
    init_emptyClosure_Transfer(all_emptyClosure);
    mergeNFA(all_emptyClosure);
}

//最小DFA部分：

Automata::MinDFAelem* Automata::init_divideStartOrFinal(){
    MinDFAelem * result=NULL;
    //初始化大小为当前转移函数总数量
    result=new MinDFAelem[stateNum];
    for(int i=0;i<stateNum;i++){
        //确定当前状态
        result[i].minDFA_state=i;
        //确定当前状态分类
        result[i].minDFA_class=0;
        for(int j=0;j<FsNum;j++)
            if(result[i].minDFA_state==Fs[j])
                result[i].minDFA_class=1;
        //添加相应边
        //设置空边条件
        Edge emptyEdge;
        emptyEdge.start=-1;
        emptyEdge.end=-1;
        emptyEdge.condition=EmptyShift;
        //对边初始化,全部初始化为空边,再更新存在的边
        result[i].minDFA_edges=new Edge[charNum];
        for(int j=0;j<charNum;j++)
            result[i].minDFA_edges[j]=emptyEdge;
        for(int j=0;j<edgeNum;j++){
            if(edges[j].start==result[i].minDFA_state){
                for(int k=0;k<charNum;k++)
                    if(chars[k]==edges[j].condition){
                        result[i].minDFA_edges[k]=edges[j];
                        break;
                    }
            }
        }
    }
    //测试过程
    if(showSomeProcess){
        cout<<endl<<"--------------------最小DFA初始化流程---------------------"<<endl;
        for(int i=0;i<stateNum;i++){
            cout<<"状态ID: "<<result[i].minDFA_state<<endl;
            cout<<"ID类属: ";
            if(result[i].minDFA_class==0)
                cout<<"非终态"<<endl;
            else
                cout<<"终态"<<endl;
            cout<<"转移函数:"<<endl;
            for(int j=0;j<charNum;j++){
                cout<<"条件: "<<chars[j];
                if(result[i].minDFA_edges[j].end==-1){
                    cout<<" 不存在"<<endl;
                }else{
                    cout<<" 终点:"<<result[i].minDFA_edges[j].end<<"  "<<endl;
                }
            }
            cout<<endl;
        }
        
        
        cout<<endl<<"---------------------------------------------------"<<endl;
    }
    return result;
}









void Automata::DFAtoMinDFA(){
    MinDFAelem* minDFA=init_divideStartOrFinal();
}

//构造函数和测试函数部分

Automata::Automata(){
    cout<<"已调用Automata无参构造函数..."<<endl;
}

Automata::Automata(string RegularExpressionIn){
    //执行正规式转换NFA
    RegularExpression=RegularExpressionIn;
    ScanRegularExpression(RegularExpression);
    printNowState(1);
    //执行NFA转换DFA
    NFAtoDFA();
    printNowState(2);
    //执行DFA转换MinDFA
}

void Automata::testFunction1(){
    //测试NFA部分
    cout<<endl<<"--------------------执行测试函数--------------------"<<endl;
    cout<<"测试NFA部分:"<<endl;
    string testString="abc";
    RegularExpression=testString;
    ScanRegularExpression(testString);
    printNowState(1);
    //测试DFA部分
    cout<<endl<<"-------------------------------------------------"<<endl;
    cout<<endl<<"测试DFA部分"<<endl;
    NFAtoDFA();
    printNowState(2);
    //测试MinDFA部分
    cout<<endl<<"-------------------------------------------------"<<endl;
    cout<<endl<<"测试MinDFA部分"<<endl;
    DFAtoMinDFA();
    cout<<endl<<"--------------------测试函数终止--------------------"<<endl;
}











