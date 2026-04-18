# Mini-VD Compiler

A full-pipeline C-subset compiler featuring Lexical Analysis, AST parsing, Semantic Checking, Optimization, and 32-bit x86 Code Generation.

## Dependencies
Because this compiler generates 32-bit x86 assembly, your Linux/WSL system must have the 32-bit GCC multilib packages installed to assemble the final binary.
* **Debian/Ubuntu/WSL:** `sudo apt-get install flex bison gcc gcc-multilib`

## Installation

### Method 1: One-Command Automated Install (Recommended)
You can install the compiler directly from your terminal using this single command:
```bash
curl -sL https://raw.githubusercontent.com/VD400/c_compiler/main/install.sh | bash
