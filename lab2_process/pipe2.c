#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
int pid;
int pid_fd[2];
char buff[1024], msg[1024];
void pipe_write()
{
  sleep(1);
  printf("\nMy pid is %d\n", getpid());
  printf("Please input a string:\n");
  fgets(msg, 20, stdin);
  write(pid_fd[1], msg, sizeof(msg));
  kill(getppid(), SIGALRM); //注意这里传递给父进程（也就是读）SIGALRM信号
}
void pipe_read()
{
  sleep(1);
  printf("\nMy pid is %d\n", getpid());
  printf("Read begins\n");
  memset(buff, 0, sizeof(buff));
  read(pid_fd[0], buff, sizeof(buff));
  printf("Message is: %s\n", buff);
  kill(pid, SIGALRM); //这里传递给子进程（也就是写）SIGALRM信号
}
void finish_write()
{
  close(pid_fd[1]);
  printf("Write finished\n");
}
void finish_read()
{
  close(pid_fd[0]);
  printf("Read finished\n");
}
int main()
{
  if (pipe(pid_fd) < 0)
  {
    perror("Pipe Creation Failed\n");
    return -1;
  }
  if ((pid = fork()) < 0)
  {
    perror("Fork Failed\n");
    return -1;
  }
  if (!pid)
  {
    //child
    printf("child process begins, pid = %d\n", getpid());
    signal(SIGALRM, pipe_write); //激活SIGALRM信号
    signal(SIGINT, finish_write);
    kill(getpid(), SIGALRM); //给当前进程传递SIGALRM信号
    //如果没有 while(1) 这段代码就无法正常运行
    while (1)
    {
    }
  }
  else
  {
    //parent
    printf("parent process begins, pid = %d\n", getpid());
    signal(SIGALRM, pipe_read);
    signal(SIGINT, finish_read);
    while (1)
    {
    }
  }
}
