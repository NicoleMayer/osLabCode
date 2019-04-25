#include<bits/stdc++.h>
#include<unistd.h>

using namespace std;

#define PROCESS_NAME_LEN 32 //进程名最大长度
#define MIN_SLICE 10 //内碎片最大大小
#define DEFAULT_MEM_SIZE 1024  //总内存大小
#define DEFAULT_MEM_START 0  //内存开始分配时的起始地址

typedef pair<int, string> My_algo;

int mem_size = DEFAULT_MEM_SIZE;
bool flag = 0; //当内存以及被分配了之后，不允许更改总内存大小的flag
static int pid = 1;
My_algo algo[3];
My_algo cur_algo; //默认为first fit算法

struct free_block{	//空闲数据块
	int size;
	int start_addr;
	struct free_block *next;
};

struct allocated_block{ //已分配的数据块
	int pid;
	int size;
	int start_addr;
	char process_name[PROCESS_NAME_LEN];
	int *data;
	struct allocated_block *next;
};

free_block *free_block_head; //空闲数据块首指针
allocated_block *allocated_block_head = NULL; //分配块首指针

allocated_block *find_process(int id); //寻找pid为id的分配块
free_block *init_free_block(int mem_size); //空闲块初始化
void display_menu(); //显示选项菜单
void set_mem_size(); //设置内存大小
int allocate_mem(allocated_block *ab); //为制定块分配内存
void rearrange(); // 对块进行重新分配
int create_new_process(); //创建新的进程
int free_mem(allocated_block *ab); //释放分配块
void swap(int *p, int *q); //交换地址
int dispose(allocated_block *ab); //释放分配块结构体
void display_mem_usage(); //显示内存情况
void kill_process(); //杀死对应进程并释放其空间与结构体
void Usemy_algo(); //使用对应的分配算法
void init_my_algo(); //初始化 algorithms

//主函数
int main(){
	int op;
	init_my_algo();
	free_block_head = init_free_block(mem_size); //初始化一个可以使用的内存块，类似与操作系统可用的总存储空间
	for(;;){
		sleep(1);
		display_menu();
		fflush(stdin);
		if(scanf("%d", &op) == 0){
			char ch;
			while ( (ch = getchar()) != '\n' && ch != EOF) { }
			puts("invalid operation");
		}
		switch (op){
			case 1:{ set_mem_size(); break; }
			case 2:{ Usemy_algo(); break; }
			case 3:{ create_new_process(); break; }
			case 4:{ kill_process(); break; }
			case 5:{ display_mem_usage(); break; }
			case 233:{ puts("bye...."); sleep(1); return 0; }
			defaut: break;
		}
	}
}

void init_my_algo(){
	 algo[0] = make_pair(0, "FIRST_FIT");
	 algo[1] = make_pair(1, "BEST_FIT");
	 algo[2] = make_pair(2, "WORST_FIT");
	 cur_algo = algo[0]; //默认为first fit算法
}

void Usemy_algo(){ //选择不同的算法
	int type = 0;
	puts("Choose from 0: first fit \t1: best fit \t2: worst fit\n");
	if(scanf("%d", &type) == 0){
		char ch;
		while ( (ch = getchar()) != '\n' && ch != EOF) { }
		puts("invalid algorithm type, use default: first fit");
	}
	cur_algo = algo[type];
}

allocated_block *find_process(int id){ //循环遍历分配块链表，寻找pid=id的进程所对应的块
	// completed 返回找到的process,若没找到则为NULL
	allocated_block *ab = allocated_block_head;
	while (ab != NULL){
		if (ab->pid == id){
			return ab;
		}
		ab = ab->next;
	}
	return NULL;	
}

free_block *init_free_block(int mem_size){ //初始化空闲块，这里的mem_size表示允许的最大虚拟内存大小
	free_block *p;
	p = (free_block *)malloc(sizeof(free_block));
	if (p == NULL){
		puts("No memory left");
		return NULL;
	}
	p->size = mem_size;
	p->start_addr = DEFAULT_MEM_START;
	p->next = NULL;
	return p;
}

void display_menu(){
	puts("\n\n******************menu*******************");
	printf("1) Set memory size (default = %d)\n", DEFAULT_MEM_SIZE);
	printf("2) Set memory allocation algorithm\n");
	printf("3) Create a new process\n");
	printf("4) Kill a process\n");
	printf("5) Display memory usage\n");
	printf("233) Exit\n");
}

void set_mem_size(){ //更改最大内存大小 
	// completed 无返回值
	int want_mem_size;
	puts("Please input the memory size you want");
	if(scanf("%d", &want_mem_size) == 0){
		char ch;
		while ( (ch = getchar()) != '\n' && ch != EOF) { }
		puts("invalid memory size, use deafult 1024 instead");
		return;
	}
	if (flag == 0){ //我的逻辑是 当存在为进程分配空间 flag=1不能再修改了
		mem_size = want_mem_size;
		free_block_head->size = mem_size;
	} else {
		puts("Can't modify memory size anymore!");
	}
}

bool first_fit_better(free_block *a, free_block *b){ //first fit comparsion
	return false;
}

bool best_fit_better(free_block *a, free_block *b){ //best fit comparsion
	// 相等则遵循 first_fit 原则
	return (a->size < b->size)?true:false;
}

bool worst_fit_better(free_block *a, free_block *b){ //worst fit comparsion
	// 相等则遵循 first_fit 原则
	return (a->size > b->size)?true:false;
}

