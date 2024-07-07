#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "dict.h"

uint16_t *intToCodes(int encodedNums[], int encodedCount);
int encode_and_write(const char *, uint16_t *, size_t );
uint16_t *read_and_decode(const char *filename, size_t *num_codes);
uint16_t *hexStringToCodes(const char *, size_t *);
uint16_t hexPairToCode(const char *hexPair);


/* copied this code from asgn3 */
char *readTextfile(char *textFileName) {
    char *text = NULL;
    char buffer[4096]; 
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


/* converts integers into decimal and takes care of padding extra 0 */
uint16_t *intToCodes(int encodedNums[], int encodedCount) {
    // Allocate memory for the array of codes
    uint16_t *codes = (uint16_t *)malloc(encodedCount * sizeof(uint16_t));
    if (!codes) {
        perror("Memory allocation failed");
        return NULL;
    }

    // Convert each integer to a uint16_t code
    int i = 0;
    for (i = 0; i < encodedCount; i++) {
        codes[i] = (uint16_t)encodedNums[i];
        //printf("This is in intToCodes: %d\n", codes[i]);
    }

    return codes;
}


/* takes in codes and number of them and writes to file */
int encode_and_write(const char *filename, uint16_t *codes, size_t num_codes) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("./lzw");
        return -1;
    }

    size_t i = 0;
    for (i = 0; i < num_codes; i += 2) {
        uint16_t code1 = codes[i];
        uint16_t code2 = (i + 1 < num_codes) ? codes[i + 1] : 0;

        // Pack code1 and code2 into three bytes
        uint8_t byte1 = (code1 >> 4) & 0xFF;
        uint8_t byte2 = ((code1 & 0x0F) << 4) | ((code2 >> 8) & 0x0F);
        uint8_t byte3 = code2 & 0xFF;

        fwrite(&byte1, 1, 1, file);
        fwrite(&byte2, 1, 1, file);
        if (code2 != 0){
            fwrite(&byte3, 1, 1, file);
        }
    }

    fclose(file);

    return 0;
}


/* decodes codes from specific file */
uint16_t *read_and_decode(const char *filename, size_t *num_codes) {
    uint16_t *decoded;
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("./lzw");
        exit(1);
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Calculate the number of codes based on the file size
    *num_codes = (size_t)(file_size * 8 / 12); // Each code is 12 bits (3/8 of a byte)

    // Allocate memory for decoded codes
    decoded = (uint16_t *)malloc((*num_codes + 1) * sizeof(uint16_t));
    if (!decoded) {
        perror("Memory allocation failed");
        fclose(file);
        return 0;
    }

    size_t index = 0;
    int singleCode = 0;
    while (index < num_codes) {
        uint8_t byte1, byte2, byte3;
        
        if (fread(&byte1, 1, 1, file) != 1 || fread(&byte2, 1, 1, file) != 1 ){
            //printf("I am inside the FIRST if loop and the index is %d\n", index);
            break;
        }
        if (fread(&byte3, 1, 1, file) != 1) {
            //printf("I am inside the SECOND if loop and the index is %d\n", index);
            singleCode = 1;
        }

        // Decode the three bytes into two 12-bit codes
        uint16_t code1 = 0;
        uint16_t code2 = 0;
        if(singleCode){
            code1 = ((uint16_t)byte1 << 4) | ((byte2 >> 4) & 0x0F);
        }
        else{
            code1 = ((uint16_t)byte1 << 4) | ((byte2 >> 4) & 0x0F);
            code2 = ((uint16_t)(byte2 & 0x0F) << 8) | (uint16_t)byte3;
        }

        decoded[index] = code1;
        //printf("This is code1: %u, with index: %d \n", code1, index);
        index++;
        if (index < num_codes) {
            decoded[index] = code2;
            //printf("This is code2: %u, with index: %d\n", code2, index);
            index++;
        }
    }
    //free(decoded);
    //printf("Outside the while loop this is num_codes: %d, and this is index:%d\n", num_codes, index);

    fclose(file);
    return decoded; 
}


/* takes in input and writes encoded to specified file */
int encode(char *input, const char *filename ) {
    Dict *dct;
    dct = dctcreate();

    /* Insert basic ASCII characters into the dictionary */
    int i = 0;
    for (i = 0; i < 128; i++) {
        char key[2];
        key[0] = (char)i;
        key[1] = '\0';
        dctinsert(dct, key, (void *)i);
    }

    int dictSize = 128;
    char *currentLetter = (char *)malloc(4096); 
    currentLetter[0] = input[0];
    currentLetter[1] = '\0';
    int encodedNums[4096];
    int encodedCount = 0;

    int index = 1;
    while (input[index] != '\0') {
        char nextChar = input[index++];
        char *extendedPhrase = (char *)malloc(strlen(currentLetter) + 2);
        strcpy(extendedPhrase, currentLetter);
        strncat(extendedPhrase, &nextChar, 1);

        /* printf("This is extendedphrase:%s, this is currentletter: %s, this is nextChar %c \n", extendedPhrase, currentLetter, nextChar); */

        /* Check if the extended phrase exists in the dictionary */
        if (dctget(dct, extendedPhrase) != NULL) {
            strcpy(currentLetter, extendedPhrase);
        } else {
            /* Output the code for the current phrase */
            /* printf("%d \n", (int)dctget(dct, currentLetter)); */
            encodedNums[encodedCount] = (int)dctget(dct, currentLetter);
            encodedCount++;

            dctinsert(dct, extendedPhrase, (void *)dictSize++);
            currentLetter[0] = nextChar;
            currentLetter[1] = '\0';
        }        
        free(extendedPhrase);
    }

    /* Output the code for the last phrase */
    encodedNums[encodedCount] = (int)dctget(dct, currentLetter);
    encodedCount++;

    free(currentLetter);
    dctdestroy(dct);

    uint16_t *codes_to_encode = intToCodes(encodedNums, encodedCount);
    encode_and_write(filename, codes_to_encode, encodedCount);
    free(codes_to_encode);
    

    return 0;
}



