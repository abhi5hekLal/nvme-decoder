#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_admin_fields_get_log_page(DecodedCommand& r, const Command& c, std::uint8_t op) {
    r.command_name = "Get Log Page";
    r.use = "Retrieves the selected log page or log-page data range from the controller.";
    const auto lid = c[10] & 0xffu;
    const auto* lname = log_name(lid);
    r.use = std::string("Retrieves ") +
            (lname ? lname : "the selected/command-set-specific log page") + " (LID " +
            hex_value(lid, 2) + ").";
    field(r, "Log Page Identifier", "CDW10[7:0]", lid,
          lname ? std::string(hex_value(lid, 2)) + " (" + lname + ")" : hex_value(lid, 2),
          "Selects the log page to retrieve.");
    field(r, "Log Specific Parameter", "CDW10[14:8]", (c[10] >> 8) & 0x7fu,
          std::to_string((c[10] >> 8) & 0x7fu),
          "Selects a log-page-specific parameter; interpretation depends on the Log Page "
          "Identifier.");
    field(
        r, "Retain Asynchronous Event", "CDW10[15]", (c[10] >> 15) & 1u,
        std::to_string((c[10] >> 15) & 1u),
        "When set, the associated asynchronous event is not cleared by this Get Log Page command.");
    field(r, "Number of Dwords, Low", "CDW10[31:16]", c[10] >> 16, std::to_string(c[10] >> 16),
          "Lower 16 bits of the zero-based number of dwords to return (NUMD).");
    field(r, "Number of Dwords, Upper", "CDW11[15:0]", c[11] & 0xffffu,
          std::to_string(c[11] & 0xffffu),
          "Upper 16 bits of the zero-based dword count; NUMDU:NUMDL + 1 is the requested number of "
          "dwords.");
    field(
        r, "Requested Dword Count", "NUMDU:NUMDL",
        (static_cast<std::uint64_t>(c[11] & 0xffffu) << 16) | (c[10] >> 16),
        std::to_string(((static_cast<std::uint64_t>(c[11] & 0xffffu) << 16) | (c[10] >> 16)) + 1u),
        "Combined zero-based transfer length: NUMDU:NUMDL plus one dword.");
    field(r, "Log Specific Identifier", "CDW11[31:16]", c[11] >> 16, std::to_string(c[11] >> 16),
          "Identifier required by some log pages; reserved when the selected log page does not "
          "define it.");
    field(r, "Log Page Offset", "CDW12:CDW13[63:0]",
          (static_cast<std::uint64_t>(c[13]) << 32) | c[12],
          hex_value((static_cast<std::uint64_t>(c[13]) << 32) | c[12], 16),
          "Byte offset within the selected log page.", 16);
    field(r, "UUID Index", "CDW14[6:0]", c[14] & 0x7fu, std::to_string(c[14] & 0x7fu),
          "Selects the UUID associated with the command; zero indicates no UUID index.");
    field(r, "Offset Type", "CDW14[23]", (c[14] >> 23) & 1u,
          ((c[14] >> 23) & 1u) ? "1 (index offset)" : "0 (byte offset)",
          "Selects whether the offset fields specify a byte offset or an index into a list of "
          "structures in the log page.");
    field(r, "Reserved", "CDW14[22:7]", (c[14] >> 7) & 0x1ffffu,
          std::to_string((c[14] >> 7) & 0x1ffffu), "Reserved; host shall set to zero.");
    field(r, "Command Set Identifier", "CDW14[31:24]", (c[14] >> 24) & 0xffu,
          hex_value((c[14] >> 24) & 0xffu, 2),
          "Selects the command set when requesting a command-set-specific log page.");
    field(r, "Reserved", "CDW15[31:0]", c[15], hex_value(c[15], 8),
          "Reserved for the base Get Log Page command; host shall set to zero.", 8);
}
} // namespace nvme_decoder::detail
