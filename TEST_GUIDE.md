# Test Script for Git-DSA Features

This document shows test scenarios for the new stash and branch features.

## Test 1: Stash Operations (Stack DSA)

### Scenario: Save work in progress and restore later
```
git-dsa> init
git-dsa> add file1.txt
git-dsa> add file2.txt
git-dsa> status
git-dsa> stash save Work in progress on feature A
git-dsa> status                    # Should show clean
git-dsa> add file3.txt
git-dsa> stash save Another WIP
git-dsa> stash list               # Should show 2 stashes (LIFO order)
git-dsa> stash pop                # Restores file3.txt
git-dsa> stash pop                # Restores file1.txt and file2.txt
git-dsa> status                   # All files back in staging
```

### Expected Output:
- Stash save creates a new entry on top of the stack
- Stash list shows entries in reverse order (newest first)
- Stash pop removes from top (LIFO behavior)

---

## Test 2: Branch Operations (Tree DSA)

### Scenario: Create hierarchical branch structure
```
git-dsa> init
git-dsa> add initial.txt
git-dsa> commit Initial commit
git-dsa> branch create feature-1
git-dsa> branch create feature-2
git-dsa> branch list              # Tree view showing master with 2 children
git-dsa> branch checkout feature-1
git-dsa> add feature1.txt
git-dsa> commit Add feature 1
git-dsa> branch create feature-1-bugfix
git-dsa> branch list              # Shows nested structure
git-dsa> branch checkout master
git-dsa> branch checkout feature-2
git-dsa> add feature2.txt
git-dsa> commit Add feature 2
git-dsa> branch list
```

### Expected Tree Structure:
```
master (1 commits)
  * feature-1 (current, 1 commits)
      feature-1-bugfix (0 commits)
    feature-2 (1 commits)
```

---

## Test 3: Combined Workflow

### Scenario: Real-world workflow with stash and branches
```
git-dsa> init
git-dsa> add main.c
git-dsa> commit Initial project setup
git-dsa> branch create feature-auth
git-dsa> branch checkout feature-auth
git-dsa> add auth.c
git-dsa> add login.c
git-dsa> stash save WIP: authentication system
git-dsa> branch checkout master
git-dsa> add bugfix.c
git-dsa> commit Critical bugfix
git-dsa> branch checkout feature-auth
git-dsa> stash pop
git-dsa> commit Complete authentication system
git-dsa> log
```

---

## Test 4: Edge Cases

### Stash Edge Cases:
```
# Empty staging area
git-dsa> stash save Nothing here
# Expected: "No local changes to save"

# Pop empty stash
git-dsa> stash pop
# Expected: "No stash entries found"
```

### Branch Edge Cases:
```
# Delete current branch
git-dsa> branch delete master
# Expected: "Cannot delete current branch"

# Delete branch with children
git-dsa> branch delete feature-1  # (when it has feature-1-bugfix)
# Expected: "Cannot delete branch with children"

# Checkout non-existent branch
git-dsa> branch checkout nonexistent
# Expected: "Branch 'nonexistent' not found"
```

---

## DSA Concepts Demonstrated

### Stack (Stash):
- **Push**: `stash save` adds to top - O(1) insertion
- **Pop**: `stash pop` removes from top - O(1) removal
- **LIFO**: Last stashed is first restored
- **Traversal**: `stash list` shows all entries

### Tree (Branches):
- **Insertion**: `branch create` adds child node
- **DFS Traversal**: `branch list` recursively prints tree
- **Search**: `branch checkout` finds node by name
- **Deletion**: `branch delete` removes leaf nodes
- **Parent-Child**: Maintains branch hierarchy

---

## Performance Notes

### Stash Operations:
- Save: O(n) where n = number of staged files
- Pop: O(n) where n = number of files in stash
- List: O(k) where k = number of stashes

### Branch Operations:
- Create: O(1) - adds to parent's children array
- List: O(b) where b = total branches (DFS traversal)
- Checkout: O(b) - searches entire tree
- Delete: O(c) where c = number of children in parent

---

## Compile and Run

### Windows (PowerShell):
```powershell
gcc -Wall -g -c src/main.c -o src/main.o
gcc -Wall -g -c src/git_dsa.c -o src/git_dsa.o
gcc -Wall -g -o git_dsa src/main.o src/git_dsa.o
./git_dsa
```

### Linux/Mac:
```bash
make
./git_dsa
```
