#include <stdio.h>
#include <stdlib.h>

int main();
int f(int);

int main(){
	int i;
	for(i = 0; i<100000000; i++){
        int x = i*293 + f(x);
		printf("%i",x);
	}
	puts("");
	return 0;
}

int f(int x){
    return x + x + x - x%2;
}
