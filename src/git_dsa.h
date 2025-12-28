#ifndef GIT_DSA_H
#define GIT_DSA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_FILENAME 256
#define MAX_MESSAGE 512
#define HASH_SIZE 100
#define MAX_BRANCH_NAME 50

typedef struct CommitNode {
    int id;
    char hash[41];
    char message[MAX_MESSAGE];
    char timestamp[50];
    char branch_name[MAX_BRANCH_NAME];
    struct CommitNode* next;
} CommitNode;

typedef struct FileEntry {
    char filename[MAX_FILENAME];
    char content_hash[50];
    struct FileEntry* next;
} FileEntry;

typedef struct StashEntry {
    FileEntry* files[HASH_SIZE];
    char message[MAX_MESSAGE];
    char timestamp[50];
    struct StashEntry* next;
} StashEntry;

typedef struct BranchNode {
    char name[MAX_BRANCH_NAME];
    CommitNode* commit_head;
    int commit_count;
    struct BranchNode* children[10];
    int child_count;
    struct BranchNode* parent;
} BranchNode;

typedef struct Repository {
    CommitNode* head;
    FileEntry* staging_area[HASH_SIZE];
    int commit_count;
    StashEntry* stash_top;
    int stash_count;
    BranchNode* branch_tree;
    BranchNode* current_branch;
} Repository;

extern Repository* repo;

void init_repo();
void add_file(const char* filename);
void commit_changes(const char* message);
void show_log();
void show_status();

void stash_save(const char* message);
void stash_pop();
void stash_list();

void branch_create(const char* branch_name);
void branch_list();
void branch_checkout(const char* branch_name);
void branch_delete(const char* branch_name);

void show_graph();

unsigned int hash_function(const char* str);
void get_current_time(char* buffer);
void generate_commit_hash(char* hash, int id, const char* message, const char* timestamp);
FileEntry** copy_staging_area();
void clear_staging_area();
void restore_staging_area(FileEntry** snapshot);

#endif
