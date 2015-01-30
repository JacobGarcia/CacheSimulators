#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define CACHESIZE 16
#define INDEX 4

struct cache{
    unsigned int address;
    char         state;
    unsigned int timer;
};

struct cache myCache[CACHESIZE];

enum values { VALID = 0, INVALID, EMPTY };

/**********************************************************************/
/* Name:        getAddr                                               */
/*                                                                    */
/* Description: This function will read the input file and return a   */
/*              decimal number. It will skip over comments, which     */
/*              begin the line with a #                               */
/*                                                                    */
/* Inputs:      The name of the input file                            */
/*                                                                    */
/* Outputs:     The address that will be presented to the cache       */
/**********************************************************************/
unsigned int getAddr(FILE *fp){

    char                 c;                         /* Character read */
    unsigned int         address;
    
    do {
        c = getc(fp);                           /* Get next character */
        if(c == '#')                              /* Skip the comment */
            do {
                c = getc(fp);
            } while(c != '\n');
    } while(!isxdigit(c) && !feof(fp));
    
    ungetc(c, fp);                /* Return the character to the file */
    fscanf(fp, "%x\n", &address);
            
    return (address);
}

/**********************************************************************/
/* Name:        cacheAccess                                           */
/*                                                                    */
/* Description: This function will simulate a fully associative cache */
/*                                                                    */
/* Inputs:      An address in decimal format                          */
/*                                                                    */
/* Outputs:     The number of hits                                    */
/**********************************************************************/
int cacheAccess(unsigned int address){
    unsigned int found = 0;  /* Boolean variable; it will point out if an address already
    exists in the entries*/
    static unsigned int fullCache = 0; /* Boolean variable; it will point out if the cache is 
    already full*/
    static unsigned int i = 0; /* Index counter*/
    static unsigned int hits = 0;
    static unsigned int timer = 0;

    for(int j = 0; j < CACHESIZE; j++)
    {
        /* Break the cycle if there is an invalid entry, due to the fact that is empty 
        and data could be stored on empty entries*/
       if(myCache[j].state == INVALID)
            j = CACHESIZE;
        /* If the contents of the entries pairs, increase the number of hits; mark the boolean
        found variable as true; update the timer variable because the entry where te address
        is found, is recently used*/
       else if(myCache[j].address == address){
                hits++;
                found = 1;
                myCache[j].timer = timer;
            }
    }

    /*If the address was not found in the entries, store it in the next entry*/
    if(found == 0){
        if(fullCache == 0) /*If the cache is not full, then it has invalid entries, 
        which have to change to valid entries*/
            myCache[i].state = VALID;
        /* Store the address, the timer and increase the index counter*/
        myCache[i].address = address;
        myCache[i].timer = timer;
        i++;
    }

    /*If the index counter is equals to the CACHESIZE that means that the cache is full; 
    in order to store more data into the cache, the index counter is reinitialized*/
    if(i == CACHESIZE){
        i = 0;
        fullCache = 1;
    }

    /* Time increase over time*/
    timer++;
    
    return hits;
}

/**********************************************************************/
/* Name:        decimalToHex                                          */
/*                                                                    */
/* Description: This function will transform an address in decimal    */
/*              format into hexadecimal format                        */  
/*                                                                    */
/* Inputs:      An array; An address in decimal format                */
/*                                                                    */
/* Outputs:     An address in hexadecimal format                      */
/**********************************************************************/
void decimalToHex(int hexAddress[], int address){

    int i = INDEX;
    while(i > 0){
        hexAddress[i] = address % 16; 
        i--;
        address = address/16;
    }
}

void entries(int i){

    /* Declare a temporal array*/
    int temp[INDEX];

    /* Create an array with only 0's*/
    for(int j = 0; j < INDEX; j++)
        temp[j] = 0;

    /* Print over the same line as the 0's array depending on the length of the entry number*/
        for(int j = 0; j < INDEX; j++)
            printf("%d", temp[j]);
        if(i < 10) 
            printf("\b%d", i);
        else if(i < 100)
            printf("\b\b%d", i);
        else if(i < 1000)
            printf("\b\b\b%d", i);
        else if(i < 10000)
            printf("\b\b\b\b%d", i);
}

