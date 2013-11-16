#include "branchpredict.h"

FILE* branchTrace;  //input file
FILE* output;       //results file
char* addr;         //current branch address
int tableIndex;     //int substring of branch address
int taken;          //was real branch taken?

int totalCount;     //total branch operations

int glbH;           //correlating predictor, global histories
int glbL;

//counters for each predictor
int alwaysTCorr, neverTCorr;
int bit512Corr, bit1024Corr, bit2048Corr, bit4096Corr;
int profCorr;
int c512Corr,c1024Corr,c2048Corr,c4096Corr;

//head of profile
profNode* profile;

//2-bit branch predictor history tables
b2Node* ssTable = NULL;
b2Node*  sTable = NULL;
b2Node*  mTable = NULL;
b2Node*  lTable = NULL;

//correlating predictor history tables
cNode* cssTable = NULL;
cNode* csTable = NULL;
cNode* cmTable = NULL;
cNode* clTable = NULL;

int main(int argc, char **argv){
    output = fopen("esults.csv","a");

    srand(time(NULL));
    alwaysTCorr = neverTCorr = totalCount = bit512Corr = bit1024Corr = bit2048Corr = bit4096Corr = profCorr = c512Corr = c1024Corr =c2048Corr = c4096Corr = 0;
    glbH=0;
    glbL=0;

    addr = malloc(16);
    taken = 0;

    //handle arguments
    //if one argument, assume it is input file name
    //if more than one, fail
    //if none, ask user for name manually
    char* inFile;
    if(argc==1)
        inFile = getFileName();
    else if(argc>2){
        printf("Please enter file name as only argument, or call with 0 arguments.\n");
        return;
    }
    else
        inFile = argv[1];
    printf("Opening file: %s\n",inFile);
    
    //file is opened, read, then closed to build profile
    //maybe not perfect, but what you gonna do?
    openTraceFile(inFile);
    buildProfile();
    writeProfile();
    closeTraceFile(inFile);

    //open file for prediction
    openTraceFile(inFile);

    while(fscanf(branchTrace,"%s %i",addr,&taken)==2){
        //convert string to integer for branch table index
        //currently converts last 6 characters of address, might do with
        //being less
        char tmp[7];
        memcpy(tmp, &addr[strlen(addr)-6],7);
        tableIndex = strtol(tmp, NULL, 10);
//        char tmp[4];
//        memcpy(tmp, &addr[strlen(addr)-3],4);
//        tableIndex = strtol(tmp, NULL, 10);

        totalCount++;
        alwaysTaken();
        neverTaken();
        bitPredict(512, &ssTable, &bit512Corr);
        bitPredict(1024, &sTable, &bit1024Corr);
        bitPredict(2048, &mTable, &bit2048Corr);
        bitPredict(4096, &lTable, &bit4096Corr);
        correlatePredict(&cssTable, 512, &c512Corr);
        correlatePredict(&csTable, 1024, &c1024Corr);
        correlatePredict(&cmTable, 2048, &c2048Corr);
        correlatePredict(&clTable, 4096, &c4096Corr);
        profilePredict();

        //update global history registers for correlation. Probably terrible
        //place to do this, but it works
        glbH = glbL;
        glbL = taken;
    }
    //send results to file
    printR(inFile);

    //print results to screen
    printf("%-20s  %8s\t%6s\t%6s\n","Predictor name","#Correct","Corr\%","Mispr\%");
    printS("Always taken",&alwaysTCorr);
    printS("Never taken",&neverTCorr);
    printS("512 table predict",&bit512Corr);
    printS("1024 table predict",&bit1024Corr);
    printS("2048 table predict",&bit2048Corr);
    printS("4096 table predict",&bit4096Corr);
    printS("Profiled",&profCorr);
    printS("512 Corr predict",&c512Corr);
    printS("1024 Corr predict",&c1024Corr);
    printS("2048 Corr predict",&c2048Corr);
    printS("4096 Corr predict",&c4096Corr);

    printf("%-20s| %7i\n","Total count", totalCount);
    closeTraceFile(inFile);
    fclose(output);
    return 0;
}

//given predictor name and result variable, print results string to screen
void printS(char* title, int* corr){
    printf("%-20s| %7i\t%2.2f%%\t%2.2f%%\n",title,*corr,getAccuracy(corr),getMispred(corr));
}

//minimise code reuse
float getMispred(int *inp){
    return 100-getAccuracy(inp);
}

//print results to csv file
void printR(char* fname){
    fprintf(output, "%s\n", fname);
    fprintf(output, "always,never,512-2,1024-2,2048-2,4096-2,profiled,512c,1024c,2048c,4096c\n");
    fprintf(output, "%2.2f,%2.2f,%2.2f,%2.2f,%2.2f,%2.2f,%2.2f,%2.2f,%2.2f,%2.2f,%2.2f\n",
        getMispred(&alwaysTCorr),getMispred(&neverTCorr),getMispred(&bit512Corr),getMispred(&bit1024Corr),getMispred(&bit2048Corr),getMispred(&bit4096Corr),
        getMispred(&profCorr),getMispred(&c512Corr),getMispred(&c1024Corr),getMispred(&c2048Corr),getMispred(&c4096Corr));
}

//PREDICTOR TIME

//trivial
void alwaysTaken(){
    static int take = 1;
    takeBranch(&alwaysTCorr, take);
}

//trivial
void neverTaken(){
    static int take = 0;
    takeBranch(&neverTCorr, take);
}

