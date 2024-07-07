#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include "batch.h"

command *mycmds = NULL;

int errorVar = 0;
int totalCommands = 0;
int isN = 0;
int maxChild = -1;
int isE = 0;
int isV = 0;
int indexFirstCommand = -1;


int main(int argc, char *argv[]) {
  
    int numChild = 0;
    int child = 1;
    int currCommand = 0;
    int status = 0;
    filterInput(argc, argv);
    //printf("Total argc is:%d", argc);
    //printf("The first argument is %s", argv[9]);

    /* start of setting up and filling memory */
    mycmds = (struct command *)malloc(totalCommands * sizeof(struct command));
    int currentArgv = indexFirstCommand + 1;

    for (int i = 0; i < totalCommands; i++) {
        /* initializing all values in the structure */
        mycmds[i].pid = -1; 
        mycmds[i].cmd = NULL;
        mycmds[i].argv = NULL;
        mycmds[i].argc = 0;

        /* add to .cmd */
        mycmds[i].cmd = (char *)malloc(strlen(argv[currentArgv]) + 1);
        strcpy(mycmds[i].cmd, argv[currentArgv]);

        /* Add the command to the .argv */
        mycmds[i].argv = (char **)malloc(2 * sizeof(char *)); // allocates space for the cmd and NULL terminator
        mycmds[i].argv[0] = (char *)malloc(strlen(argv[currentArgv]) + 1);
        strcpy(mycmds[i].argv[0], argv[currentArgv]);
        currentArgv++;
        int numArgsPerCommand = 1;

        /* Keeps adding rest of args to the command */
        while ((currentArgv < argc) && (strcmp(argv[currentArgv], "--") != 0)) {
            numArgsPerCommand++;
            char **temp = (char **)realloc(mycmds[i].argv, (numArgsPerCommand + 1) * sizeof(char *));
            mycmds[i].argv = temp;
            mycmds[i].argv[numArgsPerCommand - 1] = (char *)malloc(strlen(argv[currentArgv]) + 1);
            strcpy(mycmds[i].argv[numArgsPerCommand - 1], argv[currentArgv]);
            currentArgv++;
        }
        mycmds[i].argv[numArgsPerCommand] = NULL; // add the null terminator
        mycmds[i].argc = numArgsPerCommand;
        currentArgv++; // skips over the "--" before next command
    }
     /* end of setting up and filling memory */


    /* registering handler */
    struct sigaction action;
    action.sa_handler = parentHandler;
    action.sa_flags = SA_RESTART;
    sigemptyset(&action.sa_mask);
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGQUIT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);


    while ( (numChild < maxChild) && (child != 0) && (totalCommands > currCommand) ){
        if (isV){
            //fprintf(stderr, "+ %s %s %s", mycmds[currCommand].cmd, mycmds[currCommand].argv[0], mycmds[currCommand].argv[1]);
            fprintf(stderr, "+");
            for (int i = 0; i < mycmds[currCommand].argc; i++){
                fprintf(stderr, " %s", mycmds[currCommand].argv[i]);
                //fflush(stderr);
            }
            fprintf(stderr, "\n");
        }
        child = fork();
        /* for the child */
        if (child == 0){
            //printf("I am the child, before execvp\n");
            // char *args[] = {mycmds[currCommand].cmd, mycmds[currCommand].status, mycmds[currCommand].delay, NULL};
            execvp(mycmds[currCommand].cmd, mycmds[currCommand].argv);
            // execvp replaces current process if successful, else returns -1
            // assuming child process terminates in execvp if successful else exit with failure.
            perror(mycmds[currCommand].cmd);
            //printf("Should be returning 1\n");
            exit(EXIT_FAILURE);
        }

        /* the rest of the code only gets executed in the parent */
        mycmds[currCommand].pid = child;

        //printf("I have create a child with pid %d\n", mycmds[currCommand].pid);
        currCommand++;
        numChild++;
        if (numChild == maxChild){
            numChild = WaitAndProccess(&status, numChild);
        }
    } // end of big while

    /* waits for all but one child, that already exited, to exit */
    while (numChild != 0){
        numChild = WaitAndProccess(&status, numChild);
    } 


    freeMemory();
    return errorVar;
}


int removeChildFromCommands(int pidChildDone, int totalCommands, int numChild, struct command *mycmds){
    for (int i = 0; i < totalCommands; i++){
        if (mycmds[i].pid == pidChildDone){
            if (isV){
                fprintf(stderr, "-");
                for (int j = 0; j < mycmds[i].argc; j++){
                    fprintf(stderr, " %s", mycmds[i].argv[j]);
                }
                fprintf(stderr, "\n");
            }
            mycmds[i].pid = -2;
            //printf("The child that is done now has a pid of %d\n",mycmds[i].pid );
        }
    }
    numChild--;
    return numChild;
}


