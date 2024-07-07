#include <stdio.h>
#include "draw.h"
#include "clean.h"

int main(void) {
    int sum = 0;
    int maxLength = 0;
    int i;
    int j;
    int k;
    int m;
    int p;
    int l;
    /*
    FILE *filePointer;
    char fileName[30];
    char fileName[] = "in4.txt";
    */
    char line[MAX_LINE_LENGTH];
    int sums[MAX_ROWS];
    int occurences[1081];
    int intervaledArray[540];
    int rowNum = 0;
    int headerCellCount = 0;
    
    /*
    scanf("%s", fileName);
    filePointer = fopen(fileName, "r");
    fseek(filePointer, 0, SEEK_SET);
    */

    fgets(line, MAX_LINE_LENGTH, stdin);
    headerCellCount = countHeaderCells(line);


    while (fgets(line, MAX_LINE_LENGTH, stdin) != NULL) {
        if (containsQuotationMarks(line)) {
            printf("./hist: Unsupported quotes\n");
            return 2;
        }
        sum = sumRow(line, headerCellCount);
        if (sum == -1) {
            return 1; 
        }
        sums[rowNum] = sum;
        rowNum++;
    }


    /* 
    printf("Line: %s, Sum: %d, Row num: %d\n", line, sum, rowNum);
    prints original array with just sum values 
    
    for (i = 0; i < rowNum; i++) {
        printf( "Row sums element is: %d\n", sums[i]);
    }
    */

     /* zeroes out all values in occurences and intervaledArray */
    for (p = 0; p < 1081; p++){
        occurences[p] = 0;
    }
    for (l = 0; l < 540; l++){
        intervaledArray[l] = 0;
    }

    /* reorganizes sums array into occurances by number of occurances of index value */
    for (j = 0; j < rowNum; j++){
        occurences[(89 + sums[j])] += 1;
        /* printf("89 + value in i is :%d\n", sums[j]); */
    }
    
    /* reorganizes occurences array by combining into intervals of 2 */
    for (m = 0; m < 540; m++){
        intervaledArray[m] = (occurences[2*m] + occurences[(2*m)+1]);
    }
    

    /* prints new array from negative value range */
    for (k = 0; k < 90; k++) {
        /* printf( "Occurences element is: %d\n", occurences[k]); */
    }

    /* finds the max index that a value is actually there in */
    for (i = 0; i < 540; i++){
        if (intervaledArray[i] != 0){
            maxLength = i;
        }
    }
    if ((maxLength % 2) != 0){
        maxLength++;
    }


    /*
    for (j = 0; j <= rowNum; j++){
        printf("This is the %d iteration", j);
        printf("Sums elemnt is: %d\n", sums[j]);
        occurences[(sums[j]+1)/2] += 1;
        for (i = 1; i < rowNum; i++) {
            printf( "Occurences element is: %d\n", occurences[i+1]);
        }
    }

    printf("Sums of numbers in each row:\n");
    "%d\n", occurences[i]
    printf("Sums elemnt is: %d\n", sums[j]);
    */

    while(((maxLength % 5) != 0)){
        maxLength++;
    }

    drawHashtags(intervaledArray, maxLength);  
    
    
    return 0;
}

