#include <stdio.h>
#include <stdlib.h>
#include "randarr.h"

int main();
void parr(int[],int);
void qqsort(int[],int,int);
int split(int[],int,int);

int main(){
    qqsort(randa,0,arrlen-1);
    parr(randa,arrlen);

    return 0;
}

void parr(int a[], int len){
    int i;
    for(i=0; i<len; i++)
        printf("%i ", a[i]);
}

void qqsort(int a[], int l, int r){
    int j;

    if(l<r){
        j = split(a, l, r);
        qqsort(a, l, j-1);
        qqsort(a, j+1, r);
    }
}

int split(int a[], int l, int r){
    int pivot, i, j, t;
    pivot = a[l];
    i = l; j = r+1;

    while(1){
        do i++; while(a[i] <= pivot && i <= r);
        do --j; while(a[j] > pivot);
        if(i >= j) break;
        t = a[i];
        a[i] = a[j];
        a[j] = t;
    }
    t = a[l];
    a[l] = a[j];
    a[j] = t;
    return j;
}
