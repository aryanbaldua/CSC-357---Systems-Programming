#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freq.h"
#include "dict.h"

Dict *dct;

int toLowercase(char *str) {
    int i;
    int j = 0;
    for (i = 0; str[i]; i++) {
        /* printf("in lower case %c\n",str[i]); */
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[j++] = str[i] + ('a' - 'A'); 
            continue;
        }
        if ((str[i] >= 'a' && str[i] <= 'z') || str[i] == ' ') {
            /* printf("in lower case if %c\n",str[i]); */
            str[j++] = str[i]; 
            continue;
        }
        if ( str[i] == '\t' || str[i] == '\n' || str[i] == '\v'|| str[i] == '\f' || str[i] == '\r') {
            str[j++] = ' '; 
            continue;
        }     
    }
    str[j] = '\0';
    return 0;
}

int wordsIntoDictionary(char *text, Dict *myDictionary){
    char* token;
    void *value;
    long valInsert;

    toLowercase(text);
    token = strtok(text, " ");

    while (token != NULL) {
        /* printf("Token is:%s\n", token); */

        value = dctget(myDictionary, token);
        if (value != NULL){
            valInsert = (long)dctget(myDictionary, token);
            /* printf("This is the value from dct %ld\n", valInsert); */
            valInsert++;
        }
        else{
            valInsert = 1;
        }
        /* printf("This is the value being inserted %ld\n", valInsert); */
        dctinsert(myDictionary, token, (void *)valInsert);
        token = strtok(NULL, " ");
    } /* end of while */    
    return 0;
}

char *readTextfile(char *textFileName) {
    char *text = NULL;
    char buffer[1024]; 
    int text_size = 0;

    FILE *file = fopen(textFileName, "r");
    if (!file) {
        return NULL;
    }

    while (fgets(buffer, sizeof(buffer), file)) {
        int buffer_len = strlen(buffer);
        /* Reallocate memory to accommodate the new text */
        char *new_text = realloc(text, text_size + buffer_len + 1);
        text = new_text;
        strcpy(text + text_size, buffer); 
        text_size += buffer_len;
    }
    fclose(file);
    return text;
}

/*
int getTextFileName(char *programname,char *fileName, char *textFileName) {
    FILE *file;
    char *token;
    
    file = fopen(fileName, "r");
    
    if (file == NULL) {
        printf("%s: No such file or directory\n",programname);
        return 1;
    }
    if (fgets(textFileName, 500, file) == NULL) {
        if (!feof(file)) {
            printf("%s: fgets return error\n",programname);
            fclose(file);
            return 1;
        }
    }
    token = strtok(textFileName, " ");
    if (token == NULL) {
        printf("%s: Too few arguments\n",programname);
        fclose(file);
        return 1;
    }
    token = strtok(NULL, " ");
    if (token != NULL) {
        printf("%s: Too many arguments\n",programname);
        fclose(file);
        return 1;
    }
    fclose(file);
    return 0;
}
*/

int compareNodes(const void *a, const void *b) {
    long valA;
    long valB;

    char *ptrA = *(char **)a;
    char *ptrB = *(char **)b;
    
    valA = (long)dctget(dct, ptrA);
    valB = (long)dctget(dct, ptrB);

    if (valA < valB) return 1;
    if (valA > valB) return -1;
    return strcmp(ptrA, ptrB);
}


void printTopFrequencies(void) {
    int i;
    int topFreq = 10;
    long tempPoint;
    char **keys;
    keys = dctkeys(dct);
    
    /* for (i = 0; i < dct->size; i++){
        tempPoint = (long)dctget(dct,keys[i]);
        printf("Before qsort: %s has these occurances: %ld \n", keys[i], tempPoint);
    } */

    qsort(keys,dct->size, sizeof(char *), compareNodes);

    if (dct->size < 10){
        topFreq = dct->size;
    }

    for (i = 0; i < topFreq; i++){
        tempPoint = (long)dctget(dct,keys[i]);
        printf("%s (%ld)\n", keys[i], tempPoint);
    }
    free(keys);
}



int main(int argc, char *argv[]) {
    /* not considering arg1.txt file */
    char textFileName[512];
    char *text;

    if (argc < 2){
        printf("%s: Too few arguments\n", argv[0]);
        return 1;
    }
    if (argc > 2){
        printf("%s: Too many arguments\n",argv[0]);
        return 1;
    }

    strcpy(textFileName,argv[1]);

    /*
    if(getTextFileName(argv[0],fileName,textFileName)) return 1;
    printf("textfile name is %s\n",textFileName); 
    */

    text = readTextfile(textFileName);
    if (text == NULL){
        printf("%s: No such file or directory\n",argv[0]);
        return 1;
    }
    
    /* printf("the text is %s\n",text); */
    dct = dctcreate();

    wordsIntoDictionary(text, dct);
    printTopFrequencies();

    free(text);
    dctdestroy(dct);
    return 0;
}
