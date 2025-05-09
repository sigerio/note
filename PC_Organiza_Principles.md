1:CPU如何访问内存数据的：
    CPU直接访问硬盘的效率非常低，所以缓存Cache是必要的，而Cache一般使用的是SRAM，并且一般存在多级缓存
    内存比缓存速度低，但是空间比缓存大，一般使用DRAM，DRAM的电气特性要求必须不断刷新其内部电容，否则会丢失数据
    硬盘空间最大，但是访问速度最慢
层次结构：
cpu内部：寄存器
        L1Cache(SRAM)
        L2Cache(SRAM)
        L3Cache(SRAM)

内存    DRAM

硬盘    SSD/HHD

存储器一般只与相邻的存储设备进行通信


CPU访问数据流程，一级级向下访问，知道取到为止

CPU--->寄存器--->有--->读取
            --->没有--->L1...

CPU对数据的读取，以及Cache的理解
Cache的结构：Cache是由许多个Cache Line组成，Cache Line(缓存块)是CPU从内存读取数据的基本单位，Cache Line又是由Tag+Data Block构成，其中Tag标记了Data Block的各种信息，比如，是否有效等

{在系统中可以查看各个Cache Line的大小，Cache一次性读取的数据大小就是Cache Line的大小}