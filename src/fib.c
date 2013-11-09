#include <stdio.h>
#include <stdlib.h>

int main();
int fib(int);

int main(){
    //printf("%i",fib(30));
    printf("%i",fib(45));
    return 0;
}

int fib(int n){
    if(!n) return 0;
    if(n==1) return 1;
    return fib(n-1) + fib(n-2);
}
