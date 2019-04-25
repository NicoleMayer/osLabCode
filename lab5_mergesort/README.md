

# Merge Sort in Multithreading

## Requirement

• Implement a multithreading version merge sort in C language, which can deploy divide and merge subroutines to different threads. There is no limitation about the thread libraries. You can use pthread, thread pool, openMP and etc.
• Finish an analysis report, which includes implementation ideas, findings and analysis. 

## implementation

At first, I use `pthread` to creat new threads and divide the array into `thread_count` parts, each part with one thread running. However, after each thread done each task, the array still needs to merge, and two parts' merge is dependent on the range of each part. For example, if [1, 1] and [2, 2] haven't been merged, then [1, 2] and [3, 4] can't be merged. So the final merge I use serial method, the result is really unsatisfying.

![image-20190401220024962](/Users/nicole/Library/Application Support/typora-user-images/image-20190401220024962.png)

As the figure shows, only one thread is better than using 10 thread in such implementation. After searching the Internet, I found that treating merge sort in a reverse way can easily make full use of multi-thread. The original idea is to divide the array recursively until the range length is 1. This process is just like a tree, and the merge process can be done with order so that we don't need to worry about the dependent problem. Using openMP is really easy to implement the idea. The implementation details are shown in the attach .c file.

## Result analysis

Sorting array with size 1000000

| Thread num | First try | Second try | Third try | Avg time   |
| ---------- | --------- | ---------- | --------- | ---------- |
| 1          | 0.331075  | 0.332488   | 0.359817  | 0.34112667 |
| 2          | 0.252337  | 0.245253   | 0.258989  | 0.252193   |
| 5          | 0.26983   | 0.26597    | 0.267028  | 0.26760933 |
| 10         | 0.28168   | 0.280983   | 0.289004  | 0.283889   |

Sorting array with size 10000000

| Thread num | First try | Second try | Third try | Avg time   |
| ---------- | --------- | ---------- | --------- | ---------- |
| 1          | 3.880215  | 3.658601   | 3.590107  | 3.709641   |
| 2          | 2.9635    | 2.901      | 2.7982    | 2.887557   |
| 5          | 3.072711  | 3.153218   | 3.164871  | 3.13026667 |
| 10         | 3.28712   | 3.158615   | 3.173629  | 3.20645467 |

From these two tables, we can conclude that adding thread number indeed can improve the running time, however, too many threads will lead to the context switch cost, which can slow down the running time. If I increase the sorting array size, the outcome will become more obvious. For large scale input, the performance of multiple threads is better than single thread.

In terms of margin gain, actually I find two threads is the best choice for input with 10000000, when I increase the thread count to 3, the average total running time is increasing. With two threads, the margin gain is 0.9s in average.

