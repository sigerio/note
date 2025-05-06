gcc编译程序至可执行文件.exe .out 等类型的过程
{}中将会给出参考例程的操作过程
一：预编译：# -> （Preprocessor Directive）
    gcc会展开所有以#字符开头的语法所引用的文件、宏定义等，例如#define ABC 1;
    则在预编译阶段，所有ABC便会被替换为1；一般使用-E进行文件的预编译

gcc -E *.c -o *.i
“当.c文件中出现非当前路径下的引用时，需要使用-I来指示其引用文件夹,特殊的，当出现多个不同的头文件路径时，需要在每个路径前加-I”

gcc -E -Iinclude1 -Iinclude2 *.c -o *.i


需要将该操作使用在工程内的所有.c文件

{
    gcc -E -Iinclude -Iinclude2 main.c -o ./build/main.i
    gcc -E -Iinclude ./src/calcu.c -o ./build/calcu.i
}

二：编译
    在预处理之后，需要将每个预处理之后的程序编译成汇编文件，同时确认程序语法没有问题，过程比较简单，直接将每个.i对应的翻译成.s文件即可

gcc -S *.i -o *.s


{
    gcc -S ./build/calcu.i -o ./build/calcu.s
    gcc -S ./build/main.i -o ./build/main.s
}

三：汇编
    这一步承接上一步，将汇编文件翻译成二进制文件

    gcc -c *.s -o *.o

{
    gcc -c ./build/calcu.s -o ./build/calcu.o
    gcc -c ./build/main.s -o ./build/main.o
}


四：链接
    这一步将会把所有的二进制文件进行链接，输出可执行文件

    gcc file1.o file2.o file3.o *.o -o target

{
    gcc ./build/main.o ./build/calcu.o -o main.out
}    






特殊定义：静态库/动态库
    静态库：作为一整套功能的集合，且是预先编译好的，所以静态库的引入是在程序的编译阶段，并且每一次引用静态库，就会将该库复制一份置于程序中，所以静态库非常占用存储空间

    动态库：动态库在整个系统空间中只存储一份，仅在程序运行到的时候才会去调用，所以动态库常用于进程间资源共享

动态库的非正常理解（个人理解）：动态库提供了独立的可执行接口，其程序运行并非在当前运行堆栈内进行处理，而是在动态库函数的堆栈内进行；所以当仅当程序对该库的接口进行访问时，才会消耗运行堆栈；又或者可以理解为普通的函数调用，但是被调用函数的内存使用并非由调用函数来提供，
具体情况需要经过实验验证来进行分析








通常的，针对单个.c文件的编译，仅需要使用gcc *.c -o ./filepath/*.out 即可
