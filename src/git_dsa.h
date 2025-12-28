#ifndef GIT_DSA_H
#define GIT_DSA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_FILENAME 256
#define MAX_MESSAGE 512
#define HASH_SIZE 100 // Simple hash table size
#define MAX_BRANCH_NAME 50

// DSA Concept: Linked List Node for Commits
// Each commit points to the previous commit (parent)
typedef struct CommitNode {
    int id;
    char hash[41];  // SHA-1 like hash (40 chars + null terminator)
    char message[MAX_MESSAGE];
    char timestamp[50];
    char branch_name[MAX_BRANCH_NAME]; // Branch where commit was made
    struct CommitNode* next; // Pointer to the parent commit
} CommitNode;

// DSA Concept: Hash Table Entry for Staging Area
// We use a hash table to store files added to the staging area for O(1) access (ideally)
typedef struct FileEntry {
    char filename[MAX_FILENAME];
    char content_hash[50]; // Simulating a hash of the content
    struct FileEntry* next; // Chaining for collision resolution
} FileEntry;

// DSA Concept: Stack for Stash
// Each stash entry contains a snapshot of the staging area
typedef struct StashEntry {
    FileEntry* files[HASH_SIZE]; // Snapshot of staging area
    char message[MAX_MESSAGE];
    char timestamp[50];
    struct StashEntry* next; // Points to the next stash (deeper in stack)
} StashEntry;

// DSA Concept: Tree Node for Branches
// Each branch is a node in a tree structure
typedef struct BranchNode {
    char name[MAX_BRANCH_NAME];
    CommitNode* commit_head; // Head commit of this branch
    int commit_count;
    struct BranchNode* children[10]; // Child branches (max 10 for simplicity)
    int child_count;
    struct BranchNode* parent; // Parent branch
} BranchNode;

// The Repository structure
typedef struct Repository {
    CommitNode* head; // Head of the linked list (latest commit)
    FileEntry* staging_area[HASH_SIZE]; // Hash table for staging files
    int commit_count;
    StashEntry* stash_top; // Top of the stash stack
    int stash_count;
    BranchNode* branch_tree; // Root of the branch tree (master branch)
    BranchNode* current_branch; // Currently checked out branch
} Repository;

// Global repository instance (simulating a singleton for simplicity)
extern Repository* repo;

// Function Prototypes
void init_repo();
void add_file(const char* filename);
void commit_changes(const char* message);
void show_log();
void show_status();

// Stash operations (Stack)
void stash_save(const char* message);
void stash_pop();
void stash_list();

// Branch operations (Tree)
void branch_create(const char* branch_name);
void branch_list();
void branch_checkout(const char* branch_name);
void branch_delete(const char* branch_name);

// Graph visualization
void show_graph();

// Helper functions
unsigned int hash_function(const char* str);
void get_current_time(char* buffer);
void generate_commit_hash(char* hash, int id, const char* message, const char* timestamp);
FileEntry** copy_staging_area();
void clear_staging_area();
void restore_staging_area(FileEntry** snapshot);

#endif