int decode(const char *inputFile, const char *outputFile){
    uint16_t *decoded = NULL;
    size_t num_codes;
    Dict *dct;
    dct = dctcreate();

    //num_codes = read_and_decode(inputFile, &decoded);
    decoded = read_and_decode(inputFile, &num_codes);
    

    // Print the decoded codes
    //printf("Decoded codes: %u\n", num_codes);
    //int *decoded_int[num_codes];
    int *decoded_int = NULL;
    decoded_int = (int *)malloc(sizeof(int) * num_codes);
    size_t i = 0;
    for (i = 0; i < num_codes; i++) {
        //printf("Num codes is: %zu\n", num_codes);
        decoded_int[i] = (int)decoded[i];
        //printf("%d ", decoded_int[i]);
    }
    //printf("\n");


    char *storedCopies[4096];
    int m = 0;
    int k = 0;
    int uniqueVal = 0;
    

    /* inserts base dictionary values */
    for (m = 0; m < 128; m++) {
        char str_key[5]; 
        sprintf(str_key, "%d", m);
        storedCopies[uniqueVal] = (char *)malloc(sizeof(char) * 2);
        storedCopies[uniqueVal][0] = (char)m; 
        storedCopies[uniqueVal][1] = '\0';
        dctinsert(dct, str_key, storedCopies[uniqueVal++]);
    }

    /* open file to write */
    FILE * fp;
    fp = fopen(outputFile, "w");
    if (fp == NULL){
        perror("./lzw");
        exit(EXIT_FAILURE);
    }


    char previous[4096];
    char next[4096];
    char newPhrase[4096];
    char decodedString[4096];
    //printf("Num codes is: %zu\n", num_codes);
    //int temp = num_codes+1;
    //decodedString = (char *)malloc(sizeof(char) * temp);
    decodedString[0] = '\0';
    previous[0] = '\0';
    for (k = 0; k < num_codes; k++) {
        /* check if code is greater than the last known encoding */   
        char str_key[5]; 
        sprintf(str_key, "%d", (decoded_int[k]));
        if( decoded_int[k] >= uniqueVal){
            strncat(next, previous, 1);
            //printf("This is special case:%s\n", next);
        } 
        else {
            void *value = dctget(dct, str_key);
            strcpy(next, (char *)value);
        }

        if (previous[0] != '\0'){
            strcat(decodedString, next);
            //printf("%s\n", decoded);
            strcpy(newPhrase, previous);
            strncat(newPhrase, next, 1);
            storedCopies[uniqueVal] = (char *)malloc(sizeof(char) * (strlen(newPhrase)) + 1);
            strcpy(storedCopies[uniqueVal], newPhrase);
            sprintf(str_key, "%d", uniqueVal);
            //printf("Before calling dctinsert str: %s, val: %s\n", str_key, storedCopies[uniqueVal]);
            dctinsert(dct, str_key, storedCopies[uniqueVal++]);
        }
        else{
            strcat(decodedString, next);
            //printf("i am in the else with decoded string\n");
        }
        fwrite(decodedString, sizeof(char) , strlen(decodedString), fp);
        decodedString[0] = '\0'; 
        strcpy(previous, next);
    }
    
    //decodedString[++k] = '\n';
    //decodedString[++k] = '\0';
    //printf("%s\n", decodedString);

    free(decoded);
    free(decoded_int);
    int p = 0;
    for (p = 0; p < uniqueVal; p++){
        free(storedCopies[p]);
    }

    dctdestroy(dct);
    fclose(fp);

    return 0;
}


int main(int argc, char *argv[]) {
    char textFileName[512];
    char *text;
    //need to change testing.bin from being hardcoded
    //const char *filename = "out3.bin"; 

    /*
    if(getTextFileName(argv[0],fileName,textFileName)) return 1;
    printf("textfile name is %s\n",textFileName); 
    */
    
    //printf("This is argv1: %s", argv[1]);
    if (argc != 4){
        fprintf(stderr, "usage: ./lzw (-c | -x) SOURCE DESTINATION\n");
        return 1;
    }
    if ((strcmp(argv[1], "-c") != 0) && (strcmp(argv[1], "-x") != 0)){
        fprintf(stderr, "usage: ./lzw (-c | -x) SOURCE DESTINATION\n");
        return 1;       
    }
    if (strcmp(argv[1], "-c") == 0){
        //printf("This is argv1: %s", argv[1]);
        const char *outputFile = argv[3];
        strcpy(textFileName,argv[2]);
        text = readTextfile(textFileName);
        if (text == NULL){
            perror("./lzw");
            return 1;
        }
        //printf("this is the text: %s\n", text);
        encode(text, outputFile);
        free(text);
    }

    if (strcmp(argv[1], "-x") == 0){
        const char *inputFile = argv[2];
        const char *outputFile = argv[3];
        decode(inputFile, outputFile);
    }

    
    return 0;
}
