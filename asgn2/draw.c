#include <stdio.h>
#include "draw.h"


int printXaxis(int len, int minIndex){
    int j;
    int k;
    int i;
    int totalLen;
    
    minIndex++;
    while ((minIndex % 5) != 0){
        minIndex--;
    }

    while ((len % 5) != 0){
        len++;
    }
    totalLen = len-minIndex;
    /* highest val for x axis 
    k = len;
    while (!(set[k] && set[k])){
        max = max - 2;
        k = k - 2;
    }

    printf("This is maxNums:%d", len);
    printf("this is len: %d", len);
    */

    printf("    +-|");
    for (j = 0; j < totalLen/5; j++){
        printf("----|");
    }
    printf("-+\n");


    /* appropriate spacing based on how many digits x axis number is */
    totalLen = totalLen/5;
    printf("  ");
    minIndex = minIndex/5;
    /* printf("MinIndex:%d, totalLen:%d\n", minIndex, totalLen);*/
    if(minIndex < 9){
        for (i = minIndex; i < 9 && totalLen; i++){
            printf("  -%d",(9-i)*10);
            totalLen--;
        }
        minIndex = i;
    }
    if( minIndex == 9 && totalLen >= 0){
        printf("    %d", 0);
        totalLen--;
        minIndex++;
    }
    
    while(minIndex > 9 && totalLen >= 0){
        printf("   %d", (minIndex - 9) * 10);
        totalLen--;
        minIndex++;
    }
    while(minIndex > 18 && totalLen >= 0){
        printf("  %d", ((minIndex - 17) * 10) + 100);
        totalLen--;
        minIndex++;
    }
    for (k = 10; k <= totalLen; k+=10){
        printf("   %d", k);
    }
    printf("\n");

    
    return 0;
}


int drawHashtags(int set[], int length) {
    int k = 0;
    int i;
    int j;
    int m = 0;
    int maxY = 0;
    int minIndex = 0;
    int length2 = 0;

    /*  
    for (count = 9; count >= 0; count--){
        printf("This is set:%d", set[count]);
    }
    */

    /* finds max val and rounds up to multiple of 5 so can set Y axis accordingly */
    for(k = 0; k <= length; k++){
        if (set[k] > maxY){
            maxY = set[k];
        }
    }
    while ((maxY % 5) != 0){
        maxY++;
    }



    /* find what the first non zero value in the array is you know where to start printing */
    while (set[m] == 0){
        m++;
    }
    m++;
    while ((m % 5) != 0){
        m--;
    }
    m--;
    minIndex = m;

    

    /* the i represents maximum value for y axis*/
    printf("    | ");
    for(length2 = (length-minIndex); length2 >= 0; length2--){
        printf(" ");
    }
    printf("|\n");

    for(i = maxY; i > 0; i--){
        /* formatting */
        if (i % 5 == 0){
            if (i == 5){
                printf("  %d T ", i);
            }else{
                printf(" %d T ", i);
            }
        }else{
            printf("    | ");
        }
        

        /* printing hashtags */
        for (j = minIndex; j < length; j++){
            
            if ((set[j]) == i){
                printf("#");
                set[j]--;

            }else{
                printf(" ");
            }

        }

        /* formatting */
        if (i % 5 == 0){
            printf(" T %d", i);
        }else{
            printf(" |");
        }
            
        printf("\n");
    }
    minIndex++;
    printXaxis(length, minIndex);

    return 0;
}






/* finds lowest and highest val for Y axis
int YaxisBounds(int set[], int len){
    minIndex = set[0];
    max = set[0];
    for (i = 0; i < len; i++){
        if (set[i] > max){
            max = set[i];
        }
        if (set[i] < minIndex){
            max = set[i];
        }
    }
}



int main(void) {
    int testNums[] = {0, 0, 0, 0, 0, 0, 0, 2, 11, 7, 0, 5, 3, 1, 0, 0, 0, 0, 0};
    drawHashtags(testNums, 19);  

}
*/