/**********************************************************************/
/* Name:        header                                                */
/*                                                                    */
/* Description: This function will print the header structure of the  */
/*              table which presents the information of the cache     */  
/*                                                                    */
/* Inputs:      NONE                                                  */
/*                                                                    */
/* Outputs:     NONE                                                  */
/**********************************************************************/
void header(){
    printf("-----------------------------------------\n|        FULLY ASSOCIATIVE CACHE        |\n|                                       |\n-----------------------------------------\n");
    printf("| ENTRY | ADDRESS | STATUS  | LAST USED |\n");
    printf("-----------------------------------------\n");
}

/**********************************************************************/
/* Name:        cacheStatistics                                       */
/*                                                                    */
/* Description: This function will print the number hits, misses and  */
/*              and the hit rate                                      */  
/*                                                                    */
/* Inputs:      The number of hits, misses, and the hit rate          */
/*                                                                    */
/* Outputs:     NONE                                                  */
/**********************************************************************/
void cacheStatistics(int hits, int events, float hitRate){
    printf("-----------------------------------------\n");
    printf("|   HITS    |   MISSES   |   HIT RATE   |\n");
    printf("-----------------------------------------\n");
    printf("|    %2d     |     %2d     | p = %.6f |\n", hits, events-hits, hitRate);
    printf("-----------------------------------------\n\n");
}

/**********************************************************************/
/* Name:        printCache                                            */
/*                                                                    */
/* Description: This function will build and print all the            */
/*              information about the cache                           */  
/*                                                                    */
/* Inputs:      NONE                                                  */
/*                                                                    */
/* Outputs:     NONE                                                  */
/**********************************************************************/
void printCache(){

    header(); /* Print the basic header of the table*/
    for(int i = 0; i < CACHESIZE; i++){

        printf("| ");
        entries(i); /* Print the entry (first) column*/
        printf("  | ");

        /* Load the address of the entry*/
        int address = myCache[i].address;

        /* Declare an array that will contain the address in hex format */
        int hexAddress[INDEX];
     
        /* Call decimalToHex() to initialise hexAddress[]; pass array name, itself a pointer and
        the self address in decimal format*/
        decimalToHex(hexAddress, address);

        /* Print the address (second) column*/
        for(int j = 1; j <= INDEX; j++){
            switch(hexAddress[j]){
              case 10:
                  printf("A");
                  break;
              case 11:
                  printf("B");
                  break;
              case 12:
                  printf("C");
                  break;
              case 13:
                  printf("D");
                  break;
              case 14:
                  printf("E");
                  break;
              case 15:
                  printf("F");
                  break;
              default :
                printf("%d", hexAddress[j]);
            }
        }

        /* Print the state (third) column*/
        if(myCache[i].state == VALID)
            printf("    | VALID   |");
        else
            printf("    | INVALID |");

        /* Print the "last used" (fourth) column*/
        printf("  %8d |\n", myCache[i].timer);
    }

    /* Print the last border of the table*/
    printf("-----------------------------------------\n\n");
}

int main(){

    unsigned int events = 0;
    static unsigned int hits = 0;

    FILE         *fp;                    /* Pointer to the input file */
    unsigned int ref;
    
    /* Initialize the cache */
    for (int i = 0; i < CACHESIZE; i++){
        myCache[i].state = INVALID;
        myCache[i].address = EMPTY;
    }
    
    fp = fopen("trace.txt", "r");        /* Open file for read operation */

    while(!feof(fp)){
        ref = getAddr(fp);
        hits = cacheAccess(ref);
        events++;
    }

    /* Print the cache final state & information*/
    system("cls");
    printCache();
    float hitRate = (hits/(float)events);

    /* Print the cache statistics*/
    cacheStatistics(hits, events, hitRate);
    system("pause");
}