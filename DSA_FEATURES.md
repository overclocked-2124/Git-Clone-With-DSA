# Data Structures & Algorithms in Git-DSA

This document explains the DSA concepts implemented in this Git clone project.

## Overview

This project demonstrates 4 major data structures used in Git operations:

1. **Hash Table** - Staging Area
2. **Linked List** - Commit History
3. **Stack** - Stash Mechanism
4. **Tree** - Branch Management

---

## 1. Hash Table (Staging Area)

### Concept
The staging area uses a hash table with **chaining** for collision resolution. Each file is hashed to an index, and files that hash to the same index are stored in a linked list at that bucket.

### Implementation
- Hash function: `hash = ((hash << 5) + hash) + c` (DJB2 variant)
- Size: 100 buckets
- Collision resolution: Separate chaining using linked lists

### Operations
- **`add <filename>`**: O(1) average case - hash the filename and insert into the bucket
- **`status`**: O(n) - iterate through all buckets and their chains

### Code Location
- [src/git_dsa.c](src/git_dsa.c#L15-L24) - `hash_function()`
- [src/git_dsa.c](src/git_dsa.c#L52-L89) - `add_file()`

---

## 2. Linked List (Commit History)

### Concept
Commits are stored as a singly linked list where each commit points to its parent (previous commit). The `head` pointer always points to the most recent commit.

### Implementation
- **Structure**: `CommitNode` with `next` pointer
- **Insertion**: New commits are inserted at the head (O(1))
- **Traversal**: Follow `next` pointers from head to tail

### Operations
- **`commit <message>`**: O(1) - insert new commit at head
- **`log`**: O(n) - traverse the entire linked list from head

### Code Location
- [src/git_dsa.c](src/git_dsa.c#L91-L137) - `commit_changes()`
- [src/git_dsa.c](src/git_dsa.c#L139-L158) - `show_log()`

---

## 3. Stack (Stash)

### Concept
The stash mechanism uses a **stack** (LIFO - Last In, First Out) to save and restore working directory changes. Each stash entry contains a snapshot of the staging area.

### Implementation
- **Structure**: `StashEntry` with `next` pointer forming a stack
- **Push**: Add new stash at the top (`stash_top`)
- **Pop**: Remove and restore from the top
- **Peek**: View stack contents without removing

### Operations
- **`stash save <message>`**: O(n) - Push operation
  - Copy all staged files (O(n))
  - Create new stash entry
  - Insert at top of stack (O(1))
  - Clear staging area
  
- **`stash pop`**: O(n) - Pop operation
  - Remove top entry from stack (O(1))
  - Restore files to staging area (O(n))
  - Free the stash entry
  
- **`stash list`**: O(k) where k is number of stashes
  - Traverse stack from top to bottom
  - Display each stash with its index, message, and timestamp

### Why Stack?
Stashes follow LIFO order - the most recently saved changes should be restored first, making stack the perfect data structure.

### Code Location
- [src/git_dsa.c](src/git_dsa.c#L229-L271) - `stash_save()`
- [src/git_dsa.c](src/git_dsa.c#L273-L309) - `stash_pop()`
- [src/git_dsa.c](src/git_dsa.c#L311-L340) - `stash_list()`

---

## 4. Tree (Branch Management)

### Concept
Branches are organized in a **tree structure** where:
- Master branch is the root
- New branches are children of their parent branch
- Each branch maintains its own commit history
- Supports hierarchical branch relationships

### Implementation
- **Structure**: `BranchNode` with:
  - `children[]`: Array of child branches (max 10)
  - `parent`: Pointer to parent branch
  - `commit_head`: Pointer to this branch's latest commit
  - `commit_count`: Number of commits in this branch

### Operations

#### `branch create <name>` - Tree Insertion
- Creates a new child node under the current branch
- Initializes with parent's current commit as starting point
- Time Complexity: O(1)

#### `branch list` - Depth-First Search (DFS)
- Recursively traverses the tree using DFS
- Displays hierarchy with indentation
- Shows current branch with asterisk (*)
- Time Complexity: O(b) where b is total branches

#### `branch checkout <name>` - Tree Search
- Recursively searches the tree for the branch
- Switches HEAD to that branch's commit history
- Time Complexity: O(b) - searches all branches

#### `branch delete <name>` - Tree Node Deletion
- Cannot delete:
  - Master branch (root)
  - Current branch
  - Branches with children (to maintain tree integrity)
- Removes node from parent's children array
- Time Complexity: O(c) where c is number of children in parent

### Why Tree?
Branches form a natural hierarchy:
```
master (root)
  ├── feature-1
  │   └── feature-1-bugfix
  └── feature-2
```
Trees allow organizing related features and tracking branch lineage.

### Code Location
- [src/git_dsa.c](src/git_dsa.c#L342-L371) - `branch_create()`
- [src/git_dsa.c](src/git_dsa.c#L373-L398) - `branch_list()` and DFS helper
- [src/git_dsa.c](src/git_dsa.c#L400-L439) - `branch_checkout()` with tree search
- [src/git_dsa.c](src/git_dsa.c#L441-L485) - `branch_delete()` with node removal

---

## Usage Examples

### Stash Example
```bash
git-dsa> init
git-dsa> add file1.txt
git-dsa> add file2.txt
git-dsa> stash save WIP: working on feature
git-dsa> status                    # Clean working tree
git-dsa> stash list                # Shows stashed changes
git-dsa> stash pop                 # Restores file1.txt and file2.txt
```

### Branch Example
```bash
git-dsa> init
git-dsa> branch list               # Shows: * master
git-dsa> branch create feature-1
git-dsa> branch checkout feature-1
git-dsa> add newfile.txt
git-dsa> commit Added new feature
git-dsa> branch list               # Shows tree structure
Branch Tree:
  master (0 commits)
    * feature-1 (current, 1 commits)
```

---

## Time Complexity Summary

| Operation | Data Structure | Average Case | Worst Case |
|-----------|---------------|--------------|------------|
| add | Hash Table | O(1) | O(n) |
| commit | Linked List | O(1) | O(1) |
| log | Linked List | O(n) | O(n) |
| status | Hash Table | O(n) | O(n) |
| stash save | Stack | O(n) | O(n) |
| stash pop | Stack | O(n) | O(n) |
| stash list | Stack | O(k) | O(k) |
| branch create | Tree | O(1) | O(1) |
| branch list | Tree (DFS) | O(b) | O(b) |
| branch checkout | Tree | O(b) | O(b) |
| branch delete | Tree | O(c) | O(c) |

Where:
- n = number of files
- k = number of stashes
- b = total number of branches
- c = number of children in parent branch

---

## Memory Management

All data structures are dynamically allocated:
- Hash table buckets are freed when staging area is cleared
- Commit nodes persist for the lifetime of the repository
- Stash entries are freed when popped
- Branch nodes are freed when deleted (if no children)

---

## Educational Value

This project demonstrates:
1. **Hash Tables**: Fast lookups with collision handling
2. **Linked Lists**: Sequential data with dynamic size
3. **Stacks**: LIFO behavior for temporary storage
4. **Trees**: Hierarchical relationships and traversal algorithms
5. **Memory Management**: Dynamic allocation and deallocation
6. **Real-world Applications**: How Git actually uses these data structures
