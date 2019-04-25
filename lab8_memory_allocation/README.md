# mm.cpp

This is code for memory allocation, use best fit, worst fit and first fit. I also realize merging two continuous block after putting a allocation block back to the free block list.

I have to admit some realizations are inefficient, but this code is just for implementing a strategy to realize memory allocation.



- [ ] Original I want to realize the buddy system algorithm, but I didn't have too much time so I gave up. Maybe I will try it later.



Some problems I have met

**Problem1:** 

free_block *fb = free_block_head, cur_optima;

variable ‘cur_optima’ causes a lot of errors and warnings in the compile process

**Solution1:**

Add * before cur_optima, turn this line into 

free_block *fb = free_block_head, *cur_optima;

Since the definition of multiple variables in the same line only ensures the same type. If the variable is an array of a pointer, it needs to add corresponding representation before or after this variable.



**Problem2:**

Enter a string will lead to infinite loop without operations.

**Solution2:**

Although the code has fflush(stdin), it seems that it doesn’t work. So I add the following code in the main function in order to flush the invalid string input to avoid infinite loop without operations. Besides, I also check all of the codes using scanf to read integer. Use this way to flush the invalid string.                                                          