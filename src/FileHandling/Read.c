#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char* argv[]) {
    FILE* fp;
    fp = fopen(argv[1], "r");
    char lineBuffer[250];

    while (!(feof(fp))) {
        fgets(lineBuffer, 250, fp);
        puts(lineBuffer);
    }

    fclose(fp);
}
