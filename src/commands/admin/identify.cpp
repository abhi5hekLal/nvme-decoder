#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_admin_fields_identify(DecodedCommand& r, const Command& c, std::uint8_t op) {
    r.command_name = "Identify";
    r.use = "Requests the Identify data structure selected by CNS and the command-set/controller "
            "selectors.";
    r.use = std::string("Retrieves ") + identify_name(c[10] & 0xffu) + " (CNS " +
            hex_value(c[10] & 0xffu, 2) + ").";
    field(r, "Identify CNS", "CDW10[7:0]", c[10] & 0xffu,
          std::string(hex_value(c[10] & 0xffu, 2)) + " (" + identify_name(c[10] & 0xffu) + ")",
          "Selects the Identify data structure returned.");
    field(r, "Reserved", "CDW10[15:8]", (c[10] >> 8) & 0xffu, std::to_string((c[10] >> 8) & 0xffu),
          "Reserved; host shall set to zero.");
    field(r, "Controller Identifier", "CDW10[31:16]", c[10] >> 16, std::to_string(c[10] >> 16),
          "Controller identifier used by CNS variants that select a controller.");
    field(r, "CNS-specific value", "CDW11[31:0]", c[11], hex_value(c[11], 8),
          "Interpretation depends on CNS; this value is not common to every Identify variant.", 8);
    field(r, "Command Set Identifier", "CDW14[31:24]", (c[14] >> 24) & 0xffu,
          hex_value((c[14] >> 24) & 0xffu, 2),
          "Selects the I/O command set for command-set-specific Identify data.");
    field(r, "UUID Index", "CDW14[6:0]", c[14] & 0x7fu, std::to_string(c[14] & 0x7fu),
          "Selects the UUID associated with the command when supported.");
}
} // namespace nvme_decoder::detail
