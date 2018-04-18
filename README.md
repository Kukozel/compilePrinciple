#说明

##文件命名
>- LexicalAnalysis.hpp 为头文件。词法分析部分程序的声明，包含详细注释。
>- LexicalAnalysis.cpp 为词法分析部分程序的具体实现，包含详细注释。

###LexicalAnalysis模块定义说明
>- 该模块实现了正规式到NFA，NFA到DFA，最小化DFA三个部分。
>- 正规式到NFA部分，输入正规式要求：1.半角输入；2.连接省略；或为'|'；闭包为'*'；括号使用'(',')'。举例：(ab|c)*，可以调用testFunction1( )函数测试结果。
>- 可以直接使用Automata(string RegularExpression)。
>- showSomeProcess设置为true可以显示一些中间过程。
>- 以下分配空间大小随求可以修改。
> 默认情况：StateMaxSize=128(状态集最大数量);
> static const int CharsMaxSize=64(字符集最大数量);
> static const int EdgeMaxSize=128(转移函数最大数量);
> static const char EmptyShift='#'(设置空转移字符);
> static const int EmptyClosureSize=64;(空转移集合最大状态);

###LexicalAnalysis模块输出说明
>- 正规式到NFA部分，执行成功后，输出到当前文件夹"ResultOfNFA_RegularExpression.txt"文件中。
>- NFA到DFA部分，执行成功后，输出到当前文件夹"ResultOfDFA_RegularExpression.txt"文件中。



