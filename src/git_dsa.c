#include "git_dsa.h"

Repository* repo = NULL;

// Helper: Get current time string
void get_current_time(char* buffer) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(buffer, 50, "%Y-%m-%d %H:%M:%S", tm);
}

// DSA Concept: Hashing
// A simple hash function to map filenames to an index in the hash table
unsigned int hash_function(const char* str) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash % HASH_SIZE;
}

// Command 1: Init
// Initializes the repository structure
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
    
    // Initialize hash table buckets to NULL
    for (int i = 0; i < HASH_SIZE; i++) {
        repo->staging_area[i] = NULL;
    }
    
    // Initialize stash stack
    repo->stash_top = NULL;
    repo->stash_count = 0;
    
    // Initialize branch tree with master branch
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

// Command 2: Add
// Adds a file to the staging area using a Hash Table
void add_file(const char* filename) {
    if (repo == NULL) {
        printf("Error: Repository not initialized. Run 'init' first.\n");
        return;
    }

    unsigned int index = hash_function(filename);
    
    // Check if file already exists in staging (Update it)
    FileEntry* current = repo->staging_area[index];
    while (current != NULL) {
        if (strcmp(current->filename, filename) == 0) {
            printf("File '%s' is already in staging area. Updating...\n", filename);
            // In a real git, we would hash the content. Here we just simulate it.
            snprintf(current->content_hash, 50, "hash_%s_%d", filename, rand() % 1000);
            return;
        }
        current = current->next;
    }
    
    // DSA Concept: Collision Resolution via Chaining (Linked List)
    // If the bucket is not empty, we add the new entry to the linked list at this index
    FileEntry* new_file = (FileEntry*)malloc(sizeof(FileEntry));
    strcpy(new_file->filename, filename);
    snprintf(new_file->content_hash, 50, "hash_%s_%d", filename, rand() % 1000);
    
    // Insert at head of the bucket's linked list (O(1))
    new_file->next = repo->staging_area[index];
    repo->staging_area[index] = new_file;
    
    printf("Added '%s' to staging area.\n", filename);
}

// Command 3: Commit
// Creates a new commit node and adds it to the history Linked List
void commit_changes(const char* message) {
    if (repo == NULL) {
        printf("Error: Repository not initialized.\n");
        return;
    }
    
    // Check if staging is empty (simple check)
    int empty = 1;
    for(int i=0; i<HASH_SIZE; i++) {
        if(repo->staging_area[i] != NULL) {
            empty = 0;
            break;
        }
    }
    
    if (empty) {
        printf("Nothing to commit (staging area is empty).\n");
        return;
    }

    // DSA Concept: Linked List Insertion
    // We insert the new commit at the beginning of the list (Head)
    // This makes 'head' always point to the latest commit
    CommitNode* new_commit = (CommitNode*)malloc(sizeof(CommitNode));
    new_commit->id = ++repo->commit_count;
    strncpy(new_commit->message, message, MAX_MESSAGE);
    get_current_time(new_commit->timestamp);
    
    // Point to the previous head (Parent commit)
    new_commit->next = repo->head;
    
    // Update head to the new commit
    repo->head = new_commit;
    
    // Also update current branch's head
    repo->current_branch->commit_head = new_commit;
    repo->current_branch->commit_count++;
    
    printf("[%s %d] %s\n", repo->current_branch->name, new_commit->id, message);
    
    // Clear the staging area to show we "moved" them to the commit history.
    clear_staging_area();
}

// Command 4: Log
// Traverses the Linked List of commits to show history
void show_log() {
    if (repo == NULL || repo->head == NULL) {
        printf("No commits yet.\n");
        return;
    }
    
    CommitNode* current = repo->head;
    
    // DSA Concept: Linked List Traversal
    while (current != NULL) {
        printf("Commit ID: %d\n", current->id);
        printf("Date:      %s\n", current->timestamp);
        printf("Message:   %s\n", current->message);
        printf("|\n");
        printf("v\n");
        current = current->next;
    }
    printf("(Initial State)\n");
}

