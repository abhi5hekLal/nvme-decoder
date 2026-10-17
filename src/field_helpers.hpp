#pragma once
#include "nvme_decoder/command.hpp"
#include <iomanip>
#include <sstream>
#include <string>
namespace nvme_decoder::detail {
inline std::string hex_value(std::uint64_t value, int width = 0) {
    std::ostringstream out;
    out << "0x" << std::uppercase << std::hex << std::setfill('0');
    if (width)
        out << std::setw(width);
    out << value;
    return out.str();
}
inline void field(DecodedCommand& out, std::string name, std::string location, std::uint64_t raw,
                  std::string decoded, std::string meaning, int width = 0) {
    out.fields.push_back({std::move(name), std::move(location), hex_value(raw, width),
                          std::move(decoded), std::move(meaning)});
}
inline void field_text(DecodedCommand& out, std::string name, std::string location, std::string raw,
                       std::string decoded, std::string meaning) {
    out.fields.push_back({std::move(name), std::move(location), std::move(raw), std::move(decoded),
                          std::move(meaning)});
}
inline std::string reserved_dwords_raw(const Command& c, unsigned first, unsigned last) {
    std::ostringstream out;
    for (unsigned i = first; i <= last; ++i) {
        if (i != first)
            out << ", ";
        out << "CDW" << i << '=' << hex_value(c[i], 8);
    }
    return out.str();
}
inline std::string reserved_dwords_decoded(const Command& c, unsigned first, unsigned last) {
    std::ostringstream out;
    bool found_nonzero = false;
    for (unsigned i = first; i <= last; ++i) {
        if (c[i] == 0)
            continue;
        if (!found_nonzero)
            out << "Non-zero reserved value: ";
        else
            out << ", ";
        out << "CDW" << i << '=' << hex_value(c[i], 8);
        found_nonzero = true;
    }
    if (!found_nonzero)
        return "All zero (valid)";
    out << " (reserved dwords must be zero)";
    return out.str();
}
inline void common_header(DecodedCommand& out, const Command& c, bool nsid_used,
                          bool command_dwords_2_3_used = false, bool data_pointer_used = false,
                          bool metadata_pointer_used = false) {
    const auto cdw0 = c[0];
    field(out, "Opcode", "CDW0[7:0]", cdw0 & 0xffu, hex_value(cdw0 & 0xffu, 2),
          "Command identifier within the selected command set.", 2);
    const auto fused = (cdw0 >> 8) & 3u;
    if (fused != 0)
        field(out, "Fused Operation", "CDW0[9:8]", fused, std::to_string(fused),
              "0 = normal command; 1 = first command of a fused pair; 2 = second command of a "
              "fused pair; 3 = reserved. Fused operation is supported only for applicable commands "
              "and command-set configurations.");
    const auto psdt = (cdw0 >> 14) & 3u;
    if (data_pointer_used)
        field(out, "Pointer Type (PSDT)", "CDW0[15:14]", psdt,
              psdt == 0   ? "00b (PRP)"
              : psdt == 1 ? "01b (SGL)"
                          : "reserved",
              "Selects how CDW6:CDW9 are interpreted: PRP entries for 00b, an SGL descriptor for "
              "01b. Command and controller capabilities constrain legal use.");
    field(out, "Command Identifier", "CDW0[31:16]", cdw0 >> 16, std::to_string(cdw0 >> 16),
          "Host-assigned command tag returned in the completion entry.");
    if (nsid_used)
        field(out, "Namespace Identifier", "CDW1[31:0]", c[1], std::to_string(c[1]),
              "Identifies the namespace targeted by this command; whether it is required or "
              "ignored depends on the command and feature scope.",
              8);
    if (!command_dwords_2_3_used) {
        field_text(out, "Reserved command dwords", "CDW2-CDW3", reserved_dwords_raw(c, 2, 3),
                   reserved_dwords_decoded(c, 2, 3),
                   "Reserved for this command; host shall set to zero.");
    }
    if (metadata_pointer_used) {
        const auto mptr = (static_cast<std::uint64_t>(c[5]) << 32) | c[4];
        field(out, "Metadata Pointer (MPTR)", "CDW4:CDW5[63:0]", mptr, hex_value(mptr, 16),
              "Points to the separate metadata buffer associated with this command when the "
              "namespace format and command use separate metadata.",
              16);
    }
    if (data_pointer_used) {
        const auto entry1 = (static_cast<std::uint64_t>(c[7]) << 32) | c[6];
        const auto entry2 = (static_cast<std::uint64_t>(c[9]) << 32) | c[8];
        field(out, "Data Pointer Entry 1", "CDW6:CDW7[63:0]", entry1, hex_value(entry1, 16),
              psdt == 0 ? "PRP1: first data page address, or the first-page pointer when a PRP "
                          "list is used."
                        : "First 64 bits of the SGL descriptor; descriptor subtype and length "
                          "semantics depend on its descriptor type.",
              16);
        field(out, "Data Pointer Entry 2", "CDW8:CDW9[63:0]", entry2, hex_value(entry2, 16),
              psdt == 0 ? "PRP2: second data page address or PRP-list pointer when required by the "
                          "transfer length and page boundary."
                        : "Second 64 bits of the SGL descriptor; descriptor subtype and length "
                          "semantics depend on its descriptor type.",
              16);
    }
}
} // namespace nvme_decoder::detail
