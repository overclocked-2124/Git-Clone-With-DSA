# Git-Clone-With-DSA - Mermaid.js Diagrams

This file contains all the Mermaid.js diagram code for the project documentation.

---

## Table of Contents
1. [Architecture Diagram](#1-architecture-diagram)
2. [Flow Chart](#2-flow-chart)
3. [Data Structure Diagrams](#3-data-structure-diagrams)
   - [Linked List (Commits)](#31-linked-list-commits)
   - [Hash Table (Staging Area)](#32-hash-table-staging-area)
   - [Stack (Stash)](#33-stack-stash)
   - [Tree (Branches)](#34-tree-branches)
4. [Git Graph](#4-git-graph)
5. [Class Diagram](#5-class-diagram)
6. [Sequence Diagrams](#6-sequence-diagrams)

---

## 1. Architecture Diagram

```mermaid
flowchart TB
    subgraph Repository["📦 REPOSITORY"]
        direction TB
        HEAD["HEAD Pointer"]
        COUNT["Commit Count"]
        CURRENT["Current Branch"]
    end
    
    subgraph Commits["📝 COMMITS<br/>(Linked List)"]
        direction LR
        C3["Commit #3<br/>hash: f3b570c"] --> C2["Commit #2<br/>hash: 70ff3f9"]
        C2 --> C1["Commit #1<br/>hash: 60add72"]
        C1 --> NULL1["NULL"]
    end
    
    subgraph Staging["📂 STAGING AREA<br/>(Hash Table)"]
        direction TB
        B0["Bucket 0"] --> F1["file1.c"]
        B1["Bucket 1"] --> F2["main.c"] --> F3["config.h"]
        B2["Bucket 2"] --> NULL2["NULL"]
        B99["Bucket 99"] --> F4["test.c"]
    end
    
    subgraph Stash["💾 STASH<br/>(Stack - LIFO)"]
        direction TB
        TOP["TOP"] --> S2["Stash #2<br/>WIP"]
        S2 --> S1["Stash #1<br/>Backup"]
        S1 --> NULL3["NULL"]
    end
    
    subgraph Branches["🌳 BRANCHES<br/>(Tree)"]
        direction TB
        MASTER["master<br/>(root)"]
        MASTER --> FEATURE["feature"]
        MASTER --> HOTFIX["hotfix"]
        FEATURE --> F1B["feature-1"]
        FEATURE --> F2B["feature-2"]
    end
    
    Repository --> Commits
    Repository --> Staging
    Repository --> Stash
    Repository --> Branches
```

---

## 2. Flow Chart

### 2.1 Main Command Processing Flow

```mermaid
flowchart TD
    START([🚀 START]) --> PARSE[Parse Command]
    
    PARSE --> |init| INIT
    PARSE --> |add| ADD
    PARSE --> |commit| COMMIT
    PARSE --> |log| LOG
    PARSE --> |status| STATUS
    PARSE --> |stash| STASH
    PARSE --> |branch| BRANCH
    PARSE --> |graph| GRAPH
    PARSE --> |exit| EXIT
    
    subgraph INIT[" INIT "]
        I1[Allocate Repository] --> I2[Initialize Hash Table]
        I2 --> I3[Initialize Stash Stack]
        I3 --> I4[Create master branch]
    end
    
    subgraph ADD[" ADD "]
        A1[Hash filename] --> A2{File exists?}
        A2 --> |Yes| A3[Update hash]
        A2 --> |No| A4[Create FileEntry]
        A4 --> A5[Insert at bucket head]
    end
    
    subgraph COMMIT[" COMMIT "]
        CM1{Staging empty?} --> |Yes| CM2[Error: Nothing to commit]
        CM1 --> |No| CM3[Create CommitNode]
        CM3 --> CM4[Generate hash]
        CM4 --> CM5[Link to HEAD]
        CM5 --> CM6[Update branch HEAD]
        CM6 --> CM7[Clear staging]
    end
    
    subgraph LOG[" LOG "]
        L1[Start at HEAD] --> L2{Current != NULL?}
        L2 --> |Yes| L3[Print commit info]
        L3 --> L4[Move to next]
        L4 --> L2
        L2 --> |No| L5[Print Initial State]
    end
    
    subgraph STATUS[" STATUS "]
        ST1[Iterate Hash Table] --> ST2{Bucket empty?}
        ST2 --> |No| ST3[Print files in bucket]
        ST3 --> ST4[Next bucket]
        ST4 --> ST2
        ST2 --> |Yes| ST4
    end
    
    subgraph STASH[" STASH "]
        STH1{Subcommand?}
        STH1 --> |save| STH2[Push to stack]
        STH1 --> |pop| STH3[Pop from stack]
        STH1 --> |list| STH4[Traverse stack]
    end
    
    subgraph BRANCH[" BRANCH "]
        BR1{Subcommand?}
        BR1 --> |create| BR2[Add child node]
        BR1 --> |checkout| BR3[Search & switch]
        BR1 --> |list| BR4[DFS traversal]
        BR1 --> |delete| BR5[Remove node]
    end
    
    subgraph GRAPH[" GRAPH "]
        G1[Collect all commits] --> G2[Sort by ID]
        G2 --> G3[Display visual tree]
    end
    
    INIT --> DONE
    ADD --> DONE
    COMMIT --> DONE
    LOG --> DONE
    STATUS --> DONE
    STASH --> DONE
    BRANCH --> DONE
    GRAPH --> DONE
    
    DONE([ Done]) --> PARSE
    EXIT([ EXIT])
```

### 2.2 Simplified Flow

```mermaid
flowchart LR
    A[User Input] --> B{Command Parser}
    B --> C[init]
    B --> D[add]
    B --> E[commit]
    B --> F[stash]
    B --> G[branch]
    B --> H[graph]
    
    C --> I[Repository Created]
    D --> J[Hash Table Updated]
    E --> K[Linked List Extended]
    F --> L[Stack Modified]
    G --> M[Tree Modified]
    H --> N[Graph Displayed]
```

---

## 3. Data Structure Diagrams

### 3.1 Linked List (Commits)

```mermaid
flowchart LR
    subgraph LinkedList["Singly Linked List - Commit History"]
        HEAD["HEAD<br/>📍"]
        
        subgraph C3["CommitNode"]
            C3ID["id: 3"]
            C3H["hash: f3b570c"]
            C3M["msg: Update main"]
            C3T["time: 2024-12-28"]
            C3N["next →"]
        end
        
        subgraph C2["CommitNode"]
            C2ID["id: 2"]
            C2H["hash: 70ff3f9"]
            C2M["msg: Add feature"]
            C2T["time: 2024-12-27"]
            C2N["next →"]
        end
        
        subgraph C1["CommitNode"]
            C1ID["id: 1"]
            C1H["hash: 60add72"]
            C1M["msg: Initial commit"]
            C1T["time: 2024-12-26"]
            C1N["next →"]
        end
        
        NULL["NULL<br/>⊗"]
        
        HEAD --> C3
        C3 --> C2
        C2 --> C1
        C1 --> NULL
    end
    
    style HEAD fill:#ff6b6b,color:#fff
    style NULL fill:#4a4a4a,color:#fff
    style C3 fill:#4ecdc4
    style C2 fill:#45b7d1
    style C1 fill:#96ceb4
```

### 3.2 Hash Table (Staging Area)

```mermaid
flowchart TB
    subgraph HashTable["Hash Table with Chaining - Staging Area"]
        direction TB
        
        subgraph Buckets["Array of Buckets (size: 100)"]
            B0["[0]"]
            B1["[1]"]
            B2["[2]"]
            B3["[3]"]
            DOTS["..."]
            B99["[99]"]
        end
        
        subgraph Chain1["Linked List Chain"]
            F1["file1.c<br/>hash_file1_234"]
            F2["config.h<br/>hash_config_567"]
        end
        
        subgraph Chain2["Linked List Chain"]
            F3["main.c<br/>hash_main_890"]
        end
        
        subgraph Chain3["Linked List Chain"]
            F4["test.c<br/>hash_test_123"]
        end
        
        B0 --> NULL1["NULL"]
        B1 --> F1 --> F2 --> NULL2["NULL"]
        B2 --> NULL3["NULL"]
        B3 --> F3 --> NULL4["NULL"]
        B99 --> F4 --> NULL5["NULL"]
    end
    
    subgraph HashFunction["Hash Function (djb2)"]
        INPUT["filename"] --> HASH["hash = 5381<br/>hash = hash*33 + c"]
        HASH --> INDEX["index = hash % 100"]
    end
    
    style B1 fill:#4ecdc4
    style B3 fill:#45b7d1
    style B99 fill:#96ceb4
```

### 3.3 Stack (Stash)

```mermaid
flowchart TB
    subgraph Stack["Stack (LIFO) - Stash"]
        direction TB
        
        TOP["TOP 📍"]
        
        subgraph S3["StashEntry #3"]
            S3M["msg: Emergency backup"]
            S3T["time: 2024-12-28 15:30"]
            S3F["files: [staging snapshot]"]
            S3N["next ↓"]
        end
        
        subgraph S2["StashEntry #2"]
            S2M["msg: Work in progress"]
            S2T["time: 2024-12-28 12:00"]
            S2F["files: [staging snapshot]"]
            S2N["next ↓"]
        end
        
        subgraph S1["StashEntry #1"]
            S1M["msg: Backup before merge"]
            S1T["time: 2024-12-27 18:00"]
            S1F["files: [staging snapshot]"]
            S1N["next ↓"]
        end
        
        NULL["NULL ⊗"]
        
        TOP --> S3
        S3 --> S2
        S2 --> S1
        S1 --> NULL
    end
    
    subgraph Operations["Stack Operations O(1)"]
        PUSH["PUSH<br/>stash save"] 
        POP["POP<br/>stash pop"]
        PEEK["PEEK<br/>stash list"]
    end
    
    PUSH -.-> TOP
    POP -.-> TOP
    
    style TOP fill:#ff6b6b,color:#fff
    style S3 fill:#ffd93d
    style S2 fill:#6bcb77
    style S1 fill:#4d96ff
```

### 3.4 Tree (Branches)

```mermaid
flowchart TB
    subgraph BranchTree["N-ary Tree - Branch Hierarchy"]
        direction TB
        
        subgraph ROOT["BranchNode: master"]
            R_NAME["name: master"]
            R_HEAD["commit_head → Commit #5"]
            R_COUNT["child_count: 2"]
            R_PARENT["parent: NULL"]
        end
        
        subgraph FEATURE["BranchNode: feature"]
            F_NAME["name: feature"]
            F_HEAD["commit_head → Commit #3"]
            F_COUNT["child_count: 2"]
            F_PARENT["parent: master"]
        end
        
        subgraph HOTFIX["BranchNode: hotfix"]
            H_NAME["name: hotfix"]
            H_HEAD["commit_head → Commit #6"]
            H_COUNT["child_count: 0"]
            H_PARENT["parent: master"]
        end
        
        subgraph FEAT1["BranchNode: feature-auth"]
            FA_NAME["name: feature-auth"]
            FA_HEAD["commit_head → Commit #4"]
            FA_COUNT["child_count: 0"]
            FA_PARENT["parent: feature"]
        end
        
        subgraph FEAT2["BranchNode: feature-ui"]
            FU_NAME["name: feature-ui"]
            FU_HEAD["commit_head → Commit #7"]
            FU_COUNT["child_count: 0"]
            FU_PARENT["parent: feature"]
        end
        
        ROOT --> FEATURE
        ROOT --> HOTFIX
        FEATURE --> FEAT1
        FEATURE --> FEAT2
    end
    
    style ROOT fill:#ff6b6b,color:#fff
    style FEATURE fill:#4ecdc4
    style HOTFIX fill:#ffd93d
    style FEAT1 fill:#96ceb4
    style FEAT2 fill:#45b7d1
```

---

## 4. Git Graph

### 4.1 Simple Git Graph

```mermaid
gitGraph
    commit id: "60add72" tag: "Initial"
    commit id: "8d3a988"
    branch feature
    checkout feature
    commit id: "3154233"
    commit id: "393be48" tag: "feature"
    checkout main
    commit id: "745eac2"
    branch hotfix
    checkout hotfix
    commit id: "f9df3fa" tag: "hotfix"
    checkout main
    merge hotfix
    checkout feature
    commit id: "a1b2c3d"
    checkout main
    merge feature
```

### 4.2 Complex Git Graph with Multiple Branches

```mermaid
gitGraph
    commit id: "init" tag: "v0.1"
    commit id: "add-makefile"
    commit id: "add-readme"
    branch develop
    checkout develop
    commit id: "setup-structure"
    commit id: "add-headers"
    branch feature/linked-list
    checkout feature/linked-list
    commit id: "implement-commit-node"
    commit id: "implement-log"
    checkout develop
    branch feature/hash-table
    checkout feature/hash-table
    commit id: "implement-staging"
    commit id: "implement-add"
    checkout develop
    merge feature/linked-list id: "merge-ll" tag: "linked-list-done"
    merge feature/hash-table id: "merge-ht"
    branch feature/stack
    checkout feature/stack
    commit id: "implement-stash"
    commit id: "add-stash-cmds"
    checkout develop
    branch feature/tree
    checkout feature/tree
    commit id: "implement-branch"
    commit id: "add-branch-cmds"
    checkout develop
    merge feature/stack
    merge feature/tree id: "all-features" tag: "v1.0"
    checkout main
    merge develop tag: "release"
```

### 4.3 Git Graph showing Our Project's Development

```mermaid
gitGraph
    commit id: "60ae630" type: NORMAL tag: "init"
    commit id: "8d3a988" type: NORMAL
    branch feature
    checkout feature
    commit id: "3154233" type: HIGHLIGHT
    commit id: "393be48" type: HIGHLIGHT tag: "feature-complete"
    checkout main
    commit id: "745eac2" type: NORMAL
    branch hotfix
    checkout hotfix
    commit id: "f9df3fa" type: REVERSE tag: "emergency"
    checkout main
    merge hotfix type: NORMAL
```

---

## 5. Class Diagram

```mermaid
classDiagram
    class Repository {
        +CommitNode* head
        +FileEntry* staging_area[100]
        +int commit_count
        +StashEntry* stash_top
        +int stash_count
        +BranchNode* branch_tree
        +BranchNode* current_branch
    }
    
    class CommitNode {
        +int id
        +char hash[41]
        +char message[512]
        +char timestamp[50]
        +char branch_name[50]
        +CommitNode* next
    }
    
    class FileEntry {
        +char filename[256]
        +char content_hash[50]
        +FileEntry* next
    }
    
    class StashEntry {
        +FileEntry* files[100]
        +char message[512]
        +char timestamp[50]
        +StashEntry* next
    }
    
    class BranchNode {
        +char name[50]
        +CommitNode* commit_head
        +int commit_count
        +BranchNode* children[10]
        +int child_count
        +BranchNode* parent
    }
    
    Repository "1" --> "*" CommitNode : head
    Repository "1" --> "*" FileEntry : staging_area
    Repository "1" --> "*" StashEntry : stash_top
    Repository "1" --> "1" BranchNode : branch_tree
    Repository "1" --> "1" BranchNode : current_branch
    
    CommitNode --> CommitNode : next
    FileEntry --> FileEntry : next
    StashEntry --> StashEntry : next
    StashEntry --> FileEntry : files
    BranchNode --> BranchNode : children
    BranchNode --> BranchNode : parent
    BranchNode --> CommitNode : commit_head
```

---

## 6. Sequence Diagrams

### 6.1 Commit Workflow

```mermaid
sequenceDiagram
    participant User
    participant CLI as CLI (main.c)
    participant Git as git_dsa.c
    participant Staging as Hash Table
    participant Commits as Linked List
    participant Branch as Branch Tree
    
    User->>CLI: add file.c
    CLI->>Git: add_file("file.c")
    Git->>Git: hash_function("file.c")
    Git->>Staging: Insert FileEntry at bucket
    Staging-->>Git: Success
    Git-->>CLI: "Added 'file.c' to staging"
    CLI-->>User: Display message
    
    User->>CLI: commit "Add feature"
    CLI->>Git: commit_changes("Add feature")
    Git->>Staging: Check if empty
    Staging-->>Git: Not empty
    Git->>Git: Create CommitNode
    Git->>Git: generate_commit_hash()
    Git->>Commits: Insert at HEAD
    Git->>Branch: Update commit_head
    Git->>Staging: Clear staging area
    Git-->>CLI: "[master abc1234] Add feature"
    CLI-->>User: Display message
```

### 6.2 Branch Workflow

```mermaid
sequenceDiagram
    participant User
    participant CLI as CLI
    participant Git as git_dsa.c
    participant Tree as Branch Tree
    participant Commits as Linked List
    
    User->>CLI: branch create feature
    CLI->>Git: branch_create("feature")
    Git->>Git: Create BranchNode
    Git->>Tree: Add as child of current
    Git->>Git: Copy commit_head from parent
    Git-->>CLI: "Created branch 'feature'"
    CLI-->>User: Display message
    
    User->>CLI: branch checkout feature
    CLI->>Git: branch_checkout("feature")
    Git->>Tree: find_branch("feature")
    Tree-->>Git: Return BranchNode
    Git->>Git: Update current_branch
    Git->>Commits: Update HEAD pointer
    Git-->>CLI: "Switched to 'feature'"
    CLI-->>User: Display message
```

### 6.3 Stash Workflow

```mermaid
sequenceDiagram
    participant User
    participant CLI as CLI
    participant Git as git_dsa.c
    participant Stack as Stash Stack
    participant Staging as Hash Table
    
    User->>CLI: stash save "WIP"
    CLI->>Git: stash_save("WIP")
    Git->>Staging: Check if empty
    Staging-->>Git: Not empty
    Git->>Git: Create StashEntry
    Git->>Staging: Deep copy all files
    Git->>Stack: Push StashEntry (LIFO)
    Git->>Staging: Clear staging area
    Git-->>CLI: "Saved: stash@{0}"
    CLI-->>User: Display message
    
    User->>CLI: stash pop
    CLI->>Git: stash_pop()
    Git->>Stack: Pop top entry
    Stack-->>Git: Return StashEntry
    Git->>Staging: Restore files
    Git->>Git: Free StashEntry
    Git-->>CLI: "Restored stash"
    CLI-->>User: Display message
```

---

## 7. State Diagram

```mermaid
stateDiagram-v2
    [*] --> Uninitialized
    
    Uninitialized --> Initialized: init
    
    Initialized --> StagingFiles: add file
    Initialized --> ViewingLog: log
    Initialized --> ViewingStatus: status
    Initialized --> ManagingBranches: branch
    Initialized --> ManagingStash: stash
    Initialized --> ViewingGraph: graph
    
    StagingFiles --> Initialized: done
    StagingFiles --> Committing: commit
    
    Committing --> Initialized: success
    
    ViewingLog --> Initialized: done
    ViewingStatus --> Initialized: done
    ViewingGraph --> Initialized: done
    
    ManagingBranches --> CreatingBranch: create
    ManagingBranches --> CheckingOut: checkout
    ManagingBranches --> ListingBranches: list
    ManagingBranches --> DeletingBranch: delete
    
    CreatingBranch --> Initialized: done
    CheckingOut --> Initialized: done
    ListingBranches --> Initialized: done
    DeletingBranch --> Initialized: done
    
    ManagingStash --> SavingStash: save
    ManagingStash --> PoppingStash: pop
    ManagingStash --> ListingStash: list
    
    SavingStash --> Initialized: done
    PoppingStash --> Initialized: done
    ListingStash --> Initialized: done
    
    Initialized --> [*]: exit
```

---

## 8. Data Flow Diagram

```mermaid
flowchart LR
    subgraph Input["Input Layer"]
        USER["👤 User"]
        CMD["Command Line"]
    end
    
    subgraph Processing["Processing Layer"]
        PARSER["Command Parser"]
        HASH["Hash Function"]
        COMMIT_OP["Commit Operations"]
        STASH_OP["Stash Operations"]
        BRANCH_OP["Branch Operations"]
    end
    
    subgraph Storage["Storage Layer (In-Memory)"]
        LL["📝 Linked List<br/>(Commits)"]
        HT["📂 Hash Table<br/>(Staging)"]
        STK["💾 Stack<br/>(Stash)"]
        TREE["🌳 Tree<br/>(Branches)"]
    end
    
    subgraph Output["Output Layer"]
        DISPLAY["Terminal Display"]
        GRAPH["Graph Visualization"]
    end
    
    USER --> CMD
    CMD --> PARSER
    PARSER --> HASH
    PARSER --> COMMIT_OP
    PARSER --> STASH_OP
    PARSER --> BRANCH_OP
    
    HASH --> HT
    COMMIT_OP --> LL
    COMMIT_OP --> HT
    STASH_OP --> STK
    STASH_OP --> HT
    BRANCH_OP --> TREE
    BRANCH_OP --> LL
    
    LL --> DISPLAY
    HT --> DISPLAY
    STK --> DISPLAY
    TREE --> DISPLAY
    LL --> GRAPH
    TREE --> GRAPH
```

---

## 9. Time Complexity Visualization

```mermaid
xychart-beta
    title "Time Complexity Comparison"
    x-axis ["Insert", "Search", "Delete", "Traverse"]
    y-axis "Operations" 0 --> 5
    bar [1, 1, 1, 4] "Hash Table O(1)/O(n)"
    bar [1, 4, 1, 4] "Linked List"
    bar [1, 1, 1, 4] "Stack"
    bar [4, 4, 4, 4] "Tree (worst)"
```

---

## How to Use These Diagrams

### In GitHub README
GitHub natively supports Mermaid diagrams. Simply include the code blocks with ` ```mermaid ` in your README.md file.

### In Documentation Sites
Most modern documentation generators (Docusaurus, MkDocs, GitBook) support Mermaid.

### Export as Images
Use tools like:
- [Mermaid Live Editor](https://mermaid.live/)
- VS Code extensions (Markdown Preview Mermaid Support)
- CLI tool: `mmdc` (mermaid-cli)

```bash
# Install mermaid-cli
npm install -g @mermaid-js/mermaid-cli

# Convert to PNG
mmdc -i diagrams.md -o output.png
```

---

## References

- [Mermaid.js Documentation](https://mermaid.js.org/)
- [Git Graph Syntax](https://mermaid.js.org/syntax/gitgraph.html)
- [Flowchart Syntax](https://mermaid.js.org/syntax/flowchart.html)
- [Class Diagram Syntax](https://mermaid.js.org/syntax/classDiagram.html)
- [Sequence Diagram Syntax](https://mermaid.js.org/syntax/sequenceDiagram.html)

---

*Generated for Git-Clone-With-DSA Project - December 2024*
