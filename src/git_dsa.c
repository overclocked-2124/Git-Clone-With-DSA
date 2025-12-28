#include "git_dsa.h"

Repository* repo = NULL;

#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_WHITE   "\033[37m"
#define COLOR_BOLD    "\033[1m"

const char* branch_colors[] = {
    COLOR_RED, COLOR_GREEN, COLOR_YELLOW, 
    COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN
};
#define NUM_COLORS 6

void get_current_time(char* buffer) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(buffer, 50, "%Y-%m-%d %H:%M:%S", tm);
}

unsigned int hash_function(const char* str) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash % HASH_SIZE;
}

void generate_commit_hash(char* hash, int id, const char* message, const char* timestamp) {
    unsigned long h1 = 5381, h2 = 0;
    h1 = ((h1 << 5) + h1) + id;
    
    const char* p = message;
    while (*p) {
        h1 = ((h1 << 5) + h1) + *p;
        h2 = ((h2 << 3) + h2) ^ *p;
        p++;
    }
    
    p = timestamp;
    while (*p) {
        h1 = ((h1 << 5) + h1) + *p;
        h2 = ((h2 << 3) + h2) ^ *p;
        p++;
    }
    
    snprintf(hash, 41, "%08lx%08lx%08lx%08lx%08lx", 
             h1 % 0xFFFFFFFF, h2 % 0xFFFFFFFF,
             (h1 ^ h2) % 0xFFFFFFFF, (h1 + h2) % 0xFFFFFFFF,
             (h1 * 31 + h2) % 0xFFFFFFFF);
}

void init_repo() {
    if (repo != NULL) {
        printf("Repository already initialized.\n");
        return;
    }
    
    repo = (Repository*)malloc(sizeof(Repository));
    if (!repo) {
        perror("Failed to allocate memory for repo");
        exit(1);
    }
    
    repo->head = NULL;
    repo->commit_count = 0;
    
    for (int i = 0; i < HASH_SIZE; i++) {
        repo->staging_area[i] = NULL;
    }
    
    repo->stash_top = NULL;
    repo->stash_count = 0;
    
    repo->branch_tree = (BranchNode*)malloc(sizeof(BranchNode));
    strcpy(repo->branch_tree->name, "master");
    repo->branch_tree->commit_head = NULL;
    repo->branch_tree->commit_count = 0;
    repo->branch_tree->child_count = 0;
    repo->branch_tree->parent = NULL;
    repo->current_branch = repo->branch_tree;
    
    printf("Initialized empty Git repository (in memory).\n");
    printf("Created default branch: master\n");
}

void add_file(const char* filename) {
    if (repo == NULL) {
        printf("Error: Repository not initialized. Run 'init' first.\n");
        return;
    }

    unsigned int index = hash_function(filename);
    
    FileEntry* current = repo->staging_area[index];
    while (current != NULL) {
        if (strcmp(current->filename, filename) == 0) {
            printf("File '%s' is already in staging area. Updating...\n", filename);
            snprintf(current->content_hash, 50, "hash_%s_%d", filename, rand() % 1000);
            return;
        }
        current = current->next;
    }
    
    FileEntry* new_file = (FileEntry*)malloc(sizeof(FileEntry));
    strcpy(new_file->filename, filename);
    snprintf(new_file->content_hash, 50, "hash_%s_%d", filename, rand() % 1000);
    new_file->next = repo->staging_area[index];
    repo->staging_area[index] = new_file;
    
    printf("Added '%s' to staging area.\n", filename);
}

void clear_staging_area() {
    for (int i = 0; i < HASH_SIZE; i++) {
        FileEntry* current = repo->staging_area[i];
        while (current != NULL) {
            FileEntry* temp = current;
            current = current->next;
            free(temp);
        }
        repo->staging_area[i] = NULL;
    }
}

void commit_changes(const char* message) {
    if (repo == NULL) {
        printf("Error: Repository not initialized.\n");
        return;
    }
    
    int empty = 1;
    for(int i = 0; i < HASH_SIZE; i++) {
        if(repo->staging_area[i] != NULL) {
            empty = 0;
            break;
        }
    }
    
    if (empty) {
        printf("Nothing to commit (staging area is empty).\n");
        return;
    }

    CommitNode* new_commit = (CommitNode*)malloc(sizeof(CommitNode));
    new_commit->id = ++repo->commit_count;
    strncpy(new_commit->message, message, MAX_MESSAGE);
    get_current_time(new_commit->timestamp);
    strcpy(new_commit->branch_name, repo->current_branch->name);
    generate_commit_hash(new_commit->hash, new_commit->id, message, new_commit->timestamp);
    
    new_commit->next = repo->head;
    repo->head = new_commit;
    repo->current_branch->commit_head = new_commit;
    repo->current_branch->commit_count++;
    
    printf("[%s %.7s] %s\n", repo->current_branch->name, new_commit->hash, message);
    clear_staging_area();
}

