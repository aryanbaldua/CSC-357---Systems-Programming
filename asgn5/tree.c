#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tree.h"

int exit_val = 0;

int main(int argc, char *argv[]) {


   /* check if argv contains a "-a" */
   int i = 0;
   int isA = 0;
   int isD = 0;
   int Dlevel = -1;
   int isMode = 0;
   int checkInput = 1;
    for (i = 1; i < argc; i++){


       if (strcmp(argv[i], "-a") == 0){
           isA = 1;
           checkInput++;
       }
       if (strcmp(argv[i], "-d") == 0){
           //printf("%d isMode %s\n", i, argv[i]);
           isD = 2;
           if (Dlevel == -1){
                if (is_valid_integer(argv[i+1])){
                    Dlevel = atoi(argv[i+1]);
                }
                else{
                    //printf("Hi i am here");
                    fprintf(stderr, "usage: ./tree [-a] [-d N] [-l] [PATH]\n");
                    return EXIT_FAILURE; 
                }
           }
           if (Dlevel >= 0){
               //printf("This is dlevel: %d", Dlevel);
               checkInput += 2;
           }
       }
       if (strcmp(argv[i], "-l") == 0){
           isMode = 1;
           checkInput++;
       }


       if (checkInput == 0){
           fprintf(stderr, "usage: ./tree [-a] [-d N] [-l] [PATH]\n");
           return EXIT_FAILURE;
       }
       checkInput--;
    }

    char *root = NULL;
    if ((argc - isA - isD - isMode) == 1){
        root = ".";
    }
    else{
        root = argv[argc-1];
    }
    //printf("The root is %s\n", root);

    if (is_symlink(root)){
        //printf("Before printOthers\n");
	    exit_val = printOthers(isMode, 0, root);
        return EXIT_SUCCESS;
    }

    struct stat buf;
    if (stat(root, &buf) < 0) {
        //perror(root);
        //fprintf(stdout, "%s: No such file or directory\n", root);
        //printf("Here\n");
        printf("%s: %s\n", root, strerror(errno));
        //printf("Here after\n");
        //printf("Reaching line 82\n");
        exit(EXIT_FAILURE);
    }
    else{
        if (lstat(root, &buf) < 0) {
            //printf("Here!\n");
            return -1;
        }
        // printf("Reaching line 90\n");
    }

    DIR *dir;
    //struct dirent *entry;
    //chdir(argv[argc-1]);
    //printf("dir is: %s\n", dir);

    /* char cwd1[256];
    if (getcwd(cwd1, sizeof(cwd1)) == NULL)
        perror("getcwd() error");
    else
        printf("current working directory 1st time is: %s\n", cwd1);
    */
    //entry = readdir(dir);
    
    /* prints persmission numbers for root, printing d because root has to be directory */
    printOthers(isMode, 0, root);
    //printf("exit val on 108 is %d", exit_val);

    /* is argument is soft link */
    if (is_symlink(root)){
        return EXIT_SUCCESS;
    }

    int level = 1;
    dir = opendir(root);
    if (S_ISDIR(buf.st_mode)){
        temp(root, level, isA, isD, Dlevel, isMode);
    }
    closedir(dir);

   return exit_val;
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

int compareEntries(const void *a, const void *b) {
   struct dirent *entrya;
   struct dirent *entryb;


   entrya = (struct dirent *)a;
   entryb = (struct dirent *)b;


   return strcmp(entrya->d_name, entryb->d_name);
}


int is_symlink(const char *path) {
   struct stat buf;
   if (lstat(path, &buf) < 0) {
        //printf("Before perror lstat in symlink\n");
       //perror("lstat");
       return 0;
   }
   return S_ISLNK(buf.st_mode);
}

int printOthers(int isMode, int level, char *iName){
    struct stat buf;
    //struct dirent *name = entry;
    if (is_symlink(iName)){
        if (lstat(iName, &buf) < 0) {
            //perror("stat");
            fprintf(stderr, "usage: ./tree [-a] [-d N] [-l] [PATH]\n");
            exit(EXIT_FAILURE);
        }
    }
    else if (stat(iName, &buf) < 0) {
        //printf("I am here\n");
        //perror("stat");
        fprintf(stderr,"usage: ./tree [-a] [-d N] [-l] [PATH]\n");
        exit(EXIT_FAILURE);
    }

    if (isMode){
        //printf("The dname is %s\n",iName);
        if (is_symlink(iName)){
            printf("l");
        }
        else if (S_ISDIR(buf.st_mode)){
            printf("d");
        }
        else {
            printf("-");
        }
        printf( (buf.st_mode & S_IRUSR) ? "r" : "-");
        printf( (buf.st_mode & S_IWUSR) ? "w" : "-");
        printf( (buf.st_mode & S_IXUSR) ? "x" : "-");
        printf( (buf.st_mode & S_IRGRP) ? "r" : "-");
        printf( (buf.st_mode & S_IWGRP) ? "w" : "-");
        printf( (buf.st_mode & S_IXGRP) ? "x" : "-");
        printf( (buf.st_mode & S_IROTH) ? "r" : "-");
        printf( (buf.st_mode & S_IWOTH) ? "w" : "-");
        printf( (buf.st_mode & S_IXOTH) ? "x" : "-");
    }
    /* end of if mode */


    int i = 0;
    for (i = level; i > 0; i--){
        printf("    ");
    }


    if (is_symlink(iName) && isMode){
        char buffer[16192];
        ssize_t len = 0;


        len = readlink(iName, buffer, sizeof(buffer) - 1);
        if (len == -1) {
            //perror("readlink");
            fprintf(stderr, "usage: ./tree [-a] [-d N] [-l] [PATH]\n");
            return EXIT_FAILURE;
        }
        buffer[len] = '\0';


        printf(" %s -> %s", iName, buffer);
    }
    else{
        if (isMode){
            printf(" ");
        }
        printf("%s", iName);
    }

    /* checking if have permission for the directory */
    DIR *dir;
    //printf("This is the iname:%s\n", iName);
    
    if (S_ISDIR(buf.st_mode)){
        //printf("This is the iname:%s\n", iName);
        dir = opendir(iName);
        if (dir == NULL) {
            exit_val = 1;
            //printf("Hellllllo\n");
            printf(": Permission denied");
        }
        else {
            closedir(dir);
        }
    }

    /* print newline after everything else if printed for all types */
    printf("\n");



    return EXIT_SUCCESS;
}

int temp(char *path, int level, int isA, int isD, int Dlevel, int isMode) {
   DIR *dir;
   //const char *parentPath;
   struct dirent *entry;
   struct stat buf;

   char cwd1[16192];
   if (getcwd(cwd1, sizeof(cwd1)) == NULL)
      perror("getcwd() error");
   /* else
      printf("current working directory 1st time is: %s\n", cwd1);
    */
    //printf("dir is: %s\n", dir);
    //char cwd1[256];
    //if (getcwd(cwd1, sizeof(cwd1)) == NULL)
       // perror("getcwd() error");
    //else
        //printf("current working directory right before the opendir error: %s\n", cwd1);
    //printf("This is the path right before the opendir error: %s\n", path);

   dir = opendir(path);
   
   //printf("this is the dir: %s\n", dir);
   if (dir == NULL) {
       //printf("Hi I cannot access this dir\n");
       return 0;
   }


   chdir(path);


   /* get number of children to know how much space to malloc */
   int numChildren = 0;
   while ((entry = readdir(dir)) != NULL) {
       //printf("this is the dname: %s\n", entry->d_name[0] );
       stat(entry->d_name, &buf);
       
       /*
       if (stat(entry->d_name, &buf) < 0) {
           printf("Line 231\n");
           perror("stat");
           exit(EXIT_FAILURE);
       }
       */
       numChildren++;
   }
   closedir(dir);


 


   /* malloc space for array of struct dirents */
   struct dirent *entries = NULL;
   entries = (struct dirent *)malloc(numChildren * sizeof(struct dirent));


   /* insert each entry into entries */
   int insertIndex = 0;
   /* changing from chdir("..") to path */
   /*
   char cwd2[8096];
    if (getcwd(cwd2, sizeof(cwd2)) == NULL)
        perror("getcwd() error");
    */
    /* else
      printf("current working directory 2nd time is: %s\n", cwd2);
    */
   chdir(cwd1);
   //printf("This is the path the second time: %s\n", path);
   dir = opendir(path);
   //printf("this is the dir the second time: %s\n", dir);


   //printf("Is it a link %d\n", S_ISLNK(buf.st_mode));
   if (dir == NULL) {
       //perror("opendir2");
       fprintf(stderr, "usage: ./tree [-a] [-d N] [-l] [PATH]\n");
       exit(EXIT_FAILURE);
   }
   while ((entry = readdir(dir)) != NULL) {
       //printf("index isMode %d\n",insertIndex);
       if ((strncmp(entry->d_name, ".", 1) == 0) && !isA){
           numChildren--;
       }
       else{
        entries[insertIndex] = *(entry);
        insertIndex++;
       }
   }
   //printf("after while loop\n");




   /* use qsort to sort the array of entry pointers alphabetically */
   qsort(entries, numChildren, sizeof(struct dirent),compareEntries);


   chdir(path);
   int j = 0;


   for (j = 0; j < numChildren; j++){


       if ((strcmp(entries[j].d_name, ".") != 0) && (strcmp(entries[j].d_name, "..") != 0)){


           /* stating contents into buf */
            if(!is_symlink(entries[j].d_name)){
                if (stat(entries[j].d_name, &buf) < 0) {
                    //printf("Line 294\n");
                    //perror("stat");
                    fprintf(stderr, "usage: ./tree [-a] [-d N] [-l] [PATH]\n");
                    exit(EXIT_FAILURE);
                }
            }
            else{
                if (lstat(entries[j].d_name, &buf) < 0) {
                    //perror("lstat");
                    fprintf(stderr, "usage: ./tree [-a] [-d N] [-l] [PATH]\n");
                    exit(EXIT_FAILURE);
                }
            }
            

            if ((strncmp(entries[j].d_name, ".", 1) != 0) || isA){
                if ( (isD && (level <= Dlevel)) || !isD){
                    printOthers(isMode, level, entries[j].d_name);
                }             
            }
       }

       // If entry isMode a directory and not "." or "..", recurse into it
       if (S_ISDIR(buf.st_mode) && (strcmp(entries[j].d_name, ".") != 0) && (strcmp(entries[j].d_name, "..") != 0)) {
           if (!is_symlink(entries[j].d_name)){
               level++;
               //printf("This is entiries j: %s\n", entries[j].d_name);
               temp(entries[j].d_name, level, isA, isD, Dlevel, isMode);
               level--;
           }
       }

   }

    free(entries);
    closedir(dir);
    chdir(cwd1); // Move back to the parent directory



   return EXIT_SUCCESS;
}


