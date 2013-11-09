#include <stdio.h>
#include <stdlib.h>

int main();
int f(int);

int main(){
	int i,j,k;
	/*for(i = 0; i<100000000; i++){
        int x = i*293 + f(x);
		printf("%i",x);
	}
	puts("");*/
    for(i=0; i<1000; i++)
        for(j=0; j<100; j++)
            for(k=0; k<100; k++)
                printf("%i,%i,%i",i,j,k);
	return 0;
}

int f(int x){
    return x + x + x - x%2;
}
