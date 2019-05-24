#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	FILE * originfile, * copyfile;
	int c;
	int str[60];

	originfile = fopen(argv[1], "r");

	if (originfile == NULL)
	{
		perror("fopen fail\n");
		exit(EXIT_FAILURE);
	}

	copyfile = fopen(argv[2], "w");

	if (copyfile == NULL)
	{ 
	 	perror("fopen fail\n");
		exit(EXIT_FAILURE);
	}

	// while (1)
	// {
	// 	c = fgetc(originfile);
	// 	if (feof(originfile))
	// 	{
	// 		break;
	// 	}
	// 	fputc(c, copyfile);
	// }
	while (fgets(str, 60, originfile))
	{
		fputs(str, copyfile);
	}


	fclose(originfile);
	fclose(copyfile);
	return 0;
	

}
