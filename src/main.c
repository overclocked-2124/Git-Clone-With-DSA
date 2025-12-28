#include "git_dsa.h"

void print_help() {
    printf("Simple Git Clone (DSA Project)\n");
    printf("Commands:\n");
    printf("  init                  Initialize a new repository\n");
    printf("  add <filename>        Add a file to staging area\n");
    printf("  commit <msg>          Record changes to the repository\n");
    printf("  log                   Show commit logs\n");
    printf("  status                Show the working tree status\n");
    printf("  graph                 Show visual commit graph with hashes\n");
    printf("\nStash Commands (Stack DSA):\n");
    printf("  stash save <msg>      Save changes to stash stack\n");
    printf("  stash pop             Restore most recent stash\n");
    printf("  stash list            List all stashes\n");
    printf("\nBranch Commands (Tree DSA):\n");
    printf("  branch create <name>  Create a new branch\n");
    printf("  branch list           List all branches (tree view)\n");
    printf("  branch checkout <name> Switch to a branch\n");
    printf("  branch delete <name>  Delete a branch\n");
    printf("\n  exit                  Exit the program\n");
}

int main() {
    char command[50];
    char subcommand[50];
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
        } else if (strcmp(command, "graph") == 0) {
            show_graph();
        } else if (strcmp(command, "stash") == 0) {
            scanf("%s", subcommand);
            if (strcmp(subcommand, "save") == 0) {
                char temp;
                scanf("%c", &temp); // consume space
                fgets(arg, MAX_MESSAGE, stdin);
                arg[strcspn(arg, "\n")] = 0;
                stash_save(arg);
            } else if (strcmp(subcommand, "pop") == 0) {
                stash_pop();
            } else if (strcmp(subcommand, "list") == 0) {
                stash_list();
            } else {
                printf("Unknown stash command: %s\n", subcommand);
            }
        } else if (strcmp(command, "branch") == 0) {
            scanf("%s", subcommand);
            if (strcmp(subcommand, "create") == 0) {
                scanf("%s", arg);
                branch_create(arg);
            } else if (strcmp(subcommand, "list") == 0) {
                branch_list();
            } else if (strcmp(subcommand, "checkout") == 0) {
                scanf("%s", arg);
                branch_checkout(arg);
            } else if (strcmp(subcommand, "delete") == 0) {
                scanf("%s", arg);
                branch_delete(arg);
            } else {
                printf("Unknown branch command: %s\n", subcommand);
            }
        } else {
            printf("Unknown command: %s\n", command);
        }
    }
    
    return 0;
}
