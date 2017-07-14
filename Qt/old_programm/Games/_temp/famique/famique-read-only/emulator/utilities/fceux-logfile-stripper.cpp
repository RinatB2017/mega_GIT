/*
 this program came in to existance 03/09/2010

 this program strips unneccessary information from FCEUX 6502 log files for easier comparison

 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>



#ifdef CONSERVATIVE_VALUES
#define LINE_STRIP_OFFSET 14
#define LINE_STRIP_LENGTH 26
#define LINE_FCEUX_MAXLEN 75
#else
#define LINE_STRIP_OFFSET 14
#define LINE_STRIP_LENGTH 28
#define LINE_FCEUX_MAXLEN 75
#endif


FILE *infhandle;
FILE *outfhandle;
char *textbuffer_in;
char *textbuffer_out;

int main(int argc, char *argv[])
{

int linec;
printf("fceuxhaxx0r");
    if(argc >= 0 && argc < 2)
    {
        printf("Usage: %s fceuxdump.txt outdump.txt   ", argv[0]);
        return 0;
    }


    textbuffer_in = (char *)malloc(LINE_FCEUX_MAXLEN);
    textbuffer_out = (char *)malloc( (LINE_FCEUX_MAXLEN) - (LINE_STRIP_LENGTH-LINE_STRIP_OFFSET));

    infhandle = fopen(argv[1], "r");
    outfhandle = fopen(argv[2], "w");

    linec = 0;
    //read one line from the fceux dump file
    while(!feof(infhandle))
    {
    fread(textbuffer_in, LINE_FCEUX_MAXLEN, 1, infhandle);
    memcpy(textbuffer_out, textbuffer_in, LINE_STRIP_OFFSET);
    memcpy(textbuffer_out+LINE_STRIP_OFFSET, textbuffer_in+(LINE_STRIP_LENGTH+LINE_STRIP_OFFSET), LINE_FCEUX_MAXLEN - (LINE_STRIP_LENGTH + LINE_STRIP_OFFSET));
    textbuffer_out[strlen(textbuffer_out)-1] = 0x0; //NULL termination of string, otherwise diff goes bananas...
    fwrite(textbuffer_out, strlen(textbuffer_out), 1, outfhandle);
    linec++;
    }

    printf("%d lines converted...", linec);
    fclose(infhandle);
    fclose(outfhandle);



    return 0;
}
