#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
    int i = 0;
    long l = 0;
    double d = 0;

    int n = argv[1][0];
    switch(n){
        case 'i':
            fread(&i, sizeof(int), 1, stdin);
            printf("%d\n", i);
            break;
        case 'l':
            fread(&l, sizeof(long), 1, stdin);
            printf("%ld\n", l);
            break;
        case 'd':
            fread(&d, sizeof(double), 1, stdin);
            printf("%f\n", d);
            break;
    }
    return 0;
}