// Command 5: Status
// Iterates through the Hash Table to show staged files
void show_status() {
    if (repo == NULL) {
        printf("Error: Repository not initialized.\n");
        return;
    }
    
    printf("On branch %s\n", repo->current_branch->name);
    printf("Changes to be committed:\n");
    
    int found = 0;
    // DSA Concept: Hash Table Iteration
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

// Helper: Copy staging area for stash
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

// Helper: Clear staging area
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

// Helper: Restore staging area from snapshot
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

// === STASH OPERATIONS (STACK) ===

// Command 6: Stash Save
// DSA Concept: Stack Push
// Saves the current staging area onto the stash stack
void stash_save(const char* message) {
    if (repo == NULL) {
        printf("Error: Repository not initialized.\n");
        return;
    }
    
    // Check if staging is empty
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
    
    // Create new stash entry
    StashEntry* new_stash = (StashEntry*)malloc(sizeof(StashEntry));
    
    // Copy staging area to stash
    FileEntry** snapshot = copy_staging_area();
    for (int i = 0; i < HASH_SIZE; i++) {
        new_stash->files[i] = snapshot[i];
    }
    free(snapshot);
    
    strncpy(new_stash->message, message, MAX_MESSAGE);
    get_current_time(new_stash->timestamp);
    
    // Push onto stack (insert at top)
    new_stash->next = repo->stash_top;
    repo->stash_top = new_stash;
    repo->stash_count++;
    
    // Clear staging area
    clear_staging_area();
    
    printf("Saved working directory and index state: stash@{%d}: %s\n", 
           repo->stash_count - 1, message);
}

// Command 7: Stash Pop
// DSA Concept: Stack Pop
// Restores the most recent stash and removes it from stack
void stash_pop() {
    if (repo == NULL) {
        printf("Error: Repository not initialized.\n");
        return;
    }
    
    if (repo->stash_top == NULL) {
        printf("No stash entries found.\n");
        return;
    }
    
    // Pop from stack (remove from top)
    StashEntry* popped = repo->stash_top;
    repo->stash_top = popped->next;
    repo->stash_count--;
    
    // Restore staging area from stash
    restore_staging_area(popped->files);
    
    printf("Restored stash: %s\n", popped->message);
    
    // Free the stash entry and its files
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

// Command 8: Stash List
// DSA Concept: Stack Traversal
// Shows all stashed changes
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
        
        // Show files in this stash
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

// === BRANCH OPERATIONS (TREE) ===

// Command 9: Branch Create
// DSA Concept: Tree Node Insertion
// Creates a new branch as a child of the current branch
void branch_create(const char* branch_name) {
    if (repo == NULL) {
        printf("Error: Repository not initialized.\n");
        return;
    }
    
    // Check if branch name already exists (simple search)
    // We'll do a basic check on current branch's children
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
    
    // Create new branch node
    BranchNode* new_branch = (BranchNode*)malloc(sizeof(BranchNode));
    strcpy(new_branch->name, branch_name);
    new_branch->commit_head = repo->current_branch->commit_head; // Start from current commit
    new_branch->commit_count = 0;
    new_branch->child_count = 0;
    new_branch->parent = repo->current_branch;
    
    // Add as child to current branch (tree insertion)
    repo->current_branch->children[repo->current_branch->child_count++] = new_branch;
    
    printf("Created branch '%s' from '%s'\n", branch_name, repo->current_branch->name);
}

// Command 10: Branch List
// DSA Concept: Tree Traversal (DFS)
// Lists all branches in the tree
void branch_list_helper(BranchNode* node, int depth) {
    if (node == NULL) return;
    
    // Print current branch with indentation
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    
    if (node == repo->current_branch) {
        printf("* %s (current, %d commits)\n", node->name, node->commit_count);
    } else {
        printf("  %s (%d commits)\n", node->name, node->commit_count);
    }
    
    // Recursively list children (DFS traversal)
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

// Command 11: Branch Checkout
// DSA Concept: Tree Search
// Switches to a different branch
BranchNode* find_branch(BranchNode* node, const char* name) {
    if (node == NULL) return NULL;
    
    if (strcmp(node->name, name) == 0) {
        return node;
    }
    
    // Search in children
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
    
    // Find the branch in the tree
    BranchNode* target_branch = find_branch(repo->branch_tree, branch_name);
    
    if (target_branch == NULL) {
        printf("Branch '%s' not found.\n", branch_name);
        return;
    }
    
    if (target_branch == repo->current_branch) {
        printf("Already on '%s'\n", branch_name);
        return;
    }
    
    // Switch branch
    repo->current_branch = target_branch;
    repo->head = target_branch->commit_head;
    repo->commit_count = target_branch->commit_count;
    
    printf("Switched to branch '%s'\n", branch_name);
}

// Command 12: Branch Delete
// DSA Concept: Tree Node Deletion
// Deletes a branch from the tree
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
    
    // Find and delete from parent's children
    BranchNode* target = find_branch(repo->branch_tree, branch_name);
    if (target == NULL) {
        printf("Branch '%s' not found.\n", branch_name);
        return;
    }
    
    if (target->child_count > 0) {
        printf("Cannot delete branch with children. Delete child branches first.\n");
        return;
    }
    
    // Remove from parent's children array
    BranchNode* parent = target->parent;
    for (int i = 0; i < parent->child_count; i++) {
        if (parent->children[i] == target) {
            // Shift remaining children
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
