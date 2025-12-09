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
    
    printf("Initialized empty Git repository (in memory).\n");
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
    
    printf("[master %d] %s\n", new_commit->id, message);
    
    // Note: In a real git, 'add' puts things in index, 'commit' saves them to tree.
    // Here we keep files in staging for simplicity of the 'status' command demo,
    // or we could clear them. Let's clear them to simulate a clean working directory after commit.
    // But wait, usually 'add' stages changes. If I clear them, 'status' will be empty.
    // Let's clear the staging area to show we "moved" them to the commit history.
    
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
    
    printf("On branch master\n");
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
