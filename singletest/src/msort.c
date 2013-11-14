#include <stdio.h>
#include <stdlib.h>
#include "randarr.h"

int main();
void merge(int[], int, int, int);
void msort(int[], int, int);
void parr(int[],int);

int main(){
    msort(randa,0,arrlen-1);
    parr(randa,arrlen);
    return 0;
}

void parr(int a[], int len){
    int i;
    for(i=0; i<len; i++)
        printf("%i ", a[i]);
}

void msort(int a[], int low, int high){
    int mid;

    if(low < high){
        mid = (low + high)/2;
        msort(a, low, mid);
        msort(a, mid+1, high);
        merge(a, low, mid, high);
    }
}

void merge(int a[], int low, int mid, int high){
    int i,m,k,l,temp[arrlen];

    l = low;
    i = low;
    m = mid+1;

    while((l <= mid) && (m <= high)){
        if(a[l] <= a[m]){
            temp[i] = a[l];
            l++;
        }else{
            temp[i] = a[m];
            m++;
        }
        i++;
    }

    if(l > mid){
        for(k=m; k<=high; k++){
            temp[i] = a[k];
            i++;
        }
    }else{
        for(k=l; k<=mid; k++){
            temp[i] = a[k];
            i++;
        }
    }

    for(k=low; k<=high; k++)
        a[k] = temp[k];
}
