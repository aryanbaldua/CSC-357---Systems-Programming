#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/* things to take care of (+ is done, - is not sure/not done)
- can i have all my helped functions in fsort.c do do i need another file (6th bullet pg 2)
- any perror messages or to stderr?
+ if pipe is full...(mid 2nd page)
+ ensure base is cast to (char *) whenever used in arthmetic
*/

/* function just for debugging to print array 
void printArray(void *base, size_t n, size_t width) {
    printf("Array: ");
    for (size_t i = 0; i < n; i++) {
        if (width == sizeof(int)) {
            printf("%d ", *((int *)((char *)base + i * width)));
        } else if (width == sizeof(char)) {
            printf("%c ", *((char *)((char *)base + i * width)));
        } else {
            printf("Unknown type\n");
            break;
        }
    }
    printf("\n");
}
*/

/* merges two sorted halves */
int merge(void *base, size_t left, size_t mid, size_t right, size_t width, int (*cmp)(const void *, const void *)) {
    /* sizes of subarrays */
    size_t n1 = mid - left + 1;
    size_t n2 = right - mid;


    /* copies subarrays into helper temp arrays */
    void *L = malloc(n1 * width);
    void *R = malloc(n2 * width);
    memcpy(L, (char *)base + left * width, n1 * width);
    memcpy(R, (char *)base + (mid + 1) * width, n2 * width);

    /* uses given cmp function to merge two subarrays */
    size_t i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (cmp((char *)L + i * width, (char *)R + j * width) <= 0) {
            memcpy((char *)base + k * width, (char *)L + i * width, width);
            i++;
        } else {
            memcpy((char *)base + k * width, (char *)R + j * width, width);
            j++;
        }
        k++;
    }

    /* if any remaining elements in one of the arrays */
    while (i < n1) {
        memcpy((char *)base + k * width, (char *)L + i * width, width);
        i++;
        k++;
    }
    while (j < n2) {
        memcpy((char *)base + k * width, (char *)R + j * width, width);
        j++;
        k++;
    }

    free(L);
    free(R);

    return 0;
}



int fsort(void *base, size_t n, size_t width, size_t min, int (*cmp)(const void *, const void *)) {
    //printf("Sorting array of size %zu\n", n);
    //printArray(base, n, width);
    if (n <= min) { //is it < or <= ??
        //printf("Using qsort for array of size %zu\n", n); 
        qsort(base, n, width, cmp);
        //printf("This is the sorted ");
        //printArray(base, n, width); 
        return 0;
    }

    size_t mid = n / 2;

    int pipefd[2];

    if (pipe(pipefd) == -1) {
      perror("pipe");
      return 1;
  }

    pid_t pid = fork();
    /* if resource limit is touched */
    if (pid < 0) {
        //perror("fork");
        return 1;
    }

    /* child process */
    if (pid == 0) { 
        close(pipefd[0]); 
        //printf("The pid calling this fsort is: %d\n", getpid());
        if (fsort((char *)base + mid * width, n - mid, width, min, cmp) != 0) {
            exit(1);
        }
        //delete
        //while ( (write(pipefd[1], (char *)base + mid * width, (n - mid) * width)) > 0 )

        /* keeps writing until everything is written */
        size_t bytesWritten = 0;
        size_t messageLength = (n - mid) * width;
        while (bytesWritten < messageLength) {
            ssize_t n = (write(pipefd[1], (char *)base + mid * width, messageLength - bytesWritten));
            bytesWritten += n;
        }
        close(pipefd[1]);
        exit(0);
    } 
    /* parent process */
    else { 
        //printf("Parent process waiting for child\n");
        //printf("The pid calling this fsort is: %d\n", getpid());
        //do i need this if statement
        if (fsort(base, mid, width, min, cmp) != 0) {
            return 1;
        }
        close(pipefd[1]); 

        /* keeps reading until everything is read */
        ssize_t BytesRead = 0;
        ssize_t TotalBytesToRead = (n - mid) * width;
        char *read_position = (char *)base + mid * width;

        while (BytesRead < TotalBytesToRead) {
            ssize_t n = read(pipefd[0], read_position + BytesRead, TotalBytesToRead - BytesRead);
            if (n == 0) {
                break;
            }
            BytesRead += n;
        }
        close(pipefd[0]);
        wait(NULL);
        merge(base, 0, mid - 1, n - 1, width, cmp); 
    }

    //printArray(base, n, width); 
    return 0;
    }










