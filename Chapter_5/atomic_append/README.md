# Proof of Atomicity: Race Condition Demonstration (O_APPEND vs lseek)

A systems programming experiment that empirically demonstrates the necessity of the O_APPEND flag for concurrent file writes. This project simulates a race condition between two processes trying to append data to the same file simultaneously.

This experiment validates concepts from Chapter 5 of The Linux Programming Interface (TLPI) regarding the System-Wide Open File Table versus per-process file descriptors.

## The Experiment

The program atomic_append writes a specified number of bytes to a file. It can operate in two modes:

<b>Atomic Mode (Correct)</b>: Uses the O_APPEND flag. The kernel guarantees that the "seek to end" and "write data" steps happen as a single, uninterruptible operation.

<b>Non-Atomic Mode (Buggy)</b>: Manually calls lseek(fd, 0, SEEK_END) followed by write(). This leaves a window for a context switch between the two calls.

## The Test Case

We launch two instances of the program simultaneously, each attempting to write 1,000,000 bytes to the same file.

<b>Expected Size</b>: 2,000,000 bytes (Perfect serialization).

<b> Actual Result (Non-Atomic)<b>: ~1,000,624 bytes (Massive data loss due to overwrites).

## How to Reproduce

### 1. Compile

`gcc -Wall -g atomic_append.c -o atomic_append`


### 2. Run the "Buggy" Case (Race Condition)

Pass the x argument to disable O_APPEND and force manual lseek. Run two instances in parallel using &.

`./atomic_append race.txt 1000000 x & ./atomic_append race.txt 1000000 x`


#### Result: Check the file size.

`ls -l race.txt`
<b>Output</b>: ~1000624 bytes (instead of 2000000). 
almost 50% of data is lost due to kernel context switches overwriting data.


### 3. Run the "Atomic" Case (Correct)

Run without the x argument to use O_APPEND.

`./atomic_append safe.txt 1000000 & ./atomic_append safe.txt 1000000`


#### Result:

`ls -l safe.txt`
<b>Output</b>: 2000000 bytes. 
0 bytes lost. Perfect atomicity.


## System Internals Explained

Why the Race Condition Happens

In the Non-Atomic mode, the following sequence occurs:

Process A calls lseek -> Returns offset 1000.

Context Switch happens.

Process B calls lseek -> Returns offset 1000 (Because A hasn't written yet!).

Process B writes 1 byte at 1000.

Context Switch happens.

Process A writes 1 byte at 1000. (OVERWRITE)

### Why O_APPEND Fixes It

When O_APPEND is set, the logic for "move to end" is moved from User Space to Kernel Space. The file offset is updated in the System-Wide Open File Table (shared by all processes referencing the inode) at the exact moment of the write, ensuring no other process can interject.

## Build & Requirements

OS: Linux (Native or WSL2). Note: WSL2 on NTFS mounts may exhibit undefined behavior; use inside Linux root (~/) for consistent results.

Compiler: GCC.