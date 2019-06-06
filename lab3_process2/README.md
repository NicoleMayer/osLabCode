# C functions for process2

## 查看进程的状态

![1](/Users/nicole/Desktop/3-操作系统/b.lab/Lab3_code_for_lab/images/1.png)

实际上是这样的⬇️

![2](/Users/nicole/Desktop/3-操作系统/b.lab/Lab3_code_for_lab/images/2.png)

结果

![image-20190606220234378](images/image-20190606220234378.png)



## fork_exec.c

和 `fork_exec2.c` 一样，都实现了模拟 system() 函数的方法。相比之下，后者更加严密，加入了 wait(NULL) 使得父进程不会早于子进程退出。



## Write to same file

![image-20190606220734486](images/image-20190606220734486.png)

![image-20190606220851076](images/image-20190606220851076.png)

O_RDONLY: Open for reading only. O_WRONLY: Open for writing only.

用例说明 write and fwrite 都能锁定正在读写的文件，其他人无法访问

![image-20190606221006246](images/image-20190606221006246.png)



##  wait_exit.c

想了好一会没搞明白这里的深意，原来这是 `wait()` 的知识点。

![image-20190606221659022](images/image-20190606221659022.png)

从下图可以看出，子进程结束后不是立刻就终结的，它需要等待父进程 wait() 的返回值

![image-20190606221835972](images/image-20190606221835972.png)



## time_example.c

这一系列代码是在比较 system time 和 user time



## mq_read.c && mq_write.c

采用 message queue 进行进程间通信的范例

![image-20190606223951225](/Users/nicole/Library/Application Support/typora-user-images/image-20190606223951225.png)

