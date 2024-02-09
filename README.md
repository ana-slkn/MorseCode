# Morse Code Tool

## Introduction
The Morse Code Tool is a versatile encoding and decoding application designed to support input via standard input or files. It features a binary tree for efficient Morse code decoding, file handling capabilities for external data processing, and provides detailed statistics on translation operations.

## Features
- **Encoding and Decoding**: Convert English text to Morse code and vice versa.
- **File Handling**: Process data from files for both encoding and decoding.
- **Translation History**: Maintain a history of translations with statistics.
- **Efficient Decoding**: Utilize a binary tree data structure for quick Morse code decoding.

## Getting Started
### Installation
1. Clone the repository to your local machine.
2. Compile the source code using a C compiler, e.g., `gcc morse.c -o morse`.

### Usage
- **Encoding**: Run the compiled application with the appropriate switch: `./morse -c <filename>` for encoding text to Morse code.
- **Decoding**: Run the compiled application with the appropriate switch: `./morse -d <filename>` for decoding Morse code to English text.
- **Viewing History**: Use `./morse -h e2m` or `./morse -h m2e` to view translation history.

## Prerequisites
- A C compiler (GCC or Clang)
- Basic knowledge of command-line operations
