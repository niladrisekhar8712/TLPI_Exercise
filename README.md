# The Linux Programming Interface (TLPI) - Exercise Solutions

A comprehensive collection of systems programming implementations based on *The Linux Programming Interface* by Michael Kerrisk.

This repository documents my journey from an Electronics (ETCE) background to Systems Engineering, bridging the gap between hardware interrupts and kernel system calls.

##  Project Philosophy

Unlike standard homework solutions, these implementations focus on:

* **Production-Grade Error Handling:** Every system call (`open`, `write`, `fork`) is wrapped with rigorous error checking using `errno`.
* **Memory Safety:** All dynamic allocations are verified with **Valgrind** to ensure zero leaks.
* **Kernel Mechanics:** Experiments are designed to prove OS concepts (e.g., demonstrating race conditions in file appending).

##  Repository Structure

The solutions are organized by the "Systems Architect" roadmap:

| Chapter | Topic | Key Implementations | 
| :--- | :--- | :--- | 
| **04** | **File I/O: The Universal Model** | `copy.c` (A bare-metal clone of `cp` using `read`/`write`) | 
| **05** | **File I/O: Atomicity** | `atomic_append.c` (Proof of race conditions without `O_APPEND`) | 
| **06** | **Process Memory Layout** | `mem_segments.c` (Visualizing Stack, Heap, BSS, and Data segments) | 
| **07** | **Memory Allocation** | `free_and_sbrk.c` (Manipulating the Program Break manually) | 
| **24** | **Process Creation** | `fork` mechanics | 
| **26** | **Child Process Monitoring** | `wait` mechanics | 

##  Build & Usage

This project uses manual compilation via the GCC toolchain.

### Prerequisites

* Linux Environment (Native or WSL2)
* GCC Compiler (`build-essential`)
* Valgrind (for memory debugging)

### How to Compile & Run

Navigate to any chapter directory and use the following commands:

```bash
# 1. Compile a specific program (e.g., copy.c)
# -Wall -Wextra: Enable warnings
# -O2: Optimize for production-grade performance
gcc -Wall -Wextra -O2 filename.c -o output_name

# 2. Run the program
./output_name [arguments]

# 3. Check for Memory Leaks (using Valgrind)
valgrind --leak-check=full ./output_name [arguments]

# 4. Clean up binaries

rm output_name
