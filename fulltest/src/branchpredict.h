#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct pnode{
    char* addr;
    int taken;
    int total;
    struct pnode *next;
} profNode;

typedef struct b2node{
	int addr;
	int h;
	int l;
	struct b2node *next;
} b2Node;

typedef struct cnode{
	int addr;
	b2Node* oo;
    b2Node*	ol;
    b2Node*	lo;
    b2Node*	ll;
	struct cnode *next;
} cNode;

int check(b2Node*, int*);
b2Node* createTable();
b2Node* initNode();
char* getFileName();
void openTraceFile(char*);
void closeTraceFile(char*);
void alwaysTaken();
void neverTaken();
int takeBranch(int*, int);
void bitPredict(int, b2Node**, int*);
float getAccuracy(int *);
float getMispred(int *);
void printS(char*, int*);
void buildProfile();
void printProfile();
void profilePredict();
void correlatePredict(cNode**,int,int*);
void writeProfile();
b2Node* createB2Node(int);
cNode* createCNode();
int updateBits(b2Node* , int* );
void freeCNode(cNode*);
void printR(char*);