int allocate_mem(allocated_block *ab){ //为块分配内存，真正的操作系统会在这里进行置换等操作
	// completed 返回值 -1: 无法分配 其余情况表示分配地址的起点
	// 1. 判断size是否满足 2. 根据相应算法找到合适的插入的位置 3. free block分块
	if (ab->size < 0 || ab->size > mem_size) return -1;
	free_block *fb = free_block_head, *cur_optimal = NULL; // 第二个变量前的 * 很重要！
	while (fb != NULL){
		if (fb->size >= ab->size){
			if (cur_optimal == NULL || (cur_algo.first == 0 && first_fit_better(fb, cur_optimal))
				|| (cur_algo.first == 1 && best_fit_better(fb, cur_optimal))
				|| (cur_algo.first == 2 && worst_fit_better(fb, cur_optimal))
			){
				cur_optimal = fb;
			}
		}
		fb = fb->next;
	}

	if (cur_optimal == NULL){
		return -1;
	}
	cur_optimal->size -= ab->size;
	cur_optimal->start_addr += ab->size;

	return cur_optimal->start_addr - ab->size;
}

int create_new_process(){ //创建新进程
	int proc_mem_size;
	char proc_name[PROCESS_NAME_LEN]; //TODO 不知道这样初始化对不对？
	puts("Please input the memory size allocated for this process");
	while (scanf("%d", &proc_mem_size) == 0){
		char ch;
		while ( (ch = getchar()) != '\n' && ch != EOF) { }
		puts("invalid process memory size, enter again");
	}
	puts("Please input the name for the new process");
	scanf("%s", proc_name);

	allocated_block *p;
	p = (allocated_block *)malloc(sizeof(allocated_block));
	p->pid = pid; pid++;
	p->size = proc_mem_size;
	memcpy(p->process_name, proc_name, PROCESS_NAME_LEN * sizeof(char));
	p->data = NULL; 
	p->next = NULL;

	p->start_addr = allocate_mem(p);
	
	if (p->start_addr == -1){
		puts("Allocate failed, no enough memory");
		free(p);
		return -1;
	}
	flag = 1;
	if (allocated_block_head == NULL){
		allocated_block_head = p;
	}
	else
	{
		allocated_block *tmp = allocated_block_head;
		while(tmp->next != NULL){
			tmp = tmp->next;
		}
		tmp->next = p;
	}
	puts("Allocate succeed");
	return 0;
}

void swap(int *p, int *q){
	int tmp = *p;
	*p = *q;
	*q = tmp;
	return;
}

void rearrange(){ //将块按照地址大小进行排序 
	free_block *tmp, *tmpx;
	puts("Rearrange begins...");
	puts("Rearrange by address...");
	tmp = free_block_head;
	while(tmp != NULL){
		tmpx = tmp->next;
		while (tmpx != NULL){
			if (tmpx->start_addr < tmp->start_addr){
				//合并连在一起的block
				swap(&tmp->start_addr, &tmpx->start_addr);
				swap(&tmp->size, &tmpx->size);
				if (tmp->start_addr + tmp->size == tmpx->start_addr){
					tmp->size += tmpx->size;
					tmp->next = tmpx->next;
					tmpx->next = NULL;
					free(tmpx);
				}

			}
			tmpx = tmpx->next;
		}
		tmp = tmp->next;
	}
	usleep(500);
	puts("Rearrange Done.");
}


int free_mem(allocated_block *ab){ //释放某一块的内存	
	//completed 无有含义的返回值
	//1. free_block 插入一个block rearrange根据地址排序  
	//2. allocate_block 删除一个block 释放process内存
	free_block *p;
	p = (free_block *)malloc(sizeof(free_block));
	p->size = ab->size;
	p->start_addr = ab->start_addr;

	if (free_block_head == NULL){
		free_block_head = p;
	}
	else
	{
		free_block *tmp = free_block_head;
		while(tmp->next != NULL){
			tmp = tmp->next;
		}
		tmp->next = p;
		rearrange(); //TODO 这里碎片需不需要合并？？？
	}
	return 0;
}

int dispose(allocated_block *fab){ //释放结构体所占的内存
	allocated_block *pre, *ab;
	if (fab == allocated_block_head){
		allocated_block_head = allocated_block_head->next;
		free(fab);
		return 1;
	}
	pre = allocated_block_head;
	ab = allocated_block_head->next;
	while (ab != fab){ pre = ab; ab = ab->next;}
	pre->next = ab->next;
	free(ab); 
	return 2;
}

void display_mem_usage(){
	free_block *fb = free_block_head;
	allocated_block *ab = allocated_block_head;
	puts("*********************Free Memory*********************");
	printf("%20s %20s\n", "start_addr", "size");
	int cnt = 0;
	while (fb != NULL){
		cnt++;
		printf("%20d %20d\n", fb->start_addr, fb->size);
		fb = fb->next;
	}
	if (!cnt) puts("No Free Memory");
	else printf("Totaly %d free blocks\n", cnt);
	puts("");
	puts("*******************Used Memory*********************");
	printf("%10s %20s %10s %20s\n", "PID", "ProcessName", "start_addr", "size");
	cnt = 0;
	while (ab != NULL){
		cnt++;
		printf("%10d %20s %10d %20d\n", ab->pid, ab->process_name, ab->start_addr, ab->size);
		ab = ab->next;
	}
	if (!cnt) puts("No allocated block");
	else printf("Totaly %d allocated blocks\n", cnt);
	return;
}

void kill_process(){ //杀死某个进程
	allocated_block *ab = NULL;
	int pid = -1;
	puts("Please input the pid of Killed process");
	if(scanf("%d", &pid) == 0){
		char ch;
		while ( (ch = getchar()) != '\n' && ch != EOF) { }
		puts("invalid pid");
		return;
	}

	if (pid > 0){
		ab = find_process(pid);
	} 
	
	if (ab != NULL){
		free_mem(ab);
		dispose(ab);
	}else{
		printf("No process with pid %d\n", pid); //myadd
	}
}



