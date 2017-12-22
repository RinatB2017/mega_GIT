#include <stdio.h>
#include <stdlib.h>


void strip_file(char *file, int filesize)
{
    if(strlen(file) > 1000)
    {
        printf("filepath too long.");
        exit(1);
    }

    char filename[1024];
    char *data = (char *)malloc(filesize);

    printf("strippin'");

    memset(filename, 0, 1024);
    sprintf(filename, "%s.stripped", file);
    FILE *fp = fopen(file, "r");
    FILE *fpot = fopen(filename, "w");
    fread(data, filesize,1 , fp);
    fwrite(data, filesize, 1, fpot);

    fclose(fpot);
    fclose(fp);
    printf("wrote %d", filesize);
    free(data);

}

int main(int argc, char *argv[])
{

printf("hello! I am stripper, the good kind, not the one that swirls around poles...");
strip_file(argv[1], atoi(argv[2]));
printf("all went well, bye!");

return 0;
}
