将Ti打包的驱动库移植出来
1.将inc包含到keil的头文件路径下，
2.工程目录中添加msp432p4xx_driverlib.lib
3工程目录中添加汇编启动文件.s
4.工程目录中添加外设接入源文件.c


keil下可以转到定义查看函数定义或者修改源代码
如需直接使用ti打包好的驱动库文件，在每个函数前加前缀MAP_可以使用ti出厂配置于ROM区的代码，可节约flash代码空间。
如果熟悉ti的驱动函数，可以直接使用MAP_对应的函数。
具体原理详细可以观看cloud的MSP432入门教程以及STM32库开发实战指南对底层启动有更加
深刻的理解