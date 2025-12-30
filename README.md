# woody-woodpacker

School project implementing a basic ELF executable packer.

## Description
Implements an ELF packer that encrypts the original `.text` section of a binary and injects a custom payload.
When the resulting executable is run, execution starts from the injected payload, which:
- decrypts the previously encrypted `.text` section,
- prints a marker message (`"...WOODY..."`),
- then transfers control back to the original program entry point.

## Concepts Practiced
- ELF file format and executable layout
- Code injection and binary modification
- Runtime code decryption
- Program entry point manipulation
- Low-level systems programming in C

## Status
Completed academic project.
