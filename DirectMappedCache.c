#include <stdio.h>
#include <string.h>
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

enum values { VALID = 0, INVALID, EMPTY};

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

    char	             c;	   	                    /* Character read */
    unsigned int         address;
    
    do {
        c = getc(fp);	                        /* Get next character */
        if(c == '#')	                          /* Skip the comment */
            do {
                c = getc(fp);
            } while(c != '\n');
    } while(!isxdigit(c) && !feof(fp));
    
    ungetc(c, fp);                /* Return the character to the file */
    fscanf(fp, "%x\n", &address);
            
    return (address);
}

/**********************************************************************/
/* Name:        decimalToBinary                                       */
/*                                                                    */
/* Description: This function will transform an address or a number   */
/*              in decimal format into binary format                  */  
/*                                                                    */
/* Inputs:      An array; An address or number in decimal format      */
/*                                                                    */
/* Outputs:     A number in binary format                             */
/**********************************************************************/
void decimalToBinary(int binaryIndex[], int address){

    int i = INDEX - 1;
    while(i >= 0){
        binaryIndex[i] = address % 2; 
        i--;
        address = address/2;
    }
}

/**********************************************************************/
/* Name:        cacheAccess                                           */
/*                                                                    */
/* Description: This function will simulate a direct mapped cache     */
/*                                                                    */
/* Inputs:      An address in decimal format                          */
/*                                                                    */
/* Outputs:     The number of hits                                    */
/**********************************************************************/
int cacheAccess(unsigned int address){
    
    static unsigned int hit = 0;
    static unsigned int timer = 0;
 
    /* Declare an array with the last 4 binary digits of the address*/
    int binaryIndex[INDEX];
 
    /* Call decimalToBinary() to initialise binaryIndex[]; pass array name, itself a pointer and
    the self address in order to get the last 4 binary digits*/
    decimalToBinary(binaryIndex, address);

    /* Get the index of the cache where the address it is going to be allocated */
    int i = -1;
    int found = -1;
    while((i < CACHESIZE)&&(found != 0)){
        i++;

        /* Declare a temporal array*/
        int temp[INDEX];
        
        /* Create a temporal array with the number of the entry*/
        decimalToBinary(temp, i);

        /* Compare the last 4 binary digits of the address with the cache entrys */
        found = memcmp(temp, binaryIndex, sizeof(temp));
    }

    /* Check the state of the cache entry; if the entry is not valid, change the state
        and store the address of the entry */
    if(myCache[i].state == INVALID){
        myCache[i].state = VALID;
        myCache[i].address = address;
    }
    else{
        /* If the contents pairs, increase the number of hits*/
        if(myCache[i].address == address)
            hit++;
        else
            myCache[i].address = address; /* If the content does not
            pair, replace the old address with the new one*/
    }
    
    /* Timer Assignation; increase it over timer*/
    myCache[i].timer = timer;
    timer++;
    return hit;
}

/**********************************************************************/
/* Name:        decimalToHex                                          */
/*                                                                    */
/* Description: This function will transform an address or a number   */
/*              in decimal format into hexadecimal format             */  
/*                                                                    */
/* Inputs:      An array; An address in decimal format                */
/*                                                                    */
/* Outputs:     An address in hexadecimal format                      */
/**********************************************************************/
void decimalToHex(int hexAddress[], int address){

    int i = INDEX;
    /* The last digit is ignored, instead a zero is placed at the begining of the 
    number, due to the fact that the entry in which is contained, already 
    gives the information that we need from the last digit*/
    hexAddress[0] = 0;
    while(i > 0){
        hexAddress[i] = address % 16; 
        i--;
        address = address/16;
    }
}

/**********************************************************************/
/* Name:        entries                                               */
/*                                                                    */
/* Description: This function will print the first column of the      */
/*              cache, which basically is the entry number            */  
/*                                                                    */
/* Inputs:      An integer counter                                    */
/*                                                                    */
/* Outputs:     NONE                                                  */
/**********************************************************************/
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
    printf("-----------------------------------------\n|          DIRECT MAPPED CACHE          |\n|                                       |\n-----------------------------------------\n");
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
        for(int j = 0; j < INDEX; j++){
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
    static unsigned int hit = 0;

    FILE         *fp;                    /* Pointer to the input file */
    unsigned int ref;
    
    /* Initialize the cache */
    for (int i = 0; i < CACHESIZE; i++){
        myCache[i].state = INVALID;
        myCache[i].address = EMPTY;
    }
    
    fp = fopen("trace.txt", "r");		 /* Open file for read operation */

    while(!feof(fp)){
        ref = getAddr(fp);
        hit = cacheAccess(ref);
        events++;
    }

    /* Print the cache final state & information*/
    system("cls");
    printCache();
    float hitRate = (hit/(float)events);

    /* Print the cache statistics*/
    cacheStatistics(hit, events, hitRate);
    system("pause");
}