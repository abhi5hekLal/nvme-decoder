# NVMe Decoder

A portable C++ command-line tool for describing NVMe 64-byte command submission queue entries. The decoder library is separate from the CLI so a future UI can call the same parsing and decoding API.

## Build

Requires CMake 3.20 or newer and a C++20 compiler (GCC, Clang, or MSVC).

```sh
cmake -S . -B build
cmake --build build
```

Run `./build/nvme-decoder` (Windows: `build\Debug\nvme-decoder.exe` with a typical Visual Studio generator) and enter 16 hexadecimal dwords in order, CDW0 through CDW15. A `0x` prefix is optional; separate values with spaces, commas, or line breaks.

```text
./build/nvme-decoder 00000005 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00070001 00030003 00000000 00000000 00000000 00000000
```

The decoder requires a command-set context because opcodes are reused between command sets. Select `admin` or `nvm` when prompted, or pass `--set admin` / `--set nvm` before the dwords. Output is headed by `Command:` and `Use:`, followed by applicable decoded fields with their CDW/bit range, decoded value, and meaning. Non-applicable common pointer fields are omitted, normal fused-operation encoding is omitted, and consecutive fully reserved dwords are grouped. Each currently catalogued Admin and NVM command has its own source file under `src/commands/admin/` or `src/commands/nvm/`; Set Features and Get Features have separate command entry points and share feature-value decoding. The project is not yet a decoder for every NVMe command set or every field in the full NVMe 2.4 specification family. In particular, ZNS, Key Value, Subsystem Local Memory, Computational Programs, Fabrics, and transport command contexts are not selectable, and controller-dependent fields or data-buffer contents cannot be resolved from a submission entry alone. See [the specification baseline and coverage notes](docs/specification-baseline.md).

Command decoding is kept in separate source files by Admin and NVM opcode, with common formatting and feature helpers factored separately. `src/decoder.cpp` only dispatches to the command-set modules. The API is in `include/nvme_decoder/` for future UI integration.

## Input convention

Each value is a 32-bit dword written as hexadecimal. The first token is CDW0; the opcode is its least significant byte. This tool expects the command dwords as displayed by the source that captured them. If input comes from a raw byte dump, the caller should account for the host/controller byte order before supplying dwords. The current command-set choices are Admin and NVM; ZNS, Key Value, Fabrics, and other command sets need separate modules and explicit selection.
