# Final Project Report - Git-Clone-With-DSA

## 1. Project Overview
This project is a simplified, in-memory clone of core Git workflows implemented for a Data Structures and Algorithms (DSA) lab. It demonstrates how fundamental data structures underpin version-control operations such as staging, committing, and logging history. The program offers a REPL-style interface that accepts Git-like commands and maintains all state in memory for clarity and teaching value.

## 2. Functional Scope (Commands)
- `help`: Print available commands and short descriptions.
- `init`: Initialize an in-memory repository (`Repository` struct) with staging and commit history.
- `add <filename>`: Stage a filename. Uses a hash table with chaining to store entries and simulates a content hash.
- `commit <msg>`: Create a new commit node with an auto-incremented ID and timestamp; clears the staging area after committing.
- `log`: Traverse the commit linked list from newest to oldest and print commit metadata.
- `status`: Iterate through the staging hash table and show staged entries (or report a clean tree).
- `exit`: Quit the program loop.

## 3. Architecture & Data Structures
- **Global repository**: `Repository* repo` (singleton-like) holds commit history and the staging area.
- **Commit history**: Singly linked list (`CommitNode`) where `head` always points to the latest commit; insertion is O(1).
- **Staging area**: Fixed-size hash table (`HASH_SIZE = 100`) of buckets, each a chained linked list of `FileEntry` nodes. Hashing uses the classic djb2 variant for string keys.
- **Timestamps**: `get_current_time` formats commit times as `YYYY-MM-DD HH:MM:SS` using `strftime`.
- **Source layout**:
  - `src/main.c`: CLI loop, command dispatch, and help output.
  - `src/git_dsa.c`: Repository logic, DSA usage (hash table, linked lists), command implementations.
  - `src/git_dsa.h`: Data structure definitions, constants, and function prototypes.
  - `Makefile`: Build rules for the CLI executable `git_dsa`.

## 4. Control Flow (Runtime Loop)
1. Print welcome banner and prompt (`git-dsa>`).
2. Read a command token via `scanf`.
3. Dispatch to handlers (`init`, `add`, `commit`, `log`, `status`, `help`, `exit`).
4. Continue until `exit` or EOF.

## 5. Build & Run
Prerequisites: GCC toolchain (tested with `gcc`, `-Wall -g`).

Build:
```sh
make
```
Run:
```sh
./git_dsa
```
Clean objects and binary:
```sh
make clean
```
Note: On Windows with MinGW or similar, use `./git_dsa.exe` after building.

## 6. Example Session
```
Welcome to Git-DSA! Type 'help' for commands.

git-dsa> init
Initialized empty Git repository (in memory).

git-dsa> add file1.txt
Added 'file1.txt' to staging area.

git-dsa> commit Initial commit
[master 1] Initial commit

git-dsa> status
On branch master
Changes to be committed:
  (nothing to commit, working tree clean)

git-dsa> log
Commit ID: 1
Date:      2025-12-18 12:00:00
Message:   Initial commit
|
v
(Initial State)
```
(Timestamps will reflect your system clock.)

## 7. Complexity Highlights
- Hash insert/lookup for staging: Expected O(1) average due to hashing with chaining; worst-case O(n) per bucket if collisions cluster.
- Commit insertion: O(1) (prepend to linked list).
- Log traversal: O(c) where c is number of commits.
- Status traversal: O(f + b) where f is number of staged files and b is bucket count (100), as each bucket is scanned.

## 8. Error Handling & Edge Cases
- Command gating: Most operations guard against missing initialization (`init` must precede `add`, `commit`, `status`, `log`).
- Empty commit prevention: `commit` warns when staging is empty.
- Staging updates: Re-adding a staged filename updates its simulated hash and reports the update.
- Input reading: `commit` reads the rest of the line as the message and strips the trailing newline.

## 9. Limitations
- In-memory only; state is lost on program exit.
- File contents are not read; hashes are simulated (`hash_<filename>_<rand>`).
- No branches, checkouts, merges, diffs, or file removal (`rm`); history is linear and append-only.
- Hash table size is fixed and not resized; high collision rates would degrade performance.
- No persistence layer or serialization of commit objects/staging area.

## 10. Future Enhancements
1. Persist repository to disk (e.g., simple JSON or binary dump) and add `load/save` commands.
2. Track file contents and compute real hashes (SHA-1/256) for staged blobs.
3. Add `rm`, `reset`, and `checkout`-like operations to demonstrate tree state changes.
4. Implement branching with a DAG of commits and references to simulate `git` more closely.
5. Introduce dynamic hash-table resizing and better collision metrics.
6. Add basic diffing or change summaries for staged files.

## 11. Testing Guidance
- Build sanity: `make` should succeed without warnings under `-Wall -g`.
- Manual REPL tests: verify flows `init -> add -> status -> commit -> log` and repeated commits.
- Edge tests: `status` before `init`; `commit` with empty staging; re-`add` the same filename to ensure update message appears; multiple different filenames to observe hash bucket chaining.
- Memory checks (optional, Unix): run under `valgrind ./git_dsa` to catch leaks in staging/commit routines.

## 12. Maintenance Notes
- All allocations in staging are freed when a commit clears the staging table; commit nodes remain until program exit. Extend with cleanup logic if you add a shutdown path.
- `HASH_SIZE` can be tuned; ensure consistency across `git_dsa.c` and `git_dsa.h` if modified.
- When adding new commands, expose prototypes in `git_dsa.h` and update the REPL dispatch in `main.c`.

## 13. Quick Reference
- Build: `make`
- Run: `./git_dsa`
- Clean: `make clean`
- Core files: `src/main.c`, `src/git_dsa.c`, `src/git_dsa.h`, `Makefile`

---
Prepared as the final in-depth report for the Git-Clone-With-DSA project.
