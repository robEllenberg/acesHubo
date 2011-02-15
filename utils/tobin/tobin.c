#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
    int i = 0;
    long l = 0;
    double d = 0;

    int n = argv[1][0];
    switch(n){
        case 'i':
            i = atoi(argv[2]);
            fwrite(&i, sizeof(int), 1, stdout);
            break;
        case 'l':
            l = atol(argv[2]);
            fwrite(&l, sizeof(long), 1, stdout);
            break;
        case 'd':
            d = atof(argv[2]);
            fwrite(&d, sizeof(double), 1, stdout);
            break;
    }
    return 0;
}

