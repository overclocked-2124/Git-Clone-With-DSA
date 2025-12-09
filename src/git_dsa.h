#ifndef GIT_DSA_H
#define GIT_DSA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_FILENAME 256
#define MAX_MESSAGE 512
#define HASH_SIZE 100 // Simple hash table size

// DSA Concept: Linked List Node for Commits
// Each commit points to the previous commit (parent)
typedef struct CommitNode {
    int id;
    char message[MAX_MESSAGE];
    char timestamp[50];
    struct CommitNode* next; // Pointer to the parent commit
} CommitNode;

// DSA Concept: Hash Table Entry for Staging Area
// We use a hash table to store files added to the staging area for O(1) access (ideally)
typedef struct FileEntry {
    char filename[MAX_FILENAME];
    char content_hash[50]; // Simulating a hash of the content
    struct FileEntry* next; // Chaining for collision resolution
} FileEntry;

// The Repository structure
typedef struct Repository {
    CommitNode* head; // Head of the linked list (latest commit)
    FileEntry* staging_area[HASH_SIZE]; // Hash table for staging files
    int commit_count;
} Repository;

// Global repository instance (simulating a singleton for simplicity)
extern Repository* repo;

// Function Prototypes
void init_repo();
void add_file(const char* filename);
void commit_changes(const char* message);
void show_log();
void show_status();

// Helper functions
unsigned int hash_function(const char* str);
void get_current_time(char* buffer);

#endif
