#include <stdio.h>

int main();
int fib(int);

int main(){
    printf("%i",fib(100000000));
    return 0;
}

int fib(int n){
    if(!n)
        return n;
    int a = 1;
    int b = 1;

    int i;
    for(i=3; i<=n; i++){
        int c = a+b;
        a=b;
        b=c;
    }
    return b;
}
