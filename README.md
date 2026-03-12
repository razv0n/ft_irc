# Git Troubleshooting Guide

This guide explains how to resolve common `git pull` errors that you might encounter while working on this project.

## Error: "Your local changes to the following files would be overwritten by merge"

**Why this happens:** 
You have uncommitted changes in your local file (e.g., `src/server.cpp`), and someone else has pushed changes to the same file on the remote branch (`origin/main`). Git aborts the pull to prevent overwriting your hard work.

**How to fix it:**
1. **Stash your changes** (temporarily save them away):
   ```bash
   git stash
   ```
2. **Pull the latest changes** from the remote branch:
   ```bash
   git pull
   ```
3. **Bring back your changes**:
   ```bash
   git stash pop
   ```
   *Note: If your stashed changes conflict with the newly pulled code, Git will ask you to resolve the conflict (see below).*

---

## Error: "Pulling is not possible because you have unmerged files / unresolved conflict"

**Why this happens:**
During a `git pull`, `git merge`, or `git stash pop`, Git noticed that you and another person edited the exact same lines of code. Git doesn't know which version to keep, so it raises a conflict.

**How to fix it:**
1. **Find the conflicted files**:
   ```bash
   git status
   ```
   (Files with conflicts will be listed under "Unmerged paths" in red).

2. **Fix the conflict in your editor**:
   Open the file(s) in your text editor (like VS Code). You will see conflict markers that look like this:
   ```cpp
   <<<<<<< HEAD
   // Your local changes or the current branch's code
   =======
   // The incoming changes from the remote branch
   >>>>>>> origin/main
   ```
   Decide which code you want to keep, or manually combine them. Then, **delete the `<<<<<<<`, `=======`, and `>>>>>>>` markers**. Save the file.

3. **Mark the conflict as resolved**:
   ```bash
   git add <filename>
   ```
   *(e.g., `git add src/server.cpp`)*

4. **Complete the merge**:
   ```bash
   git commit -m "Merge branch 'main' and resolve conflicts"
   ```

Now your conflicts are resolved, your changes are safely integrated, and you can continue working!
