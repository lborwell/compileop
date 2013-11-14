#include <stdio.h>
#include <stdlib.h>

int main();
int fib(int);

int main(){
    printf("%i",fib(45));
    return 0;
}

int fib(int n){
    return n<2?n:fib(n-1) + fib(n-2);
}
