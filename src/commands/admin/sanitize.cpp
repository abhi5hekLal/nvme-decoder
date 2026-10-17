#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_admin_fields_sanitize(DecodedCommand& r, const Command& c, std::uint8_t op) {
    r.command_name = "Sanitize";
    r.use = "Starts the selected media sanitization action.";
    field(r, "Sanitize Action", "CDW10[2:0]", c[10] & 7u,
          (c[10] & 7u) == 1   ? "1 (Exit Failure Mode)"
          : (c[10] & 7u) == 2 ? "2 (Block Erase)"
          : (c[10] & 7u) == 3 ? "3 (Overwrite)"
          : (c[10] & 7u) == 4 ? "4 (Crypto Erase)"
          : (c[10] & 7u) == 5 ? "5 (Exit Media Verification State)"
          : (c[10] & 7u) == 0 ? "0 (reserved)"
                              : "reserved",
          "Selects the sanitize action to perform.");
    field(r, "Allow Unrestricted Sanitize Exit", "CDW10[3]", (c[10] >> 3) & 1u,
          std::to_string((c[10] >> 3) & 1u),
          "Controls whether unrestricted sanitize exit is allowed.");
    field(
        r, "Overwrite Pass Count", "CDW10[7:4]", (c[10] >> 4) & 0xfu,
        ((c[10] >> 4) & 0xfu) == 0 ? "0 (16 passes)"
                                   : std::to_string((c[10] >> 4) & 0xfu) + " passes",
        "Number of overwrite passes when overwrite sanitize is selected; zero encodes 16 passes.");
    field(r, "Overwrite Invert Pattern Between Passes", "CDW10[8]", (c[10] >> 8) & 1u,
          std::to_string((c[10] >> 8) & 1u),
          "Requests inversion of the overwrite pattern between passes.");
    field(r, "No Deallocate After Sanitize", "CDW10[9]", (c[10] >> 9) & 1u,
          std::to_string((c[10] >> 9) & 1u),
          "Controls whether logical blocks are deallocated after sanitize completes.");
    field(r, "Reserved", "CDW10[31:10]", c[10] >> 10, std::to_string(c[10] >> 10),
          "Reserved; host shall set to zero.");
    field(
        r, "Overwrite Pattern", "CDW11[31:0]", c[11], hex_value(c[11], 8),
        "32-bit pattern used by the overwrite sanitize action; ignored for other sanitize actions.",
        8);
    reserved_dwords(r, c, 12, 15);
}
} // namespace nvme_decoder::detail
