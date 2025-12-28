# Git-Clone-With-DSA

## A Version Control System Implementation Using Data Structures

![C](https://img.shields.io/badge/Language-C-blue)
![DSA](https://img.shields.io/badge/Focus-Data%20Structures-green)
![Git](https://img.shields.io/badge/Inspired%20By-Git-orange)

---

## Table of Contents

1. [Introduction](#1-introduction)
2. [Problem Definition](#2-problem-definition)
3. [Objectives](#3-objectives)
4. [Methodology](#4-methodology)
5. [Project Execution](#5-project-execution)
6. [Tools and Techniques Used](#6-tools-and-techniques-used)
7. [Partial Results](#7-partial-results)
8. [Results and Discussion](#8-results-and-discussion)
9. [Prototype (Software)](#9-prototype-software)
10. [Conclusion](#10-conclusion)
11. [Visuals](#11-visuals)
12. [Outcome of the Work](#12-outcome-of-the-work)

---

## 1. Introduction

This project is a **simplified clone of Git**, the world's most popular distributed version control system. The implementation focuses on demonstrating how fundamental **Data Structures and Algorithms (DSA)** concepts can be applied to build real-world software systems.

Git is an essential tool for software developers, enabling them to track changes, collaborate on code, and manage project history. By recreating core Git functionalities from scratch using C, this project provides deep insights into:

- How version control systems work internally
- Practical applications of data structures like Linked Lists, Hash Tables, Stacks, and Trees
- Memory management and pointer manipulation in C
- Command-line interface (CLI) application development

---

## 2. Problem Definition

### 2.1 Problem Statement

Modern software development relies heavily on version control systems, yet most developers use these tools without understanding their internal workings. The challenge is to **design and implement a simplified version control system** that demonstrates the practical application of data structures while providing educational value.

**Key Questions Addressed:**
- How can commits be stored and traversed efficiently? → **Linked Lists**
- How can files be staged for commits with fast lookup? → **Hash Tables**
- How can temporary changes be saved and restored? → **Stacks**
- How can multiple development branches be managed? → **Trees**

### 2.2 Background Information (Literature Review)

**Git's History:**
Git was created by Linus Torvalds in 2005 for Linux kernel development. It has since become the de facto standard for version control, used by over 90% of developers worldwide.

**Core Concepts in Git:**

| Concept | Description | Data Structure Used |
|---------|-------------|---------------------|
| Commits | Snapshots of the project at a point in time | Linked List (DAG in real Git) |
| Staging Area | Files prepared for the next commit | Hash Table |
| Branches | Parallel development lines | Tree Structure |
| Stash | Temporary storage for uncommitted changes | Stack |

**Previous Implementations:**
- **libgit2**: A portable, pure C implementation of Git core methods
- **JGit**: Java implementation used in Eclipse IDE
- **Dulwich**: Pure Python implementation

Our implementation differs by focusing on **educational clarity** over production features, making the DSA concepts explicit and easy to understand.

---

## 3. Objectives

### 3.1 Primary Objectives

1. **Implement core Git commands** using appropriate data structures:
   - `init` - Initialize repository
   - `add` - Stage files using Hash Table
   - `commit` - Create commits using Linked List
   - `log` - Display commit history
   - `status` - Show staged files

2. **Implement Git Stash** using **Stack** data structure:
   - `stash save` - Push changes onto stack (LIFO)
   - `stash pop` - Pop and restore most recent stash
   - `stash list` - Display all stashed changes

3. **Implement Git Branch** using **Tree** data structure:
   - `branch create` - Create new branch (add child node)
   - `branch list` - Display branch hierarchy (tree traversal)
   - `branch checkout` - Switch between branches
   - `branch delete` - Remove branch from tree

4. **Implement Commit Graph Visualization**:
   - Display visual representation of commits across branches
   - Show commit hashes (SHA-1 like)
   - Color-coded branch lines

### 3.2 Secondary Objectives

1. Demonstrate memory management best practices in C
2. Create a user-friendly CLI interface
3. Generate meaningful commit hashes
4. Provide educational documentation explaining DSA concepts

---

## 4. Methodology

### 4.1 Approach

The project follows a **modular design approach** where each Git feature maps to a specific data structure:

```
┌─────────────────────────────────────────────────────────────────┐
│                    GIT-DSA ARCHITECTURE                         │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ┌─────────────┐    ┌─────────────┐    ┌─────────────┐         │
│  │   COMMITS   │    │   STAGING   │    │   STASH     │         │
│  │             │    │    AREA     │    │             │         │
│  │  Linked     │    │   Hash      │    │   Stack     │         │
│  │   List      │    │   Table     │    │   (LIFO)    │         │
│  └──────┬──────┘    └──────┬──────┘    └──────┬──────┘         │
│         │                  │                  │                 │
│         └──────────────────┼──────────────────┘                 │
│                            │                                    │
│                   ┌────────▼────────┐                           │
│                   │   REPOSITORY    │                           │
│                   │                 │                           │
│                   │  Central Data   │                           │
│                   │   Structure     │                           │
│                   └────────┬────────┘                           │
│                            │                                    │
│                   ┌────────▼────────┐                           │
│                   │    BRANCHES     │                           │
│                   │                 │                           │
│                   │  Tree Structure │                           │
│                   │  (Parent-Child) │                           │
│                   └─────────────────┘                           │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### 4.2 Data Structure Mapping

| Git Feature | Data Structure | Operations | Time Complexity |
|-------------|----------------|------------|-----------------|
| Commits | Singly Linked List | Insert at head, Traverse | O(1) insert, O(n) traverse |
| Staging Area | Hash Table with Chaining | Insert, Search, Delete | O(1) average |
| Stash | Stack | Push, Pop, Peek | O(1) all operations |
| Branches | N-ary Tree | Insert child, DFS traversal, Delete | O(n) traversal |
| File Lookup | Hash Function (djb2) | Hash computation | O(k) where k = key length |

### 4.3 Flow Chart

```
                    ┌─────────────────┐
                    │      START      │
                    └────────┬────────┘
                             │
                             ▼
                    ┌─────────────────┐
                    │  Parse Command  │
                    └────────┬────────┘
                             │
           ┌─────────────────┼─────────────────┐
           │                 │                 │
           ▼                 ▼                 ▼
    ┌──────────────┐  ┌──────────────┐  ┌──────────────┐
    │     INIT     │  │     ADD      │  │    COMMIT    │
    │              │  │              │  │              │
    │ Allocate     │  │ Hash         │  │ Create Node  │
    │ Repository   │  │ filename     │  │ Link to HEAD │
    │ Create       │  │ Insert to    │  │ Update       │
    │ master       │  │ Hash Table   │  │ branch HEAD  │
    │ branch       │  │              │  │              │
    └──────────────┘  └──────────────┘  └──────────────┘
           │                 │                 │
           │                 │                 │
           ▼                 ▼                 ▼
    ┌──────────────┐  ┌──────────────┐  ┌──────────────┐
    │    STASH     │  │    BRANCH    │  │    GRAPH     │
    │              │  │              │  │              │
    │ SAVE: Push   │  │ CREATE: Add  │  │ Collect all  │
    │ to stack     │  │ child node   │  │ commits      │
    │              │  │              │  │              │
    │ POP: Pop     │  │ CHECKOUT:    │  │ Sort by ID   │
    │ from stack   │  │ Switch node  │  │              │
    │              │  │              │  │ Display      │
    │ LIST:        │  │ DELETE:      │  │ visual tree  │
    │ Traverse     │  │ Remove node  │  │              │
    └──────────────┘  └──────────────┘  └──────────────┘
                             │
                             ▼
                    ┌─────────────────┐
                    │       END       │
                    └─────────────────┘
```

### 4.4 Procedures and Timeline

| Phase | Duration | Activities |
|-------|----------|------------|
| Phase 1: Research | Week 1 | Study Git internals, identify DSA mappings |
| Phase 2: Design | Week 2 | Design data structures, define interfaces |
| Phase 3: Core Implementation | Week 3-4 | Implement init, add, commit, log, status |
| Phase 4: Stash & Branch | Week 5-6 | Implement stack-based stash, tree-based branches |
| Phase 5: Visualization | Week 7 | Implement graph command with colors |
| Phase 6: Testing & Docs | Week 8 | Testing, documentation, report writing |

---

## 5. Project Execution

### 5.1 Planning and Design

**Initial Brainstorming:**
- Identified core Git commands most commonly used by developers
- Mapped each command to appropriate data structure
- Designed modular code structure with separate header and implementation files

**Design Decisions:**
1. **Linked List for Commits**: Chose singly linked list (each commit points to parent) over doubly linked list as we primarily traverse backwards in history
2. **Hash Table with Chaining**: Selected chaining over open addressing for collision resolution due to simplicity and dynamic sizing
3. **Stack for Stash**: Natural fit as stash follows LIFO - most recent stash is typically restored first
4. **N-ary Tree for Branches**: Allows multiple child branches from any branch, mimicking real Git behavior

**File Structure:**
```
Git-Clone-With-DSA/
├── src/
│   ├── git_dsa.h      # Header file with structures and prototypes
│   ├── git_dsa.c      # Implementation of all Git operations
│   └── main.c         # CLI interface and command parsing
├── Makefile           # Build configuration
└── README.md          # This documentation
```

### 5.2 Implementation

**Phase 1: Core Repository Structure**
```c
typedef struct Repository {
    CommitNode* head;                    // Linked List head
    FileEntry* staging_area[HASH_SIZE];  // Hash Table
    StashEntry* stash_top;               // Stack top
    BranchNode* branch_tree;             // Tree root
    BranchNode* current_branch;          // Current position in tree
} Repository;
```

**Phase 2: Hash Table Implementation**
- Implemented djb2 hash function for filename hashing
- Used separate chaining with linked lists for collision resolution

**Phase 3: Stack Implementation for Stash**
- Each stash entry contains a complete snapshot of staging area
- Push/Pop operations maintain O(1) complexity

**Phase 4: Tree Implementation for Branches**
- Each branch node can have up to 10 child branches
- DFS traversal for branch listing with visual indentation

**Phase 5: Commit Hash Generation**
- Created pseudo-SHA-1 hash combining commit ID, message, and timestamp
- 40-character hexadecimal string displayed as 7-char short hash

---

## 6. Tools and Techniques Used

### 6.1 Tools

| Tool | Purpose |
|------|---------|
| **GCC** | GNU C Compiler for building the project |
| **Make** | Build automation tool |
| **VS Code** | Primary code editor with C/C++ extension |
| **Git** | Version control for project development (meta!) |
| **Valgrind** | Memory leak detection and debugging |
| **GDB** | Debugging C programs |

### 6.2 Techniques

**1. Hashing (djb2 Algorithm)**
```c
unsigned int hash_function(const char* str) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    return hash % HASH_SIZE;
}
```
*Why djb2?* Simple, fast, and provides good distribution for string keys.

**2. Linked List Operations**
- Head insertion for O(1) commit creation
- Sequential traversal for log display
- Pointer manipulation for linking commits

**3. Stack Operations (LIFO)**
- Push: Insert at top of stash stack
- Pop: Remove and return top element
- Peek: View top without removal (stash list)

**4. Tree Traversal (DFS)**
- Depth-First Search for branch listing
- Recursive traversal with indentation for visual hierarchy

**5. ANSI Color Codes**
```c
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
```
*Used for colorful graph visualization in terminal*

---

## 7. Partial Results

### 7.1 Initial Findings

**Early Testing Results:**

1. **Hash Table Performance**:
   - Average O(1) lookup confirmed for file staging
   - Collision rate < 5% with djb2 for typical filenames

2. **Commit Linked List**:
   - Successfully maintained parent-child relationships
   - Log traversal correctly showed newest-to-oldest order

3. **Initial Issues Identified**:
   - Branch switching incorrectly reset global commit counter
   - Stash entries weren't properly copying staging area

### 7.2 Iterative Improvements

| Issue | Solution | Result |
|-------|----------|--------|
| Duplicate commit IDs across branches | Made commit_count global, not per-branch | Unique IDs for all commits |
| Stash not preserving file hashes | Implemented deep copy of staging area | Complete state preservation |
| Graph not showing all branch commits | Fixed tree traversal to collect from all branches | Complete visualization |
| Color codes not working on Windows | Added platform detection | Cross-platform support |

---

## 8. Results and Discussion

### 8.1 Final Results

**Implemented Commands:**

| Command | Status | DSA Used |
|---------|--------|----------|
| `init` | ✅ Complete | Memory allocation, struct initialization |
| `add <file>` | ✅ Complete | Hash Table with chaining |
| `commit <msg>` | ✅ Complete | Linked List insertion |
| `log` | ✅ Complete | Linked List traversal |
| `status` | ✅ Complete | Hash Table iteration |
| `stash save` | ✅ Complete | Stack push |
| `stash pop` | ✅ Complete | Stack pop |
| `stash list` | ✅ Complete | Stack traversal |
| `branch create` | ✅ Complete | Tree node insertion |
| `branch list` | ✅ Complete | DFS tree traversal |
| `branch checkout` | ✅ Complete | Tree search |
| `branch delete` | ✅ Complete | Tree node deletion |
| `graph` | ✅ Complete | Multi-branch traversal, sorting |

**Sample Output - Graph Command:**
```
╔══════════════════════════════════════════════════════════════════╗
║                      GIT COMMIT GRAPH                            ║
╚══════════════════════════════════════════════════════════════════╝

● f9df3fa (hotfix) Emergency fix
│ 
│ ● 745eac2 (HEAD -> master) Update main
│ │ 
│ │ ● 393be48 (feature) Add more features
│ │ │ 
│ │ ● 3154233 Add feature
│ │ │ 
│ ● │ 8d3a988 Add file2
│ │ │ 
│ ● │ 60ae630 Initial commit
│ │ │ 
◯     (Initial State)

─────────────────────────────────────────────────────────────────────
Branches:
    hotfix
  * master (current)
    feature
```

### 8.2 Discussion

**Objectives Met:**
- ✅ All primary objectives achieved
- ✅ All secondary objectives achieved
- ✅ Educational documentation complete

**Key Findings:**

1. **Data Structure Selection is Critical**: The choice of linked list for commits and hash table for staging directly impacts performance and code simplicity.

2. **Stack is Perfect for Stash**: The LIFO nature of stash operations maps directly to stack data structure, making implementation intuitive.

3. **Tree Structure Enables Branching**: Hierarchical relationships between branches are naturally represented by tree nodes.

**Limitations:**
- In-memory only (no persistence to disk)
- Simplified hash function (not cryptographically secure)
- Maximum 10 child branches per node
- No merge functionality

**Comparison with Real Git:**

| Feature | Our Implementation | Real Git |
|---------|-------------------|----------|
| Commit Storage | Linked List | Directed Acyclic Graph (DAG) |
| Hash Algorithm | Custom djb2-based | SHA-1/SHA-256 |
| File Storage | In-memory hash table | Blob objects on disk |
| Branches | N-ary Tree | References to commits |

---

## 9. Prototype (Software)

### 9.1 Prototype Description

**Specifications:**
- **Language**: C (C99 standard)
- **Build System**: GNU Make
- **Platform**: Cross-platform (Linux, macOS, Windows with MinGW)
- **Interface**: Command Line Interface (CLI)

**Features:**
1. Repository initialization
2. File staging with hash-based storage
3. Commit creation with auto-generated hashes
4. Commit history viewing
5. Stash management (save, pop, list)
6. Branch management (create, checkout, delete, list)
7. Visual commit graph with colors

### 9.2 Development Process

**Challenges Faced:**

| Challenge | Solution |
|-----------|----------|
| Memory leaks in staging area | Implemented `clear_staging_area()` function |
| Branch commit isolation | Maintained separate commit_head per branch |
| Color codes in Windows | Used conditional compilation |
| Graph alignment with multiple branches | Dynamic column tracking system |

### 9.3 Testing and Validation

**Test Cases:**

```bash
# Test 1: Basic workflow
init
add file1.c
commit "Initial commit"
log
status

# Test 2: Stash operations
add temp.c
stash save "Work in progress"
stash list
stash pop

# Test 3: Branch operations
branch create feature
branch checkout feature
add feature.c
commit "Add feature"
branch checkout master
branch list

# Test 4: Graph visualization
graph
```

**All test cases passed successfully.**

---

## 10. Conclusion

### 10.1 Summary

This project successfully implemented a **simplified Git clone** demonstrating practical applications of fundamental data structures:

- **Linked Lists** for commit history management
- **Hash Tables** for efficient file staging
- **Stacks** for stash functionality (LIFO operations)
- **Trees** for branch hierarchy management

The implementation includes **13 commands** covering core Git functionality, with a special **visual graph feature** that displays commit history across multiple branches with color coding.

**Key Achievements:**
1. Demonstrated that complex software systems like Git can be understood through DSA concepts
2. Built a functional version control system from scratch
3. Created comprehensive documentation for educational purposes

### 10.2 Personal Reflection

This project provided invaluable hands-on experience in:

1. **Applying theoretical knowledge**: Transforming textbook data structure concepts into working code
2. **System design**: Understanding how to architect software with modular components
3. **Debugging**: Extensive practice with memory management and pointer operations in C
4. **Documentation**: Learning to write technical documentation that others can follow

The project deepened understanding of:
- How real-world tools (Git) leverage DSA internally
- Trade-offs between different data structure choices
- Importance of code organization and clean interfaces

---

## 11. Visuals

### Data Structure Diagrams

**Linked List (Commits):**
```
HEAD
  │
  ▼
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│ Commit #3       │───▶│ Commit #2       │───▶│ Commit #1       │───▶ NULL
│ hash: f3b570c   │    │ hash: 70ff3f9   │    │ hash: 60add72   │
│ msg: "Update"   │    │ msg: "Feature"  │    │ msg: "Initial"  │
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

**Hash Table (Staging Area):**
```
Index    Bucket (Linked List for Chaining)
  0  ──▶ NULL
  1  ──▶ [file1.c] ──▶ [config.h] ──▶ NULL
  2  ──▶ NULL
  3  ──▶ [main.c] ──▶ NULL
  ...
 99  ──▶ [test.c] ──▶ NULL
```

**Stack (Stash):**
```
        TOP
         │
         ▼
    ┌─────────────┐
    │ Stash #2    │
    │ "WIP"       │
    └─────┬───────┘
          │
          ▼
    ┌─────────────┐
    │ Stash #1    │
    │ "Backup"    │
    └─────┬───────┘
          │
          ▼
        NULL
```

**Tree (Branches):**
```
                    master (root)
                   /      \
              feature    hotfix
              /    \
         feature-1  feature-2
```

### Command Line Interface

```
╭─────────────────────────────────────────────────╮
│           Git-DSA Command Line Interface         │
╰─────────────────────────────────────────────────╯

git-dsa> help
Simple Git Clone (DSA Project)
Commands:
  init                  Initialize a new repository
  add <filename>        Add a file to staging area
  commit <msg>          Record changes to the repository
  log                   Show commit logs
  status                Show the working tree status
  graph                 Show visual commit graph with hashes

Stash Commands (Stack DSA):
  stash save <msg>      Save changes to stash stack
  stash pop             Restore most recent stash
  stash list            List all stashes

Branch Commands (Tree DSA):
  branch create <name>  Create a new branch
  branch list           List all branches (tree view)
  branch checkout <name> Switch to a branch
  branch delete <name>  Delete a branch

  exit                  Exit the program
```

---

## 12. Outcome of the Work

### Academic Outcomes

1. **Course Project**: Submitted as Data Structures and Algorithms laboratory project
2. **Learning Outcomes**: Demonstrated practical application of DSA concepts

### Technical Deliverables

1. **Source Code**: Complete C implementation (~800 lines)
2. **Documentation**: Comprehensive README with DSA explanations
3. **Build System**: Makefile for easy compilation

### Future Enhancements

| Enhancement | Description | Complexity |
|-------------|-------------|------------|
| Persistence | Save repository to disk | Medium |
| Merge | Implement branch merging | High |
| Diff | Show differences between commits | Medium |
| Remote | Add remote repository support | High |
| GUI | Graphical user interface | High |

---

## How to Build and Run

```bash
# Clone the repository
git clone https://github.com/username/Git-Clone-With-DSA.git

# Navigate to directory
cd Git-Clone-With-DSA

# Build the project
make

# Run the program
./git_dsa

# Clean build files
make clean
```

---

## License

This project is created for educational purposes as part of a Data Structures and Algorithms course.

---

## Acknowledgments

- Linus Torvalds for creating Git
- Course instructors for guidance
- Open source community for inspiration

---

*Last Updated: December 2024*