//2-bit prediction. Takes table, length of table and coutner variable
void bitPredict(int tableLen, b2Node **table, int *count){
    //iterator
    b2Node *prev;
    int i;

    //if this is the first time, init head
    if(!(*table))
        *table = createB2Node(1);
    b2Node *t = *table;
    for(i=0; i<tableLen; i++){
        //if table is shorter than limit
        if(!t) break;

        //test current node
        if(checkb2(t,count))
            return;
        //iterate
        prev = t;
        t = t->next;
    }

    //haven't found match, create new node and add to end
    b2Node *newN = createB2Node(1);
    prev->next = newN;

    //if table is at length limit, move head to next element. FIFO table
    if(i==tableLen){
        b2Node *p = *table;
        *table = (*table)->next;
        free(p);
    }

    //predict branch not taken
    takeBranch(count, 0);
}

//correlate predict, same arguments as 2-bit predict
void correlatePredict(cNode **table, int tableLen, int *count){
    //iterators
    cNode *prev;
    int i;

    //if no head, create one
    if(!(*table))
        *table = createCNode(1);

    cNode *t = *table;
    for(i=0; i<tableLen; i++){
        //if end of table, break
        if(!t) break;

        //if current address matches, index table row by global registers
        //and 2-bit predict
        if(t->addr == tableIndex){
            if(!glbH && !glbL){
                updateBits(t->oo,count);
            }
            else if(!glbH && glbL){
                updateBits(t->ol,count);
            }
            else if(glbH && !glbL){
                updateBits(t->lo,count);
            }
            else if(glbH && glbL){
                updateBits(t->ll,count);
            }
            return;
        }
        //iterate
        prev = t;
        t = t->next;
    }

    //haven't found matching row, create one
    cNode *newN = createCNode(1);
    prev->next = newN;

    //predict not taken
    updateBits(newN->oo,count);

    //if table is max length, cut off head. FIFO operation.
    if(i==tableLen){
        cNode *p = (*table);
        (*table)= (*table)->next;
        freeCNode(p);
    }
}

int checkb2(b2Node* t, int* count){
    if(t->addr == tableIndex){
        updateBits(t,count);
        return 1;
    }
    return 0;
}

//2-bit predict logic. Finite state machine with states 00,01,10,11.
//00,01 predict not taken, 10,11 predict taken.
int updateBits(b2Node* t, int* count){
    if(t->h == 1){
        if(takeBranch(count,1)){
            t->l = 1;
            return 1;
        }else{
            if(t->l == 1)
                t->l = 0;
            else
                t->h = 0;
        }
    }else{
        if(takeBranch(count,0))
            t->l = 0;
        else{
            if(t->l == 0)
                t->l = 1;
            else
                t->h = 1;
            return 1;
        }
    }
    return 0;
}


void buildProfile(){
    if(fscanf(branchTrace,"%s %i",addr,&taken)==2){
        profile = malloc(sizeof(profNode));
        profile->addr = strdup(addr);
        profile->taken = taken;
        profile->total = 1;
        profile->next = NULL;

        char tmp[7];
        memcpy(tmp, &addr[strlen(addr)-6],7);
        tableIndex = strtol(tmp, NULL, 10);
    }
    while(fscanf(branchTrace,"%s %i",addr,&taken)==2){
        profNode* x = profile;
        profNode* prev;
        while(x){
            if(!strcmp(x->addr,addr)){
                x->taken = taken? x->taken + 1 : x->taken;
                x->total++;
                break;
            }
            prev = x;
            x = x->next;
        }
        if(!x){
            profNode* n = malloc(sizeof(profNode));
            n->addr = strdup(addr);
            n->taken = taken;
            n->total = 1;
            n->next = NULL;
            prev->next = n;
        }
    }
}

void profilePredict(){
    profNode* x = profile;
    while(x){
        if(!strcmp(x->addr, addr)){
            float chanceTaken = (float)x->taken / (float)x->total;
            if((rand()/(double)RAND_MAX) < chanceTaken)
                takeBranch(&profCorr,1);
            else
                takeBranch(&profCorr,0);
            break;
        }
        x = x->next;
    }
}

void writeProfile(){
    FILE* f = fopen("profile.txt","w");
    if(f){
        profNode *n = profile;
        while(n){
            fprintf(f,"%s\n",n->addr);
            n = n->next;
        }
    }
    fclose(f);
}

b2Node* createB2Node(int giveTableIndex){
    b2Node *n = (b2Node*)malloc(sizeof(b2Node));
    n->addr = giveTableIndex? tableIndex : 0;
    n->h = 0; n->l = 0;
    n->next = NULL;
    return n;
}

cNode* createCNode(int giveTableIndex){
    cNode* n = (cNode*)malloc(sizeof(cNode));
    n->addr = giveTableIndex? tableIndex : 0;
    n->oo = createB2Node(0);
    n->ol = createB2Node(0);
    n->lo = createB2Node(0);
    n->ll = createB2Node(0);
    n->next = NULL;
    return n;
}

void freeCNode(cNode *n){
    free(n->oo);
    free(n->ol);
    free(n->lo);
    free(n->ll);
    free(n);
}

float getAccuracy(int *count){
    return (float)(*count) / (float)totalCount * 100;
}

int takeBranch(int *ccount, int prediction){
    if(taken == prediction){
        (*ccount)++;
        return 1;
    }
    return 0;
}

void openTraceFile(char* inFile){
    branchTrace = fopen(inFile,"r");
    if(!branchTrace){
        printf("%s failed to open.\n",inFile);
        return;
    }
    printf("%s open for read.\n",inFile);
}

void closeTraceFile(char* inFile){
    if(branchTrace != NULL){
        fclose(branchTrace);
        printf("%s closed.\n",inFile);
    }
}

char* getFileName(){
    char* line = (char*)malloc(45);
    char c;
    int i=-1;
    printf("Please enter file name: ");
    while((c=getchar()) != '\n')
        line[++i]=c;
    line[++i]='\0';
    return line;
}
