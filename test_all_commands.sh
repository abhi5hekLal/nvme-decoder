#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
decoder="${script_dir}/build/nvme-decoder"

if [[ ! -x "${decoder}" ]]; then
    echo "Decoder executable not found: ${decoder}" >&2
    echo "Build it first with: cmake --build build" >&2
    exit 1
fi

zeros=(
    00000000 00000000 00000000 00000000
    00000000 00000000 00000000 00000000
    00000000 00000000 00000000 00000000
    00000000 00000000 00000000
)

admin_opcodes=(
    00 01 02 04 05 06 08 09 0A 0C 0D 10 11 14
    15 18 19 1A 1C 1D 1E 20 24 7C 80 81 82 84
)

nvm_opcodes=(
    00 01 02 04 05 08 09 0C 0D 0E 11 12 15 18 19 1D
)

for opcode in "${admin_opcodes[@]}"; do
    printf '\n========== Admin opcode 0x%s ==========\n' "${opcode}"
    "${decoder}" --set admin "000000${opcode}" "${zeros[@]}"
done

for opcode in "${nvm_opcodes[@]}"; do
    printf '\n========== NVM opcode 0x%s ==========\n' "${opcode}"
    "${decoder}" --set nvm "000000${opcode}" "${zeros[@]}"
done
