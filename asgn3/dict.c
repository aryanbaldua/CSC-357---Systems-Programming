/* Defines functions for implmenting a dictionary.
 * CSC 357, Assignment 3
 * Given code, Winter '24 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dict.h"
void resize(Dict *dct);
void nodeInsert(Dict *, Node *);
void printNodeArr(Node **nodeArr, int);
void printTopFrequencies (Dict *);

/* #include "clean.h" */

#define SIZE 31


/* dcthash: Hashes a string key.
 * NOTE: This is certainly not the best known string hashing function, but it
 *       is reasonably performant and easy to predict when testing. */
static unsigned long int dcthash(char *key) {
    unsigned long int code = 0, i;

    for (i = 0; i < 8 && key[i] != '\0'; i++) {
        code = key[i] + 31 * code;
    }

    return code;
}


/* dctcreate: Creates a new empty dictionary.
 * TODO: Implement this function. It should return a pointer to a newly
 *       dynamically allocated dictionary with an empty backing array. */
Dict *dctcreate(void) {
    Dict *newDict;
    int i;

    newDict = (Dict *)malloc(sizeof(Dict));
    newDict->cap = SIZE;
    newDict->size = 0;
    newDict->arr = malloc(sizeof(Node *)*(newDict->cap));

    for (i = 0; i < newDict->cap; i++) {
        newDict->arr[i] = NULL;
    }

    return newDict;
}


/* dctdestroy: Destroys an existing dictionary.
 * It should deallocate a dictionary, its backing array, and all of its nodes. */
void dctdestroy(Dict *dct) {
    int i;
    Node *current = NULL;
    Node *next = NULL;

    for (i = 0; i < dct->cap; i++) {
        current = dct->arr[i];

        while (current != NULL) {
            next = current->next; 
            free(current->key);
            /* free(current->val); */
            free(current);
            current = next;
        }
    }

    free(dct->arr);
    free(dct);
}
 


/* dctget: Gets the value to which a key is mapped.
 * TODO: Implement this function. It should return the value to which "key" is
 *       mapped, or NULL if it does not exist. */ 
void *dctget(Dict *dct, char *key) {
    int position;
    Node *temp = NULL;

    position = dcthash(key) % dct->cap;
    temp = dct->arr[position];

    while (temp != NULL) {
        if (strcmp(temp->key, key) == 0) {
            return temp->val;
        }
        temp = temp->next;
    }

    return NULL;
}



/* dctinsert: Inserts a key, overwriting any existing value.
 * TODO: Implement this function. It should insert "key" mapped to "val".
 * NOTE: Depending on how you use this dictionary, it may be useful to insert a
 *       dynamically allocated copy of "key", rather than "key" itself. Either
 *       implementation is acceptable, as long as there are no memory leaks. */
void dctinsert(Dict *dct, char *key, void *val) {
    int position;
    Node *currNode;
    Node *prevNode = NULL;
    Node *newNode = NULL;
    /* long tempPoint; */
    
    position = dcthash(key) % dct->cap;
    currNode = dct->arr[position];


    /* if the key already exists in dct then just replace original nodes value; */
    while (currNode != NULL) {
        if (strcmp(currNode->key, key) == 0) {
            /* tempPoint = (long)val;
            printf("In dctinsert with key: %s and val : %ld \n",currNode->key,tempPoint); */
            /* not sure if this is right */
            currNode->val = val;
            return;
        }
        prevNode = currNode;
        currNode = currNode->next;
    }

    
    /* creates new Node and assigns all values before insertion */
    newNode = (Node *)malloc(sizeof(Node));
    newNode->key = (char *)malloc(sizeof(char) * (strlen(key)+1));
    strcpy(newNode->key,key);
    newNode->val = val;
    newNode->next = NULL;

    /* if inserts at end of linked list, else inserts at beggening b/c no other Nodes there */
    if (prevNode != NULL) {
        prevNode->next = newNode;
    } else{
        dct->arr[position] = newNode; 
    }

    dct->size++;

    /* reallocs backing array if size > capacity */
    if (dct->size > dct->cap){
        resize(dct);
    }

    return;
}



void resize(Dict *dct){
    Node **nodeArr = NULL;
    int j;
    int newCap;
    int i;
    int m; 
    int k = 0;
    Node *currLL = NULL;
    Node *currNode = NULL;

    /* printf("I am resizing\n"); */

    /* creating new array of pointers to each Node in original backing array */    
    nodeArr = malloc(sizeof(Node *)*(dct->size));
    for (m = 0; m < dct->cap; m++){
        currLL = dct->arr[m];
        while (currLL != NULL){
            currNode = currLL;
            currLL = currLL->next;
            currNode->next = NULL;
            nodeArr[k] = currNode; 
            k++;
        } 
    }
    newCap = (dct->cap)*2 + 1;

     /* reallocates backing array to 2n+1 size with original Nodes still getting pointed to from backing array */
    dct->arr = realloc(dct->arr, sizeof(Node *) * newCap);
    dct->cap = newCap;

    /* setting all values in dictionary to NULL */
    for (i = 0; i < dct->cap; i++) {
        dct->arr[i] = NULL;
    }

    /* this loop goes iterates through nodeArr array and calls nodeInsert */
    for (j = 0; j < dct->size; j++) {
        nodeInsert(dct, nodeArr[j]); 
    }  

    free(nodeArr);
    /* printf("I am done resizing\n"); */
    return;       
}                  


void nodeInsert(Dict *dct, Node *currNode){
    int position;
    Node* nextNode;

    position = dcthash(currNode->key) % dct->cap;

    if (dct->arr[position] == NULL){
        dct->arr[position] = currNode;
        return;
    }

    nextNode = dct->arr[position];
    dct->arr[position] = currNode;
    currNode->next = nextNode;
    return;
}     



/* dctremove: Removes a key and the value to which it is mapped.
 * TODO: Implement this function. It should remove "key" and return the value
 *       to which it was mapped, or NULL if it did not exist. */
void *dctremove(Dict *dct, char *key) {
    int position;
    Node *traversingNode = NULL;
    Node *tempNode = NULL;
    
    position = dcthash(key) % dct->cap;
    traversingNode = dct->arr[position];

    /* right now traversing linked list in case of earlier collision to 
    * remove the right Node */
    while (traversingNode != NULL) {
        if (strcmp(traversingNode->key, key) == 0) {
            void *value = traversingNode->val;

            if (tempNode != NULL) {
                tempNode->next = traversingNode->next;
            } else {
                dct->arr[position] = traversingNode->next;
            }

            free(traversingNode->key);
            free(traversingNode);
            dct->size--;
            return value; 
        }
        tempNode = traversingNode;
        traversingNode = traversingNode->next;
    }

    return NULL;

}




/* dctkeys: Enumerates all of the keys in a dictionary.
 * TODO: Implement this function. It should return a dynamically allocated array
 *       of pointers to the keys, in no particular order, or NULL if empty. */
char **dctkeys(Dict *dct) {
    int i;
    char **keys;
    int index;
    Node *current;

    if (dct->size == 0) {
        return NULL;
    }

    keys = malloc(sizeof(char *) * dct->size);

    index = 0;
    for (i = 0; i < dct->cap; i++) {
        current = dct->arr[i];
        while (current != NULL) {
            keys[index] = current->key;
            current = current->next;
            index++;
        }
    }

    return keys;
}

