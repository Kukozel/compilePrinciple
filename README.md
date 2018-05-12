#说明
##文件命名
>- main.cpp为程序入口。附有程序调用说明。
>- LexicalAnalysis.hpp 为头文件。词法分析part1部分的程序的声明，实现正规式到最小DFA的实现。包含详细注释。
>- LexicalAnalysis.cpp 为对应头文件程序的具体实现。包含详细注释。
>- LexicalCombination.hpp 为头文件。词法分析part2部分的程序的声明，实现应用最小DFA处理文本，生成记号流。包含详细注释。
>- LexicalCombination.cpp 为对应头文件程序的具体实现。包含详细注释。

##LexicalAnalysis模块说明
###基本信息说明
>- 该模块实现了正规式到NFA，NFA到DFA，最小化DFA三个部分。
>- 正规式到NFA部分，输入正规式要求：1.半角输入；2.连接符号省略；或为'|'；闭包为'*'；括号使用'(',')'。举例：(ab|c)*，可以调用testFunction( )函数测试用例。
>- 程序调用接口为Automata(string RegularExpressionIn,bool ShowDetail,bool Write)。RegularExpressionIn为输入正规式；ShowDetail参数为是否显示结果到控制台；Write参数为是否输出到文件。
> - 程序getResultOfMinDFA()接口为最小DFA导出。为后期其他模块调用最小DFA使用。

###参数设置说明
>- 头文件showSomeProcess参数调整中间过程细节是否显示。
>- 头文件以下分配空间大小的参数随需求修改。
> 默认情况：StateMaxSize=512(状态集最大数量);
> static const int CharsMaxSize=64(字符集最大数量);
> static const int EdgeMaxSize=512(转移函数最大数量);
> static const char EmptyShift='#'(设置空转移字符);
> static const int EmptyClosureSize=512;(空转移集合最大状态);

###输出文件说明
>- 正规式到NFA部分，执行成功后，输出到当前文件夹"ResultOfNFA_RegularExpression.txt"文件中。
>- NFA到DFA部分，执行成功后，输出到当前文件夹"ResultOfDFA_RegularExpression.txt"文件中。
>- NFA到DFA部分，执行成功后，输出到当前文件夹"ResultOfMinDFA_RegularExpression.txt"文件中。

##LexicalCombination模块说明
###基本信息说明
>- 该模块实现了应用最小DFA处理文本，生成记号流。
>- 识别8种记号。关键词，标识符，分隔符，运算符，整数，浮点数，字符，字符串。具体细节见cpp实现文件中的正规式定义部分。
>- 为了减少正规式到最小DFA转换时因正规式过大，使用同等替换字符串方式实现将52个英文字符(大小写各26个)统一替换为字符 'p'。后期再转换回原字符。详见init_commonSignals()函数。
>- 为了避免字符中 ( , ) , * , | 等与正规式运算时的定义运算符冲突，采用字符替换方式。详见solveOperateFormulaCrash()函数。
>- 程序调用接口为SignStream(string targetString,bool showDetail,bool writer);。targetString为输入文本；ShowDetail参数为是否显示结果到控制台；Write参数为是否输出到文件。

###参数设置说明
>- 头文件printProcess参数调整中间过程细节是否显示。
>- 头文件以下分配空间大小的参数随需求修改。
> 默认情况：signalSaver_MaxSize=128(存储记号流最大数量);

###输出文件说明
>- 程序执行成功后，记号流说明及记号流输出到当前文件夹"ResultOfSignStream_XX"文件中。XX为生成记号流数量。