void killRemainingChilds() {
    char buffer[1024];
    for (int i = 0; i < totalCommands; i++ ){
        if (mycmds[i].pid >= 0 ){
            kill(mycmds[i].pid, SIGKILL);
            if (isV){
                strcpy(buffer, "-");
                for (int j = 0; j < mycmds[i].argc; j++){
                    strcat(buffer, " ");
                    strcat(buffer, mycmds[i].argv[j]);
                }
                strcat(buffer, "\n");
                //sprintf(buffer, "- %s %s %s\n", mycmds[i].cmd, mycmds[i].status, mycmds[i].delay);
                write(STDERR_FILENO, buffer, strlen(buffer));
            }
        }
    }
    freeMemory(); // free memory before exiting
    //printf("I get till here\n");
    exit(EXIT_FAILURE);
}

void parentHandler(int signum) {
    killRemainingChilds();
}

int WaitAndProccess(int *status, int numChild){
    int pidChildDone = 0;
    pidChildDone = wait(status);
    numChild = removeChildFromCommands(pidChildDone, totalCommands, numChild, mycmds);
    //printf("This is the status: %d\n", *status);
    if (isE){
        if (!(WIFEXITED(*status) && (WEXITSTATUS(*status) == 0)) ){
            //fprintf( stderr,"Child failed. Killing all running children.\n");
            killRemainingChilds();
        }
    }
    else{
        /* for when permissions are denied */
        if (!(WIFEXITED(*status) && (WEXITSTATUS(*status) == 0)) ){
            errorVar = 1;
        }
    }
    return numChild;
}

int filterInput(int argc, char *argv[]){
    /* finds the first time when '--' occurs */
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--") == 0) {
            // If found, store the index and break the loop
            indexFirstCommand = i;
            break;
        }
    }
    if (indexFirstCommand == -1){
        fprintf(stderr, "usage: ./batch [-n N] [-e] [-v] -- COMMAND [-- COMMAND ...]\n");
        exit(EXIT_FAILURE);
    }
    
    /* checks for all flags like -e -v -n N */
    for (int j = 1; j < indexFirstCommand; j++){
        if ( (strcmp(argv[j], "-n") != 0) && (strcmp(argv[j], "-v") != 0) && (strcmp(argv[j], "-e") != 0)){
            fprintf(stderr, "usage: ./batch [-n N] [-e] [-v] -- COMMAND [-- COMMAND ...]\n");
            exit(EXIT_FAILURE);
        }

        if (strcmp(argv[j], "-e") == 0) {
            isE = 1;
        }

        if (strcmp(argv[j], "-v") == 0) {
            isV = 1;
        }

        if (strcmp(argv[j], "-n") == 0) {
            isN = 1;
            if (is_valid_integer(argv[j+1])){
                maxChild = atoi(argv[j+1]);
                j++; //This increments j inside the loop to skip over N
                if (maxChild <= 0){
                    fprintf(stderr, "usage: ./batch [-n N] [-e] [-v] -- COMMAND [-- COMMAND ...]\n");
                    exit(EXIT_FAILURE);
                }
            }
            else{
                fprintf(stderr, "usage: ./batch [-n N] [-e] [-v] -- COMMAND [-- COMMAND ...]\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    /* gives totalCommands appropriate value based on how many "--" occurs */
    for (int m = indexFirstCommand; m < argc; m++) {
        if (strcmp(argv[m], "--") == 0) {
            totalCommands++;
        }
    }

    /* sets default number of max processes to the number of commands */
    if(!isN){
        maxChild = totalCommands;
    }
      
    
    return 0;
}

int is_valid_integer(const char *str) {
    if (str == NULL || *str == '\0') {
        return 0; 
    }

    char *endptr = NULL;
    errno = 0; // Reset errno before the call
    strtol(str, &endptr, 10);

    // Check for various possible errors
    if (errno != 0 || endptr == str || *endptr != '\0') {
        return 0;
    }

    return 1; 
}

int freeMemory(){
    for (int i = 0; i < totalCommands; i++) {
        for (int j = 0; j < mycmds[i].argc; j++) {
            free(mycmds[i].argv[j]);
        }
        free(mycmds[i].argv);
        free(mycmds[i].cmd);
    }
    free(mycmds);
    
    return 0;
}


