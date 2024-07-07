typedef struct command {
  char *cmd;
  int argc;
  int pid;
  char **argv;
} command;

int removeChildFromCommands(int pidChildDone, int totalCommands, int numChild, struct command *mycmds);
void parentHandler(int signum);
void killRemainingChilds();
int WaitAndProccess(int *status, int numChild);
int filterInput(int argc, char *argv[]);
int is_valid_integer(const char *str);
int freeMemory();

