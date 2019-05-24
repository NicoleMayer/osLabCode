#include <stdlib.h>
#include <stdio.h>
#include <string.h>
struct Student{
  char sid[11];
  char name[20];
  int mark; 
};

int main(int argc, char *argv[])
{
  // struct Student stu = {"11611712", "Yang Hanmei", 100};
  // strcpy("11611712", stu.sid);
  // strcpy("Yang Hanmei", stu.name);
  // stu.mark = 100;
   
  FILE * fp1 = fopen("grade.txt", "r");
  struct Student stu;
  // 终于意识到了我的问题 因为名字这有个空格 所以识别失败
  while(fscanf(fp1, "%s %s %d", stu.sid, stu.name, &stu.mark) == 3)
  {
    // printf("%s %s %d\n", stu.sid, stu.name, stu.mark);
  }
  fclose(fp1);
  
  // FILE * fp2 = fopen("grade.bin", "rb");
  // struct Student stu;
  // while(fread(&stu, sizeof(struct Student), 1, fp2) !=0)
  // {
  //   // printf("%s %s %d\n", stu.sid, stu.name, &stu.mark);
  // }
  // fclose(fp2);
  
}
