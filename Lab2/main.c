#include <stdio.h>

char* text1 = "This is a string.";
char* text2 = "Yet another thing.";

int list1[20];
int list2[20];
int count = 0;

void copycodes(char* text, int* list, int* count) {
    
    while(*text != 0) {
        
        *list = *text; //sw    $t0,0($a1)
        
        text++; //addi    $a0,$a0,1
        list++; //addi    $a1,$a1,4
        
        (*count)++;
    }
}

void work() {
    
    copycodes(text1, list1, &count);
    copycodes(text2, list2, &count);
}

void printlist(const int* lst) {
    
    printf("ASCII codes and corresponding characters.\n");
    
    while(*lst != 0){
        printf("0x%03X '%c' ", *lst, (char)*lst);
        lst++;
    }
    printf("\n");
}

void endian_proof(const char* c) {
    
    printf("\nEndian experiment: 0x%02x,0x%02x,0x%02x,0x%02x\n",
           (int)*c,(int)*(c+1), (int)*(c+2), (int)*(c+3));
}

int main(void){
    
    work(text1, list1, count);
    
    printf("\nlist1: ");
    printlist(list1);
    printf("\nlist2: ");
    printlist(list2);
    printf("\nCount = %d\n", count);
    
    endian_proof((char*) &count);
}
