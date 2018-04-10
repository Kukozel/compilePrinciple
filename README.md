#说明

##文件命名
>- LexicalAnalysis.hpp 为头文件。词法分析部分程序的声明，包含详细注释。
>- LexicalAnalysis.cpp 为词法分析部分程序的具体实现，包含详细注释。

###LexicalAnalysis模块说明
>- 该模块实现了正规式到NFA，NFA到DFA，最小化DFA三个部分。
>- 正规式到NFA部分，输入正规式要求：1.半角输入；2.连接省略；或为'|'；闭包为'*'；括号使用'(',')'。举例：(ab|c)*，可以调用testFunction1()函数测试结果。
>- 正规式到NFA部分,执行成功后，输出到当前文件夹"esultOfNFA_RegularExpression.txt"文件中。
>- 可以直接使用Automata(string RegularExpression)。



