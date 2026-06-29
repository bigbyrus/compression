# Huffman Compression

A from-scratch implementation of **Huffman coding** in C++ — a lossless file compression algorithm that assigns shorter bit sequences to more frequently occurring bytes.

## How it works

Huffman coding is a greedy algorithm that builds an optimal prefix-free binary tree from byte frequencies:

1. **Frequency scan** — read the input file and count how many times each of the 256 possible byte values appears.
2. **Build the tree** — insert all non-zero-frequency bytes into a min-heap (priority queue). Repeatedly pop the two lowest-frequency nodes, combine them under a new parent node whose frequency is their sum, and push the parent back. Repeat until one node remains — that is the root.
3. **Encode** — traverse from each leaf up to the root, recording a `0` for every left-child edge and a `1` for every right-child edge. Reverse the path (root → leaf) to get the prefix code for that byte. Frequent bytes end up with short codes; rare bytes get longer ones.
4. **Decode** — rebuild the identical tree from the stored frequency table, then walk the tree bit-by-bit: `0` → go left, `1` → go right. When you reach a leaf, emit its symbol and restart from the root.

### Compressed file format

```
[ Header: 256 × 4-byte ints ]  ← frequency of byte 0, byte 1, …, byte 255
[ Body: Huffman-encoded bits ]  ← packed MSB-first into bytes
```

The header is always 1 024 bytes (256 × `sizeof(int)`). The body is the entire input re-encoded as a stream of variable-length bit codes.

## Project structure

```
compression/
├── include/
│   ├── Helper.hpp      # FancyInputStream, FancyOutputStream, HCNode, HCNodePtrComp
│   └── HCTree.hpp      # HCTree class declaration
├── src/
│   ├── Helper.cpp      # Bit-level I/O stream implementations + HCNode constructors
│   ├── HCTree.cpp      # build(), encode(), decode() implementations
│   ├── compress.cpp    # compress entry point
│   └── uncompress.cpp  # decompress entry point
├── tests/
│   └── sample.txt      # small test input
├── compress0.cpp       # earlier draft of compress (reads file in 4 KB chunks manually)
└── Makefile
```

### Key classes

| Class | Purpose |
|---|---|
| `FancyInputStream` | Wraps `ifstream`; adds `read_bit()` / `read_byte()` / `read_int()` with an internal 8-bit buffer |
| `FancyOutputStream` | Wraps `ofstream`; adds `write_bit()` / `write_byte()` / `write_int()` with an internal 8-bit buffer |
| `HCNode` | One node in the Huffman tree. `c0` = left (bit 0) child, `c1` = right (bit 1) child, `p` = parent |
| `HCNodePtrComp` | Comparator for the min-heap priority queue — sorts by count ascending, breaking ties by symbol value |
| `HCTree` | Owns the tree. `build()` constructs it from a frequency vector; `encode()` writes the bit-path for a symbol; `decode()` reads bits and returns the matching symbol |

## Building

> **Note:** The Makefile assumes all source files are in the project root. Because the code has since been reorganized into `src/` and `include/` subdirectories, you will need to pass the correct include and source paths:

```bash
g++ -Wall -pedantic -std=c++17 -Iinclude \
    src/compress.cpp src/Helper.cpp src/HCTree.cpp \
    -o compress

g++ -Wall -pedantic -std=c++17 -Iinclude \
    src/uncompress.cpp src/Helper.cpp src/HCTree.cpp \
    -o uncompress
```

Or update the Makefile's paths accordingly, then run:

```bash
make        # build both binaries
make clean  # remove binaries and object files
```

## Usage

```bash
# Compress a file
./compress <input_file> <output_file>

# Decompress a file
./uncompress <compressed_file> <output_file>
```

### Example round-trip

```bash
./compress   tests/sample.txt tests/out.bin
./uncompress tests/out.bin    tests/final.txt
diff tests/sample.txt tests/final.txt   # should produce no output
```

## Limitations

- **Small files may grow** — the 1 024-byte header dominates for tiny inputs. Huffman coding is most effective on files with skewed byte-frequency distributions (e.g. plain text, repetitive binary data).
- **Single unique byte** — if the input contains only one distinct byte value, the tree has a single leaf and no path exists to encode it. This edge case is not explicitly handled.
- **No streaming** — the compressor makes two passes over the input (one to count frequencies, one to encode), so the entire input must be a seekable file.
