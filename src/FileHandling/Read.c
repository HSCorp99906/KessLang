#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char* argv[]) {
    FILE* fp;
    fp = fopen(argv[1], "r");
    char lineBuffer[150];

    int lineMatch = 0;
    int lineCount = 0;

    while (!(feof(fp))) {
        fgets(lineBuffer, 150, fp);
        ++lineMatch;
    }

    fclose(fp);

    fp = fopen(argv[1], "r");

    while (!(feof(fp)) && lineCount < lineMatch - 1) {
        fgets(lineBuffer, 150, fp);
        puts(lineBuffer);
        ++lineCount;
    }
}
