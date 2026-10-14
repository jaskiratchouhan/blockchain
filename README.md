# Simple Blockchain

A simplified blockchain implementation written in C that demonstrates cryptographic hashing, proof-of-work mining, and blockchain integrity verification.

This project explores fundamental blockchain concepts, including how blocks are linked through cryptographic hashes, how new blocks are mined, and how modifications to existing blocks can be detected.

## Features

### Block Creation
- Creates blocks containing an index, timestamp, data, previous block hash, and nonce.
- Links consecutive blocks using SHA-256 cryptographic hashes.
- Maintains an ordered sequence of blocks.

### Proof-of-Work Mining
- Implements proof-of-work using nonce iteration.
- Generates SHA-256 hashes until a block satisfies the specified difficulty target.
- Supports blockchain initialization with or without a difficulty requirement.

### Blockchain Verification
- Recalculates block hashes to verify data integrity.
- Validates cryptographic links between consecutive blocks.
- Checks whether each block satisfies the blockchain's difficulty requirement.
- Detects modifications that invalidate the blockchain.

## Technologies Used

- **C** — Core implementation
- **OpenSSL (libcrypto)** — SHA-256 cryptographic hashing
- **CMake** — Build system
- **Linux** — Target development environment

## How It Works

Each block contains:

| Field | Description |
|---|---|
| `index` | Position of the block in the blockchain |
| `timestamp` | Time the block was created |
| `data` | Data stored in the block |
| `p_hash` | SHA-256 hash of the previous block |
| `nonce` | Value adjusted during mining |
| `hash` | SHA-256 hash of the current block |

### 1. Creating a Block

When a new block is added, the program assigns its index, timestamp, data, and previous block's hash.

### 2. Mining

If a difficulty target is specified, the program repeatedly adjusts the block's nonce and calculates its SHA-256 hash until the hash satisfies the target.

### 3. Linking Blocks

Each new block stores the hash of the previous block, creating a cryptographically linked chain.

### 4. Verification

The blockchain can be verified by recalculating block hashes, checking their links, and ensuring that the hashes satisfy the configured difficulty requirement.

## Installation

### Prerequisites

- Linux or WSL
- GCC or Clang
- CMake
- OpenSSL development libraries

### Clone the Repository

```bash
git clone https://github.com/jaskiratchouhan/blockchain.git
cd blockchain
```

### Build

```bash
cmake -S . -B build
cmake --build build
```

### Run

```bash
./build/blockchain
```


## Project Structure

```text

├── src/
├── include/
├── CMakeLists.txt
└── README.md
```

## Project Background

Originally developed as part of CMPT 201 coursework at SFU.

The project focuses on implementing fundamental blockchain mechanisms in C, including cryptographic hashing, proof-of-work mining, and data integrity verification.

This is an educational blockchain implementation rather than a production cryptocurrency network.