void show_log() {
    if (repo == NULL || repo->head == NULL) {
        printf("No commits yet.\n");
        return;
    }
    
    CommitNode* current = repo->head;
    while (current != NULL) {
        printf("Commit ID: %d\n", current->id);
        printf("Date:      %s\n", current->timestamp);
        printf("Message:   %s\n", current->message);
        printf("|\nv\n");
        current = current->next;
    }
    printf("(Initial State)\n");
}

void show_status() {
    if (repo == NULL) {
        printf("Error: Repository not initialized.\n");
        return;
    }
    
    printf("On branch %s\n", repo->current_branch->name);
    printf("Changes to be committed:\n");
    
    int found = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        FileEntry* current = repo->staging_area[i];
        while (current != NULL) {
            printf("  modified: %s (hash: %s)\n", current->filename, current->content_hash);
            found = 1;
            current = current->next;
        }
    }
    
    if (!found) {
        printf("  (nothing to commit, working tree clean)\n");
    }
}

FileEntry** copy_staging_area() {
    FileEntry** snapshot = (FileEntry**)malloc(HASH_SIZE * sizeof(FileEntry*));
    
    for (int i = 0; i < HASH_SIZE; i++) {
        snapshot[i] = NULL;
        FileEntry* current = repo->staging_area[i];
        FileEntry** dest = &snapshot[i];
        
        while (current != NULL) {
            FileEntry* new_entry = (FileEntry*)malloc(sizeof(FileEntry));
            strcpy(new_entry->filename, current->filename);
            strcpy(new_entry->content_hash, current->content_hash);
            new_entry->next = NULL;
            
            *dest = new_entry;
            dest = &(new_entry->next);
            current = current->next;
        }
    }
    
    return snapshot;
}

void restore_staging_area(FileEntry** snapshot) {
    clear_staging_area();
    
    for (int i = 0; i < HASH_SIZE; i++) {
        FileEntry* current = snapshot[i];
        FileEntry** dest = &repo->staging_area[i];
        
        while (current != NULL) {
            FileEntry* new_entry = (FileEntry*)malloc(sizeof(FileEntry));
            strcpy(new_entry->filename, current->filename);
            strcpy(new_entry->content_hash, current->content_hash);
            new_entry->next = NULL;
            
            *dest = new_entry;
            dest = &(new_entry->next);
            current = current->next;
        }
    }
}

void stash_save(const char* message) {
    if (repo == NULL) {
        printf("Error: Repository not initialized.\n");
        return;
    }
    
    int empty = 1;
    for(int i = 0; i < HASH_SIZE; i++) {
        if(repo->staging_area[i] != NULL) {
            empty = 0;
            break;
        }
    }
    
    if (empty) {
        printf("No local changes to save.\n");
        return;
    }
    
    StashEntry* new_stash = (StashEntry*)malloc(sizeof(StashEntry));
    
    FileEntry** snapshot = copy_staging_area();
    for (int i = 0; i < HASH_SIZE; i++) {
        new_stash->files[i] = snapshot[i];
    }
    free(snapshot);
    
    strncpy(new_stash->message, message, MAX_MESSAGE);
    get_current_time(new_stash->timestamp);
    
    new_stash->next = repo->stash_top;
    repo->stash_top = new_stash;
    repo->stash_count++;
    
    clear_staging_area();
    
    printf("Saved working directory and index state: stash@{%d}: %s\n", 
           repo->stash_count - 1, message);
}

