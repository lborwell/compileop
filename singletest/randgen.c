#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int main();


int arrlen=1000000;
int main(){

    FILE* outf = fopen("randarr.h","w");

    srand(time(NULL));
    int a[arrlen];
    int i;
    for(i=0; i<arrlen; i++)
        a[i] = rand();

    fprintf(outf,"int arrlen=%i;",arrlen);
    fprintf(outf,"int randa[] = {");
    for(i=0; i<arrlen-1; i++)
        fprintf(outf,"%i,",a[i]);
    fprintf(outf,"%i};",a[arrlen-1]);
    fclose(outf);
}
