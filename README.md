# Arithmetic Operations Using Pipes ⚙️➕➖✖️➗

A multi-process arithmetic system built in **C** using **named pipes (FIFO)** and executed in a **virtual machine terminal environment**. This project was developed during a university-level systems programming course, and provided hands-on experience with **process control**, **IPC**, and **Unix-based programming**.

---

## 🎯 Project Objective

To implement a **Transaction Manager** that coordinates multiple subprocesses — each responsible for a specific arithmetic operation:
- Addition
- Subtraction
- Multiplication
- Division

All communication is handled using **bidirectional named pipes**, and the subprocesses are launched using `fork()` and `exec()` system calls.

---

## 🛠 Technologies Used

- C Programming Language
- Named Pipes (`mkfifo`)
- `fork()`, `exec()`, `open()`, `read()`, `write()`, `unlink()`
- Virtual Machine (Ubuntu-based)
- Terminal-Only Interface (No GUI)

---

## ⚙️ Program Components

### 🧠 `trmn.c` — Transaction Manager
- Creates named pipes for each arithmetic operation
- Launches subprocesses using `fork()` and `execl()`
- Presents a **menu** for user input:
  - Choose operation
  - Enter two integers
- Sends data to corresponding subprocess
- Receives and displays the result
- Cleans up all pipes on exit

### ➕ `adder.c`
### ➖ `subtractor.c`
### ✖️ `multiplier.c`
### ➗ `divider.c`
- Each reads commands from their pipe
- Performs arithmetic on two numbers
- Writes result back to the pipe
- Listens in a loop until `exit` command is received

---

## 🔄 Communication Architecture

- Uses **named pipes (FIFOs)** to enable **bidirectional communication**
- Each operation has its own pipe, e.g., `/tmp/adder_pipe`
- Main process sends:
  - Operation code
  - Operand 1
  - Operand 2
- Subprocess performs calculation, returns result via same pipe
## 💡 Key Concepts Practiced
- Process Management (fork(), execl())

- Named Pipes / FIFO Communication

- Terminal-based inter-process I/O

- Resource cleanup

- Working in a VM-only development environment
