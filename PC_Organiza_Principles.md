1:CPU如何访问内存数据的: 
    CPU直接访问硬盘的效率非常低，所以缓存Cache是必要的，而Cache一般使用的是SRAM，并且一般存在多级缓存
    内存比缓存速度低，但是空间比缓存大，一般使用DRAM，DRAM的电气特性要求必须不断刷新其内部电容，否则会丢失数据
    硬盘空间最大，但是访问速度最慢
层次结构: 
cpu内部: 寄存器
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
Cache的结构: Cache是由许多个Cache Line组成，Cache Line(缓存块)是CPU从内存读取数据的基本单位，Cache Line又是由Tag+Data Block构成，其中Tag标记了Data Block的各种信息，比如，是否有效等
{注意: 一个Block并非全是数据，Block能和Cache Line对应起来的前提是两者结构一致，所以Block也是由Tag+Data构成的}
{在系统中可以查看各个Cache Line的大小，Cache一次性读取的数据大小就是Cache Line的大小，该大小也就是内存块的大小}



直接映射: 
    将内存块的地址映射在Cache Line中，假如我们的Cache共有8个Cache Line，一共存在32个内存块(Block)，那么这时的对应关系大致如下
    
    Cache Line0: Block0 Block8  Block16 Block24
    Cache Line1: Block1 Block9  Block17 Block25
    Cache Line2: Block2 Block10 Block18 Block26
    Cache Line3: Block3 Block11 Block19 Block27
    Cache Line4: Block4 Block12 Block20 Block28
    Cache Line5: Block5 Block13 Block21 Block29
    Cache Line6: Block6 Block14 Block22 Block30
    Cache Line7: Block7 Block15 Block23 Block31

可以看到当我们有m个Cache Line，当我们想访问Blockn时，仅需要知道n%m的值，就可以知道该Block存在于哪个Cache Line中了
我们知道Cache Line是由Tag 和 Data构成，而Tag将会存储Data的信息，比如Data的索引/地址
同样的，我们看到一个Cache Line可能会对应多个Block，所以Tag中将会记录当前的Data中记录的是哪一个块即索引
同时，还需要一个有效位来标记当前Block是否有效，如果所需要的数据不是一整个数据块，而是一个字(4字节),那么还需要计算一个地址偏移量
则，CPU访问数据的流程大致如下: 
    首先根据目标数据的地址(目标地址)计算Block的索引，根据索引访问对应的Cache Line，然后判断Cache Line中的有效位，如果数据是无效的，则重新加载Block至Cache Line中，如果有效，则判断当前Cache Line中的索引是否是需要访问的索引，如果不在Cache Line中(即当前Cache Line中的索引并非需要访问的索引)，则重新加载数据至Cache Line中，如果两个一致则直接根据地址偏移量取出数据

{内存的映射还有其他方法，比如全相连，组相连等}




2:CPU如何写入数据的
写直达:判断数据是否在cache中，在的话直接写入内存，不在的话，先写入cache，再写入内存
写回:仅写入cache中，当且仅当访问的cache line标记为dirty时且该cache line被替换时才会写回至内存

内存一致性: 因为写回的特性，当多核共同处理一个数据时，会因为其中某核的写回操作，将数据该核的cache ling中，从而导致其他核获取到的数据是旧数据
    解决策略: 写传播(当一个cpu的cache line被标记为dirty时，需要将其他的cpu的cache也同步修改)、事务串行化(可以理解为数据同步)
        所谓事务串行化，意思就是总线上所有操作都应按顺序串行执行，才能保证不同角色对目标进行修改时，总线上数据保持一致
    
    总线嗅探: CPU会监听总线上所有活动，并且针对针对所有活动发出广播
    M(Modified已修改)E(Exclusive独占)S(Shared共享)I(Invalidated已失效)协议: 基于总线嗅探实现的事务串行化协议
    M:脏标记就是已修改
    E: 数据为当前cache独占，可以自由写入数据，不用担心其他cpu持有该数据
    S: 如果数据被其他cache获取，则自动从E状态切换至S状态，此时对数据进行操作时，需要CPU向其他核心广播请求，将所有持有该数据的标记更改为无效状态，才能更新当前Cache的数据
    I:失效数据，已失效的数据需要先确认其他cache是否拥有该数据，且状态为M，有的话就需要更新内存的数据，否则就需要从内存中重新加载




3:CPU的执行

伪共享: MESI协议虽然规定了数据修改的过程，但是存在一种特殊情况: 
    加入两个数据物理地址连续且处于同一个Block中，当A，B两个核心分别修改data0和data1时，经由MESI协议会不断地互相修改cache的ES状态，同时也会不断修改内存中的数据，这样的极端情况会导致cache无效，每次操作都会从内存中加载数据，这就是伪共享

    解决伪共享的办法: 内核提供了一个数据结构，当在多核系统中时，如果两个数据物理地址连续，且处于同一个Block中时，会被加载至不同的cache line中


线程的选择: 