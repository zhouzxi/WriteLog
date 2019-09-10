# WriteLog
基于 Linux 的应用层日志系统 C 代码实现

对于程序的说明如下：
1.WriteLog.h 和 WriteLog.c 是分别是日志系统的头文件和源代码文件，Test.c 是对日志系统进行测试的代码。
2.WriteLog.c 中，各函数的作用如下：
(1)LogLevel：获取对应的日志等级。
(2)GetTime：获取当前日志生成的时间。
(3)GetConfigValue：获取各配置项的值。
(4)GetStringContentValue：获取字符串的值。
(5)GetConfigFileStringValue：从配置文件中获取属性为字符串的配置项的值。
(6)GetConfigFileIntValue：从配置文件中获取属性为整型的配置项的值。
(7)WriteLogFile：将日志内容写到日志文件中。
具体各函数的实现流程请详细阅读代码。
3.WriteLog.h 中，定义了一个函数宏 WRITELOGFILE 用来供其他程序调用完成写日志的操作；其他程序模块在调用 WRITELOGFILE 的时候，只需要传入日志等级和需要写入日志文件的具体消息即可。

4.编译命令：gcc -g -o WriteLog Test.c WriteLog.c
代码存放路径：/home/xxx/test/WriteLog
配置文件存放路径：/home/xxx/test/WriteLog
