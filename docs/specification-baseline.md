# Specification baseline and coverage

## Baseline

The current published specification family is NVMe 2.4 (Base 2.4 plus independent command-set, transport, Boot, and Management Interface specifications). The implementation has not yet been audited against the complete 2.4 family. A 16-dword submission entry is interpreted using the command set and context that submitted it; an opcode by itself is not enough to identify a command.

The current code dispatches only Admin and NVM command sets. It does **not** yet implement every NVMe command set or every field in the full specification family. Each opcode currently catalogued by the Admin and NVM dispatch tables has an individual command source file. The NVM opcode catalog follows the commands listed in NVM Command Set 1.3, including the common I/O management and Cancel commands. Field-level decoding still needs a revision-by-revision audit; fields carried in data buffers are described rather than decoded from their contents, and some fields depend on controller capabilities or Identify data. Other command sets and transport-specific contexts are not selectable yet. Do not treat this project as a replacement for checking an applicable specification revision until the coverage audit is complete.

## Field decoding principles

- Every displayed field should identify the source dword and bit range (or the completion/data-buffer location when the value is not in the 64-byte submission entry).
- Zero-based counts should be displayed as both the encoded value and the resulting count where the specification defines that relationship (e.g. queue entries = QSIZE + 1).
- A field whose meaning depends on Identify data, controller capabilities, a feature's scope, command-set revision, or a transferred data structure must state that dependency. The 16 dwords alone cannot supply the missing device or data-buffer contents.
- Reserved fields are reported as reserved with the host's zeroing requirement when the specification defines one.
- Data Pointer addresses are separated into 64-bit entries and interpreted according to PSDT. The SGL descriptor's nested fields and PRP list contents still require additional decoding and memory contents.

## Known coverage gaps

- Feature decoding is partial. Several FIDs have only their name or an unstructured value; feature-specific scopes, reserved bits, and Get Features result/data-buffer formats require completion and spec audit.
- Some NVM command field meanings, conditional fields, protection-information combinations, and reserved regions still need a revision-specific audit.
- Identify and Get Log Page decode command inputs, not the returned Identify structures or log-page payloads. Power state numbers, selected feature values, and log contents cannot be fully explained without those payloads.
- Data buffers are annotated but their bytes are not accepted by the current CLI, so commands such as Namespace Management, Dataset Management, Copy, and Reservations cannot be fully interpreted.
- The decoder does not yet support the ZNS, Key Value, Subsystem Local Memory, Computational Programs, Fabrics, NVMe-MI in its native message format, or transport-specific command contexts.

## Current decoder modules

- `src/decoder.cpp`: command-set dispatch only.
- `src/admin_commands.cpp`: Admin opcode catalog and routing.
- `src/commands/admin/`: one source file for each catalogued Admin command.
- `src/nvm_commands.cpp`: NVM opcode catalog, shared field helpers, and routing.
- `src/commands/nvm/`: one source file for each catalogued NVM command.
- `src/features.cpp`: shared Set/Get Features identifier and value decoding.
- `src/field_helpers.hpp`: common command header and structured-field formatting.

## Official specification references

- [NVMe Base Specification](https://nvmexpress.org/specification/nvm-express-base-specification/)
- [NVM Command Set Specification](https://nvmexpress.org/specification/nvm-command-set-specification/)
- [Zoned Namespaces Command Set Specification](https://nvmexpress.org/specification/zoned-namespaces-command-set-specification/)
- [Key Value Command Set Specification](https://nvmexpress.org/specification/key-value-command-set-specification/)
- [Subsystem Local Memory Command Set Specification](https://nvmexpress.org/specification/subsystem-local-memory-command-set/)
- [Computational Programs Command Set Specification](https://nvmexpress.org/specification/computational-programs-command-set/)
- [NVMe Boot Specification](https://nvmexpress.org/specification/nvme-boot-specification/)
- [NVMe Management Interface Specification](https://nvmexpress.org/specification/nvme-mi-specification/)

Command field definitions can depend on the selected command-set revision and controller capabilities. Where a field depends on a capability or an Identify structure, the 16 command dwords alone cannot establish the device-specific interpretation; the decoder should name that dependency instead of inventing a value.