void stash_pop() {
    if (repo == NULL) {
        printf("Error: Repository not initialized.\n");
        return;
    }
    
    if (repo->stash_top == NULL) {
        printf("No stash entries found.\n");
        return;
    }
    
    StashEntry* popped = repo->stash_top;
    repo->stash_top = popped->next;
    repo->stash_count--;
    
    restore_staging_area(popped->files);
    
    printf("Restored stash: %s\n", popped->message);
    
    for (int i = 0; i < HASH_SIZE; i++) {
        FileEntry* current = popped->files[i];
        while (current != NULL) {
            FileEntry* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(popped);
    
    printf("Dropped stash successfully.\n");
}

void stash_list() {
    if (repo == NULL) {
        printf("Error: Repository not initialized.\n");
        return;
    }
    
    if (repo->stash_top == NULL) {
        printf("No stash entries.\n");
        return;
    }
    
    printf("Stash list:\n");
    StashEntry* current = repo->stash_top;
    int index = repo->stash_count - 1;
    
    while (current != NULL) {
        printf("stash@{%d}: %s [%s]\n", index, current->message, current->timestamp);
        printf("  Files:\n");
        for (int i = 0; i < HASH_SIZE; i++) {
            FileEntry* file = current->files[i];
            while (file != NULL) {
                printf("    - %s\n", file->filename);
                file = file->next;
            }
        }
        current = current->next;
        index--;
    }
}

void branch_create(const char* branch_name) {
    if (repo == NULL) {
        printf("Error: Repository not initialized.\n");
        return;
    }
    
    for (int i = 0; i < repo->current_branch->child_count; i++) {
        if (strcmp(repo->current_branch->children[i]->name, branch_name) == 0) {
            printf("Branch '%s' already exists.\n", branch_name);
            return;
        }
    }
    
    if (repo->current_branch->child_count >= 10) {
        printf("Maximum number of child branches reached.\n");
        return;
    }
    
    BranchNode* new_branch = (BranchNode*)malloc(sizeof(BranchNode));
    strcpy(new_branch->name, branch_name);
    new_branch->commit_head = repo->current_branch->commit_head;
    new_branch->commit_count = 0;
    new_branch->child_count = 0;
    new_branch->parent = repo->current_branch;
    
    repo->current_branch->children[repo->current_branch->child_count++] = new_branch;
    
    printf("Created branch '%s' from '%s'\n", branch_name, repo->current_branch->name);
}

void branch_list_helper(BranchNode* node, int depth) {
    if (node == NULL) return;
    
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    
    if (node == repo->current_branch) {
        printf("* %s (current, %d commits)\n", node->name, node->commit_count);
    } else {
        printf("  %s (%d commits)\n", node->name, node->commit_count);
    }
    
    for (int i = 0; i < node->child_count; i++) {
        branch_list_helper(node->children[i], depth + 1);
    }
}

void branch_list() {
    if (repo == NULL) {
        printf("Error: Repository not initialized.\n");
        return;
    }
    
    printf("Branch Tree:\n");
    branch_list_helper(repo->branch_tree, 0);
}

BranchNode* find_branch(BranchNode* node, const char* name) {
    if (node == NULL) return NULL;
    
    if (strcmp(node->name, name) == 0) {
        return node;
    }
    
    for (int i = 0; i < node->child_count; i++) {
        BranchNode* result = find_branch(node->children[i], name);
        if (result != NULL) {
            return result;
        }
    }
    
    return NULL;
}

void branch_checkout(const char* branch_name) {
    if (repo == NULL) {
        printf("Error: Repository not initialized.\n");
        return;
    }
    
    BranchNode* target_branch = find_branch(repo->branch_tree, branch_name);
    
    if (target_branch == NULL) {
        printf("Branch '%s' not found.\n", branch_name);
        return;
    }
    
    if (target_branch == repo->current_branch) {
        printf("Already on '%s'\n", branch_name);
        return;
    }
    
    repo->current_branch = target_branch;
    repo->head = target_branch->commit_head;
    
    printf("Switched to branch '%s'\n", branch_name);
}

void branch_delete(const char* branch_name) {
    if (repo == NULL) {
        printf("Error: Repository not initialized.\n");
        return;
    }
    
    if (strcmp(branch_name, "master") == 0) {
        printf("Cannot delete master branch.\n");
        return;
    }
    
    if (strcmp(repo->current_branch->name, branch_name) == 0) {
        printf("Cannot delete current branch. Switch to another branch first.\n");
        return;
    }
    
    BranchNode* target = find_branch(repo->branch_tree, branch_name);
    if (target == NULL) {
        printf("Branch '%s' not found.\n", branch_name);
        return;
    }
    
    if (target->child_count > 0) {
        printf("Cannot delete branch with children. Delete child branches first.\n");
        return;
    }
    
    BranchNode* parent = target->parent;
    for (int i = 0; i < parent->child_count; i++) {
        if (parent->children[i] == target) {
            for (int j = i; j < parent->child_count - 1; j++) {
                parent->children[j] = parent->children[j + 1];
            }
            parent->child_count--;
            free(target);
            printf("Deleted branch '%s'\n", branch_name);
            return;
        }
    }
}

const char* get_branch_color(const char* branch_name) {
    unsigned int hash = 0;
    const char* p = branch_name;
    while (*p) {
        hash = ((hash << 5) + hash) + *p;
        p++;
    }
    return branch_colors[hash % NUM_COLORS];
}

typedef struct {
    CommitNode* commit;
    BranchNode* branch;
} CommitInfo;

void collect_commits_from_branch(BranchNode* branch, CommitInfo* commits, int* index, int max_size) {
    if (branch == NULL) return;
    
    CommitNode* current = branch->commit_head;
    while (current != NULL && *index < max_size) {
        int found = 0;
        for (int i = 0; i < *index; i++) {
            if (commits[i].commit->id == current->id) {
                found = 1;
                if (strcmp(current->branch_name, branch->name) == 0) {
                    commits[i].branch = branch;
                }
                break;
            }
        }
        if (!found) {
            commits[*index].commit = current;
            if (strcmp(current->branch_name, branch->name) == 0) {
                commits[*index].branch = branch;
            } else {
                BranchNode* orig = find_branch(repo->branch_tree, current->branch_name);
                commits[*index].branch = orig ? orig : branch;
            }
            (*index)++;
        }
        current = current->next;
    }
    
    for (int i = 0; i < branch->child_count; i++) {
        if (branch->children[i] != NULL) {
            collect_commits_from_branch(branch->children[i], commits, index, max_size);
        }
    }
}

int compare_commits(const void* a, const void* b) {
    CommitInfo* ca = (CommitInfo*)a;
    CommitInfo* cb = (CommitInfo*)b;
    return cb->commit->id - ca->commit->id;
}

void show_graph() {
    if (repo == NULL) {
        printf("Error: Repository not initialized.\n");
        return;
    }
    
    if (repo->head == NULL && repo->branch_tree->commit_head == NULL) {
        printf("No commits yet.\n");
        return;
    }
    
    printf("\n");
    printf("%s╔══════════════════════════════════════════════════════════════════╗%s\n", COLOR_BOLD, COLOR_RESET);
    printf("%s║                      GIT COMMIT GRAPH                            ║%s\n", COLOR_BOLD, COLOR_RESET);
    printf("%s╚══════════════════════════════════════════════════════════════════╝%s\n", COLOR_BOLD, COLOR_RESET);
    printf("\n");
    
    int max_commits = 100;
    CommitInfo* commits = (CommitInfo*)malloc(max_commits * sizeof(CommitInfo));
    int commit_count = 0;
    
    collect_commits_from_branch(repo->branch_tree, commits, &commit_count, max_commits);
    
    if (commit_count == 0) {
        printf("No commits to display.\n");
        free(commits);
        return;
    }
    
    qsort(commits, commit_count, sizeof(CommitInfo), compare_commits);
    
    char active_branches[10][MAX_BRANCH_NAME];
    int active_count = 0;
    
    for (int i = 0; i < commit_count; i++) {
        CommitNode* commit = commits[i].commit;
        BranchNode* branch = commits[i].branch;
        const char* color = get_branch_color(branch->name);
        
        int branch_col = -1;
        for (int j = 0; j < active_count; j++) {
            if (strcmp(active_branches[j], branch->name) == 0) {
                branch_col = j;
                break;
            }
        }
        if (branch_col == -1 && active_count < 10) {
            strcpy(active_branches[active_count], branch->name);
            branch_col = active_count;
            active_count++;
        }
        
        for (int j = 0; j < active_count; j++) {
            if (j == branch_col) {
                printf("%s● %s", color, COLOR_RESET);
            } else {
                const char* other_color = get_branch_color(active_branches[j]);
                printf("%s│ %s", other_color, COLOR_RESET);
            }
        }
        
        printf("%s%.7s%s ", COLOR_YELLOW, commit->hash, COLOR_RESET);
        
        if (repo->current_branch->commit_head == commit) {
            printf("%s(%sHEAD -> %s%s%s)%s ", 
                   COLOR_BOLD, COLOR_CYAN, color, branch->name, COLOR_CYAN, COLOR_RESET);
        } else if (branch->commit_head == commit) {
            printf("%s(%s%s%s)%s ", COLOR_BOLD, color, branch->name, COLOR_BOLD, COLOR_RESET);
        }
        
        printf("%s\n", commit->message);
        
        if (i < commit_count - 1) {
            for (int j = 0; j < active_count; j++) {
                const char* line_color = get_branch_color(active_branches[j]);
                printf("%s│ %s", line_color, COLOR_RESET);
            }
            printf("\n");
        }
    }
    
    for (int j = 0; j < active_count; j++) {
        const char* line_color = get_branch_color(active_branches[j]);
        printf("%s│ %s", line_color, COLOR_RESET);
    }
    printf("\n");
    
    for (int j = 0; j < active_count; j++) {
        if (j == 0) {
            const char* line_color = get_branch_color(active_branches[j]);
            printf("%s◯ %s", line_color, COLOR_RESET);
        } else {
            printf("  ");
        }
    }
    printf("%s(Initial State)%s\n\n", COLOR_WHITE, COLOR_RESET);
    
    printf("%sBranches:%s\n", COLOR_BOLD, COLOR_RESET);
    for (int j = 0; j < active_count; j++) {
        const char* bcolor = get_branch_color(active_branches[j]);
        if (strcmp(active_branches[j], repo->current_branch->name) == 0) {
            printf("  %s* %s%s (current)\n", bcolor, active_branches[j], COLOR_RESET);
        } else {
            printf("  %s  %s%s\n", bcolor, active_branches[j], COLOR_RESET);
        }
    }
    printf("\n");
    
    free(commits);
}
