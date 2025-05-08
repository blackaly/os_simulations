# OS Operations Simulation

This project is a simple attempt to simulate basic operating system operations.

It mainly focuses on:
- Measuring the cost of a simple system call.
- Measuring the cost of a context switch between two processes using UNIX pipes.

The idea is inspired by `lmbench`, but this is a minimal and educational version — not a full benchmark suite.

## Purpose

The goal is to understand how operating system mechanisms like system calls and context switches work at a low level, and how much overhead they introduce.

## How to Use

1. Compile the program:
   ```bash 
   gcc benchmark.c -o main
```
