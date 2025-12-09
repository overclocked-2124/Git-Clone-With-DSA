#include "git_dsa.h"

void print_help() {
    printf("Simple Git Clone (DSA Project)\n");
    printf("Commands:\n");
    printf("  init            Initialize a new repository\n");
    printf("  add <filename>  Add a file to staging area\n");
    printf("  commit <msg>    Record changes to the repository\n");
    printf("  log             Show commit logs\n");
    printf("  status          Show the working tree status\n");
    printf("  exit            Exit the program\n");
}

int main() {
    char command[50];
    char arg[MAX_MESSAGE];
    
    printf("Welcome to Git-DSA! Type 'help' for commands.\n");
    
    while (1) {
        printf("\ngit-dsa> ");
        if (scanf("%s", command) == EOF) break;
        
        if (strcmp(command, "exit") == 0) {
            break;
        } else if (strcmp(command, "help") == 0) {
            print_help();
        } else if (strcmp(command, "init") == 0) {
            init_repo();
        } else if (strcmp(command, "add") == 0) {
            scanf("%s", arg);
            add_file(arg);
        } else if (strcmp(command, "commit") == 0) {
            // Read the rest of the line for the message
            char temp;
            scanf("%c", &temp); // consume space
            fgets(arg, MAX_MESSAGE, stdin);
            // Remove newline at the end
            arg[strcspn(arg, "\n")] = 0;
            commit_changes(arg);
        } else if (strcmp(command, "log") == 0) {
            show_log();
        } else if (strcmp(command, "status") == 0) {
            show_status();
        } else {
            printf("Unknown command: %s\n", command);
        }
    }
    
    return 0;
}
