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
  int count;
  if (argc != 2)
  {
    return -1;
  }
  sscanf(argv[1], "%d", &count);
  struct Student stu = {"11611712", "YangHanmei", 100};
  // strcpy("11611712", stu.sid);
  // strcpy("Yang Hanmei", stu.name);
  // stu.mark = 100;
   
  FILE * fp1 = fopen("grade.txt", "w");
  // FILE * fp2 = fopen("grade.bin", "wb");
  while(count--)
  {
    fprintf(fp1, "%s %s %d\n", stu.sid, stu.name, stu.mark);
    // fwrite(&stu, sizeof(struct Student), 1, fp2);
  }
  fclose(fp1);
  // fclose(fp2);
  return 0;
}